/*
    Author: Alden Bernitt 2024/10/24

    PURPOSE:

    this file provides the definition for the define_elements function
    used by the abnf parser program.

    this function's ultimate goal is to convert string text into abnf data
    that can be used by other parts of the program. (see the official ABNF
    grammar description below)

    example text input may look something like --> 2*4( "a" "b" )

    the data extracted may include the repetition data (2 and 4),
    and the literal data ("a" and "b")

    DESCRIPTION:

    the define_elements function makes use of several helper functions, many
    of whose definitions constitute this file. in general, every sub-rule of
    the ABNF "elements" rule corresponds to at least one of these helper
    functions.

    (see below for a list of the ABNF rules and their definitions.)

    (it may be worth noting that the sequence of ABNF rules and the call chain
    of the helper functions match *very* closely.)

    most helper functions return an integer representing the error state of
    the function. these error codes include:

        ABNF_NOERR      --> the function completed successfully
        ABNF_PARSEERR   --> the function could not properly parse the text
                            data that it was given
        ABNF_MEMERR     --> an error occurred while allocating memory (for
                            whatever purpose)

    helper functions that do not return an integer code instead return a
    boolean value that represents success or failure to parse the text data
    that it was given.

    each function passes a shared stream-like structure containing the text
    data to be parsed. every function in this file promises to leave the
    stream-state unchanged if an ABNF_PARSEERR occurs. if the result of the
    function is ABNF_NOERR, the stream will be left in the position directly
    after the last character parsed. functions that only return a boolean
    value make the same promises.

    all abnf data created by the functions in this file is stored in a parent
    abnf_alt data structure. this data structure and its sub data structures
    are neither defined nor described in this file. as of the time of writing,
    these definitions and descriptions can be found in the abnf_defs.h file.

    STRUCTURE:

    the function flow begins with a call to the define_elements function.
    this function is given a stream-like structure containing the text to
    be parsed, and a pointer to an abnf_alt structure whose referenced value
    shall store the parsed data. the define_elements function will make calls
    to helper functions to parse the various portions of the input text. These
    helper functions may call other helper functions, which may further call
    other helper functions.

    for example, the define_elements function will make calls to the
    define_alt and parse_cwsp functions. the define_alt function will make
    calls to the define_cat and parse_cwsp functions. the define_cat function
    will make calls to the define_rep and parse_cwsp functions, and so forth,
    until concrete values are finally parsed and stored by the define_terminal
    function and its helper functions.

        define_elements --> define_alt --> define_cat
        --> ... --> define_terminal

    specifically, the define_terminal function and its helper functions are
    responsible for parsing and storing data corresponding to the char-val,
    bin-val, dec-val, hex-val, and prose-val rules found in the ABNF grammar
    (listed below).

    EXAMPLE:

    given the input text --> 2*4( "a" "b" )
    the (simplified) function call chain will be executed as:

    (DEPTH) (FUNCTION)      --> (STREAM STATE)
    ------------------------------------------
    (1)     define_elements --> 2*4( "a" "b" ) 
    (1)     define_alt      --> 2*4( "a" "b" )
    (1)     define_cat      --> 2*4( "a" "b" )
    (1)     define_rep      --> 2*4( "a" "b" )  // parse and store 2*4
    (1)     define_ele      --> ( "a" "b" )     // parse "(" alternation ")"

    (2)     define_alt      --> "a" "b" )
    (2)     define_cat      --> "a" "b" )

    (ITERATION 1)
    (2)     define_rep      --> "a" "b" )
    (2)     define_ele      --> "a" "b" )
    (2)     define_terminal --> "a" "b" )       // parse and store "a"

    (ITERATION 2)
    (2)     define_rep      --> "b" )
    (2)     define_ele      --> "b" )
    (2)     define_terminal --> "b" )           // parse and store "b"

    (1)     define_ele      --> )
    (1)     define_rep      -->
    ...     ...             -->
    (1)     define_elements -->

    (FINISH)
*/

/*
    From RFC 5234 (Augmented BNF for Syntax Specifications: ABNF)

    elements       =  alternation *c-wsp
 
    alternation    =  concatenation
                      *(*c-wsp "/" *c-wsp concatenation)

    concatenation  =  repetition *(1*c-wsp repetition)

    repetition     =  [repeat] element

    repeat         =  1*DIGIT / (*DIGIT "*" *DIGIT)

    element        =  rulename / group / option /
                      char-val / num-val / prose-val

    group          =  "(" *c-wsp alternation *c-wsp ")"

    option         =  "[" *c-wsp alternation *c-wsp "]"

    char-val       =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                           ; quoted string of SP and VCHAR
                           ;  without DQUOTE

    bin-val        =  "b" 1*BIT
                      [ 1*("." 1*BIT) / ("-" 1*BIT) ]
                           ; series of concatenated bit values
                           ;  or single ONEOF range

    dec-val        =  "d" 1*DIGIT
                      [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ]

    hex-val        =  "x" 1*HEXDIG
                      [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ]

    prose-val      =  "<" *(%x20-3D / %x3F-7E) ">"
                           ; bracketed string of SP and VCHAR
                           ;  without angles
                           ; prose description, to be used as
                           ;  last resort
*/
#define define_quotstr(term, in, vals)\
    define_literals(term, in, vals, is_charch, '"', '"')
#define define_prose(term, in, vals)\
    define_literals(term, in, vals, is_prosech, '<', '>')

#define define_values_single(buf, rad, term)\
    define_values_concat(buf, rad, term)

#define define_binval(term, in, varr) define_xval(term, in, varr, 'b', 2)
#define define_decval(term, in, varr) define_xval(term, in, varr, 'd', 10)
#define define_hexval(term, in, varr) define_xval(term, in, varr, 'x', 16)

#define define_group(in, alt) define_bracketed(in, alt, '(', ')')
#define define_option(in, alt) define_bracketed(in, alt, '[', ']')

// used by define_values to determine how its text input should be interpreted.
enum {
    // e.g "61"
    VALMODE_SINGLE,

    // e.g "61.62.63"
    VALMODE_CONCAT,

    // e.g "61-63"
    VALMODE_RANGE,

    VALMODE_ERR
};

static int define_alt(struct chstream* in, struct abnf_alt* alt);
static int define_name(struct chstream* in, char** out);

static _Bool is_charch(char c)
{
    return c == 0x20 || c == 0x21 || 0x23 <= c && c <= 0x7E;
}

static _Bool is_prosech(char c)
{
    return 0x20 <= c && c <= 0x3D || 0x3F <= c && c <= 0x7E;
}

static int define_cwsaltcws(struct chstream* in, struct abnf_alt** alt)
{
    // *c-wsp alternation *c-wsp
    //
    // allocate memory for an abnf_alt and assign it to the location
    // referenced by <alt>.

    size_t reset = in->pos;

    while(parse_cwsp(in))
        ; // EMPTY WHILE

    int errcode;
    *alt = (struct abnf_alt*)malloc(sizeof(struct abnf_alt));

    if (*alt == NULL) {
        return ABNF_MEMERR;
    }

    if ((errcode = define_alt(in, *alt)) != ABNF_NOERR) {
        abnf_freealt(*alt);
        free(*alt);
        *alt = NULL;

        chstream_set(in, reset);

        return errcode;
    }

    while(parse_cwsp(in))
        ; // EMPTY WHILE

    return ABNF_NOERR;
}

static int define_bracketed(
        struct chstream* in,
        struct abnf_alt** alt,
        char open,
        char close
) {
    // "OPEN" *c-wsp alternation *c-wsp "CLOSE"
    // used to parse group and option.

    if (!parse_ch(in, open)) {
        return ABNF_PARSEERR;
    }

    int errcode;
    if ((errcode = define_cwsaltcws(in, alt)) != ABNF_NOERR) {
        return errcode;
    }

    return parse_ch(in, close) ? ABNF_NOERR : ABNF_PARSEERR;
}


static int define_literals(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* vals,
        _Bool (*rangepred)(char),
        char open,
        char close
) {
    // "OPEN" *(LITERALS) "CLOSE"
    // used to parse char-val and prose-val
    // parsed values are store in <values>

    term->mode = TERM_AND;

    if (!parse_ch(in, open)) {
        return ABNF_PARSEERR;
    }

    char c = chstream_peek(in, 0); 
    while (rangepred(c)) {
        chstream_advance(in, 1);

        if (strbuffer_concatch(vals, c) != STRBUF_NOERR) {
            return ABNF_MEMERR;
        }

        c = chstream_peek(in, 0);
    }

    if (!parse_ch(in, close)) {
        return ABNF_PARSEERR;
    }

    return ABNF_NOERR;
}

static _Bool(*radix_to_pred(int radix))(char)
{
    switch (radix) {
        case 2:
            return is_bit;
        case 16:
            return is_hex;
        default:
            return is_dec;
    }
}

static int read_valseq(
        struct chstream* in,
        _Bool(*valpred)(char),
        strbuffer* out
) {
    // 1*VAL
    // parse and store values into <out>

    char c;

    if (read_val(in, valpred, &c)) {
        int memerr = strbuffer_concatch(out, c);
        while (memerr == STRBUF_NOERR && read_val(in, valpred, &c)) {
            memerr = strbuffer_concatch(out, c);
        }

        if (memerr != STRBUF_NOERR) {
            return ABNF_MEMERR;
        }
    }
    else {
        return ABNF_PARSEERR;
    }

    return ABNF_NOERR;
}

static int appendch_then_read_valseq(
        struct chstream* in,
        _Bool(*valpred)(char),
        char c,
        strbuffer* out
) {
        if (strbuffer_concatch(out, c) != STRBUF_NOERR) {
            return ABNF_MEMERR;
        }
        chstream_advance(in, 1);

        return read_valseq(in, valpred, out);
}

static int read_values_concat(
        struct chstream* in,
        _Bool (*valpred)(char),
        strbuffer* out
) {
    // 1*( "." 1*VAL)
    // parse and store text into <out>

    int errcode = appendch_then_read_valseq(in, valpred, '.', out);

    while (errcode == ABNF_NOERR && chstream_peek(in, 0) == '.') {
        errcode = appendch_then_read_valseq(in, valpred, '.', out);
    }

    return errcode;
}

static int read_value_string(
        struct chstream* in,
        _Bool (*valpred)(char),
        int* valmode,
        strbuffer* out
) {
    // 1*VAL [ 1*("." 1*VAL) / ("-" 1*VAL) ]
    // 1*VAL 1*("." 1*VAL) is a series of concatenated values.
    // 1*VAL ("-" 1*VAL) is a range of values.
    //
    // VAL is determined by <valpred>
    // parse and store text into <out>

    // 1*VAL
    int errcode = read_valseq(in, valpred, out);
    *valmode = VALMODE_SINGLE;

    if (errcode != ABNF_NOERR) {
        return errcode;
    }

    char c = chstream_peek(in, 0);

    // 1*("." 1*VAL)
    if (c == '.') {
        *valmode = VALMODE_CONCAT;
        errcode = read_values_concat(in, valpred, out);
    }

    // "-" 1*VAL
    else if (c == '-') {
        *valmode = VALMODE_RANGE;
        errcode = appendch_then_read_valseq(in, valpred, c, out);
    }

    if (errcode != ABNF_NOERR) {
        *valmode = VALMODE_ERR;
    }

    return errcode;
}

static int define_values_concat(
        strbuffer* src,
        int radix,
        strbuffer* values
) {
    char* buf = strbuffer_buffer(src);
    size_t len = strbuffer_len(src);

    int i;
    char c;
    while ((i = str_chfind(buf, len, '.')) >= 0) {
        buf[i] = '\0';
        c = (char)strtoi(buf, radix);

        if (strbuffer_concatch(values, c) != STRBUF_NOERR) {
            return ABNF_MEMERR;
        }

        buf += i + 1;
        len -= i + 1;
    }

    // there should be one last number string to convert after the last
    // delimiting '.' has been found.
    c = (char)strtoi(buf, radix);
    if (strbuffer_concatch(values, c) != STRBUF_NOERR) {
        return ABNF_MEMERR;
    }

    return ABNF_NOERR;
}

static int define_values_range(
        strbuffer* src,
        int radix,
        strbuffer* values
) {
    char* buf = strbuffer_buffer(src);
    int delim = str_chfind(buf, strbuffer_len(src), '-');

    buf[delim] = '\0';

    int lower = strtoi(buf, radix);

    buf += delim + 1;
    
    int upper = strtoi(buf, radix);

    while (lower <= upper) {
        if (strbuffer_concatch(values, lower) != STRBUF_NOERR) {
            return ABNF_MEMERR;
        }

        lower++;
    }

    return ABNF_NOERR;
}

static int define_values(
        struct abnf_term* term,
        strbuffer* src,
        int radix,
        strbuffer* values,
        int mode,
        int errcode
) {
    switch (mode) {
        // 1*VAL
        case VALMODE_SINGLE:
            term->mode = TERM_AND;
            errcode = define_values_single(src, radix, values);
            break;

        // 1*VAL 1*("." 1*VAL)
        case VALMODE_CONCAT:
            term->mode = TERM_AND;
            errcode = define_values_concat(src, radix, values);
            break;

        // 1*VAL "-" 1*VAL
        case VALMODE_RANGE:
            term->mode = TERM_OR;
            errcode = define_values_range(src, radix, values);
            break;
    }

    return errcode;
}

static int define_xval(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* values,
        char symbol,
        int radix
) {
    // parse "SYMBOL" 1*VAL [ 1*("." 1*VAL) / ("-" 1*VAL) ]
    // this function is used to parse dec-val, bin-val, and hex-val.

    // "SYMBOL"
    if (!parse_ch(in, symbol)) {
        return ABNF_PARSEERR;
    }

    int errcode = ABNF_NOERR;
    strbuffer src;
    if (strbuffer_init(&src) != STRBUF_NOERR) {
        return ABNF_MEMERR;
    }

    // read 1*VAL [ 1*("." 1*VAL) / ("-" 1*VAL) ] into <str>.
    // record the mode (single, concat, or range) in <mode>.
    int mode;
    errcode = read_value_string(in, radix_to_pred(radix), &mode, &src);

    // convert the string in <str> into values and store them in <values>.
    // for example, given <radix> == 16:
    // "61" gets stored as "a"
    // "61.62.63" gets stored as "abc"
    // "61-63" gets as "abc"
    if (errcode == ABNF_NOERR) {
        errcode = define_values(term, &src, radix, values, mode, errcode);
    }

    strbuffer_free(&src);
    return errcode;
}

static int define_numval(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* values
) {
    // num-val = "%" (bin-val / dec-val / hex-val)
    
    int errcode;

    if (!parse_ch(in, '%')) {
        return ABNF_PARSEERR;
    }

    // unassigned boolean expression used for short-circuiting
    (errcode = define_binval(term, in, values)) == ABNF_NOERR
        || (errcode = define_decval(term, in, values)) == ABNF_NOERR
        || (errcode = define_hexval(term, in, values));

    return errcode;
}


static int define_senstr(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* values
) {
    // "%s" quoted-string

    size_t reset = in->pos;

    if (!(parse_ch(in, '%') && parse_ch(in, 's'))) {
        chstream_set(in, reset);
        return ABNF_PARSEERR;
    }

    int errcode = define_quotstr(term, in, values);

    if (errcode == ABNF_NOERR) {
        term->sens = TERM_SENS;
    }

    return errcode;
}

static int define_insenstr(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* values
) {
    // [ "%i" ] quoted-string

    size_t reset = in->pos;

    if (!(parse_ch(in, '%') && parse_ch(in, 'i'))) {
        chstream_set(in, reset);
    }

    int errcode = define_quotstr(term, in, values);

    if (errcode == ABNF_NOERR) {
        term->sens = TERM_INSENS;
    }
    
    return errcode;
}

static int define_charval(
        struct abnf_term* term,
        struct chstream* in,
        strbuffer* values
) {
    // case-insensitive-string / case-sensitive-string
    int errcode;

    (errcode = define_insenstr(term, in, values)) == ABNF_NOERR
        || (errcode = define_senstr(term, in, values));

    return errcode;
}

static int define_terminal_handle_memory(
        struct abnf_term* term,
        strbuffer* values,
        int errcode
) {
    int memcode = strbuffer_shrink(values);
    strbuffer_move(values, &term->values);

    if (errcode == ABNF_NOERR && memcode != STRBUF_NOERR) {
        errcode = ABNF_MEMERR;
    }

    // free allocated memory if anything went wrong.
    if (errcode != ABNF_NOERR) {
        free(term->values);
        term->values = NULL;
    }

    return errcode;

}

static int define_terminal(struct chstream* in, struct abnf_term* term)
{
    // char-val / num-val / prose-val

    int errcode;
    strbuffer values;

    if (strbuffer_init(&values) != STRBUF_NOERR) {
        return ABNF_MEMERR;
    }

    // unassigned boolean expression used for short-circuiting
    (errcode = define_charval(term, in, &values)) == ABNF_NOERR
        || (errcode = define_numval(term, in, &values)) == ABNF_NOERR
        || (errcode = define_prose(term, in, &values));

    return define_terminal_handle_memory(term, &values, errcode);
}

static int define_ele(struct chstream* in, struct abnf_ele* ele)
{
    // element = rulename / group / option / char-val / num-val / prose-val

    int errcode;

    if ((errcode = define_name(in, &ele->value.rulename)) == ABNF_NOERR) {
        ele->type = ELE_RULE;
    }
    else if ((errcode = define_group(in, &ele->value.alt)) == ABNF_NOERR) {
        ele->type = ELE_GROUP;
    }
    else if ((errcode = define_option(in, &ele->value.alt)) == ABNF_NOERR) {
        ele->type = ELE_OPTION;
    }
    else if (
            // char-val / num-val / prose-val
            (errcode = define_terminal(in, &ele->value.term))
            == ABNF_NOERR
    ) {
        ele->type = ELE_TERMINAL;
    }

    return errcode;
}

static int define_rep_handle_memory(
        struct varr* reparr,
        struct abnf_rep* rep,
        int errcode
) {
    if (errcode == ABNF_NOERR && varr_append(reparr, rep) != VARR_NOERR) {
        errcode = ABNF_MEMERR;
    }

    if (errcode != ABNF_NOERR) {
        abnf_freerep(rep);
    }

    return errcode;
}

static _Bool define_repeat(struct chstream* in, int* minout, int* maxout)
{
    // repeat = 1*DIGIT / (*DIGIT "*" *DIGIT)

    _Bool minread, maxread;
    int min, max;

    /*
        convert a string of digits into its corresponding decimal value.
    */
    if (is_digit(chstream_peek(in, 0))) {
        minread = parse_int(in, &min);
    }

    if (chstream_peek(in, 0) == '*') {
        chstream_advance(in, 1);

        maxread = parse_int(in, &max);

        // 1*DIGIT "*" 1*DIGIT
        if (minread && maxread) {
            *minout = min;
            *maxout = (max < min) ? min : max;
        }
        
        // 1*DIGIT "*"
        else if (minread) {
            *minout = min;
            *maxout = REP_UNDEF;
        }
        
        // "*" 1*DIGIT
        else if (maxread)  {
            *minout = 0;
            *maxout = max;
        }

        // "*"
        else {
            *minout = 0;
            *maxout = REP_UNDEF;
        }
    }

    // 1*DIGIT
    else if (minread) {
        *minout = min;
        *maxout = min;
    }

    // this case corresponds to no digits and no "*" being parsed
    else {
        return false;
    }

    return true;
}


static int define_rep(struct chstream* in, struct varr* reparr)
{
    // repetition = [repeat] element

    int errcode;
    struct abnf_rep rep;
    abnf_initrep(&rep);
    rep.min = 1, rep.max = 1;

    define_repeat(in, &rep.min, &rep.max);
    errcode = define_ele(in, &rep.ele);

    return define_rep_handle_memory(reparr, &rep, errcode);
}

static int define_rep_optional(struct chstream* in, struct varr* reparr)
{
    int errcode = ABNF_NOERR;

    // *(1*c-wsp repetition)
    while (errcode == ABNF_NOERR) {

        size_t reset = in->pos;

        if (parse_cwsp(in)) {
            while (parse_cwsp(in))
                ; // EMPTY WHILE

            int code = define_rep(in, reparr);

            if (code == ABNF_PARSEERR) {
                chstream_set(in, reset);
                break;
            }

            // a repetition may have been found.
            else {
                errcode = code;
            }
        }
        
        // no initial c-wsp was found; exit normally.
        else {
            break;
        }
    }

    return errcode;
}

static int define_cat_handle_memory(
        struct varr* catarr,
        struct abnf_cat* cat,
        struct varr* reps,
        int errcode
) {
    // transfer repetition data to the abnf_cat structure.
    cat->count = reps->len;

    int memcode = varr_shrink(reps);
    varr_move(reps, (void**)&cat->reps);

    // attempt to append the concatenation data to the concatenation data
    // array as long as there were no issues transferring the repetition data.
    // --set the error code if an issue occurred.
    if (
            errcode == ABNF_NOERR
            && (memcode != VARR_NOERR
                || varr_append(catarr, cat) != VARR_NOERR)
    ) {
        errcode = ABNF_MEMERR;
    }

    // free concatenation data if any issues occurred.
    // note that the data from the abnf_rep array was moved to the
    // abnf_cat structure, so freeing just the abnf_cat structure
    // suffices.
    if (errcode != ABNF_NOERR) {
        abnf_freecat(cat);
    }

    return errcode;
}

static int define_cat(struct chstream* in, struct varr* catarr)
{
    // concatenation = repetition *(1*c-wsp repetition)

    int errcode;
    struct abnf_cat cat;
    struct varr reps;

    abnf_initcat(&cat);
    if (varr_init(&reps, sizeof(struct abnf_rep)) != VARR_NOERR) {
        return ABNF_MEMERR;
    }

    errcode = define_rep(in, &reps);
    if (errcode == ABNF_NOERR) {
        errcode = define_rep_optional(in, &reps);
    }

    return define_cat_handle_memory(catarr, &cat, &reps, errcode);
}


static int define_cat_optional(struct chstream* in, struct varr* catarr)
{

    int errcode = ABNF_NOERR;

    // parse *(*c-wsp "/" *c-wsp concatenation)
    while (errcode == ABNF_NOERR) {
        // required to reset to input in the case that the optional read fails.
        size_t reset = in->pos;

        while (parse_cwsp(in))
            ; // EMPTY WHILE

        if (!parse_ch(in, '/')) {
            chstream_set(in, reset);
            break;
        }

        while (parse_cwsp(in))
            ; // EMPTY WHILE

        int code = define_cat(in, catarr);

        if (code == ABNF_PARSEERR) {
            chstream_set(in, reset);
            break;
        }

        // report an error if it wasn't a parse error.
        else if (code != ABNF_NOERR) {
            errcode = code;
            break;
        }
    }

    return errcode;
}

static int define_alt_handle_memory(
        struct abnf_alt* alt,
        struct varr* cats,
        int errcode
) {
    // transfer the concatenation data to the abnf_alt structure.
    alt->count = cats->len;

    int memcode = varr_shrink(cats);
    varr_move(cats, (void**)&alt->cats);

    if (errcode == ABNF_NOERR && memcode != VARR_NOERR) {
        errcode = ABNF_MEMERR;
    }

    // free the alternation data if an error occurred.
    // note that the abnf_cat array was transferred to the
    // abnf_alt structure, so just freeing the abnf_alt
    // is sufficient.
    if (errcode != ABNF_NOERR) {
        abnf_freealt(alt);
    }
    
    return errcode;
}

static int define_alt(struct chstream* in, struct abnf_alt* alt)
{
    // alternation = concatenation *(*c-wsp "/" *c-wsp concatenation)
    
    int errcode;
    struct varr cats;

    if (varr_init(&cats, sizeof(struct abnf_cat)) != VARR_NOERR) {
        return ABNF_MEMERR;
    }

    errcode = define_cat(in, &cats);
    if (errcode == ABNF_NOERR) {
        errcode = define_cat_optional(in, &cats);
    }

    return define_alt_handle_memory(alt, &cats, errcode);
}

static int define_elements(struct chstream* in, struct abnf_alt* alt)
{
    // elements = alternation *c-wsp
    
    int errcode = define_alt(in, alt);

    if (errcode == ABNF_NOERR) {
        while (parse_cwsp(in))
            ; // EMPTY WHILE
    }

    return errcode;
}
