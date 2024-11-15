#include "abnf.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define PARSERULE "parse"
#define RETRIEVERULE "retrieve"

#define MAXRULEC 16

#define test_onerule_noerr(e, in) test_onerule(e, in, ABNF_NOERR)

#define test_onerule_nested_noerr(e, in)\
    test_onerule_nested(e, in, ABNF_NOERR)

#define test_onerule_repeated_noerr(e, in, index, out)\
    test_onerule_repeated(e, in, index, out, ABNF_NOERR)

#define test_onerule_repeated_dne(e, in, index)\
    test_onerule_repeated(e, in, index, "", ABNF_DNE)

#define test_abnfgrammar_noerr(rule, in, retr, reti, exo)\
    test_abnfgrammar(rule, in, retr, reti, exo, ABNF_NOERR)

#define test_abnfgrammar_dne(rule, in, retr, reti, exo)\
    test_abnfgrammar(rule, in, retr, reti, exo, ABNF_DNE)

struct abnf abnfgrammar;

typedef struct {
    const char* elements;
    const char* input;
    int index;
    const char* expout;
    int expcode;
} oneruledat;

typedef struct {
    const char** rulelist;
    size_t rulec;
    const char* input;
    const char* retname;
    size_t retindex;
    const char* expout;
    int expcode;
} concatruledat;

char* concat_str(const char* s1, const char* s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    void* p = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!p) {
        return NULL;
    }

    char* str = (char*)p;

    size_t i;
    for (i = 0; i < len1; i++) {
        str[i] = s1[i];
    }
    for (size_t j = 0; j < len2; j++) {
        str[i++] = s2[j];
    }
    str[i] = '\0';

    return str;
}

_Bool perform_test(
        void* extra,
        _Bool (*test)(struct abnf*, struct abnf_result*, void*)
) {
    struct abnf grammar;
    struct abnf_result result;
    abnf_init(&grammar);
    abnf_initresult(&result);

    _Bool success = test(&grammar, &result, extra);

    abnf_freeresult(&result);
    abnf_free(&grammar);
    return success;
}

_Bool execute_rules(
        struct abnf* grammar,
        struct abnf_result* result,
        const char* rules[],
        size_t rulec,
        const char* input,
        const char* retname,
        int retindex,
        const char* expout,
        int expcode
) {
    for (size_t i = 0; i < rulec; i++) {
        if (abnf_define(grammar, rules[i]) != ABNF_NOERR) {
            return false;
        }
    }

    if (abnf_parse(grammar, PARSERULE, input, result) != ABNF_NOERR) {
        return false;
    }

    char* value;
    int code = abnf_retrieve(result, retname, retindex, &value);

    if (code != expcode) {
        return false;
    }

    if (code == ABNF_NOERR) {
        return strcmp(value, expout) == 0;
    }

    return true;
}

_Bool test_onerule_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    oneruledat* dat = (oneruledat*)extra;

    const char* rule1 = PARSERULE"="RETRIEVERULE;
    char* rule2 = concat_str(RETRIEVERULE"=", dat->elements);
    if (!rule2) {
        return false;
    }

    const char* rulelist[] = {rule1, rule2};

    _Bool success = execute_rules(
            grammar,
            result,
            rulelist,
            2,
            dat->input,
            RETRIEVERULE,
            dat->index,
            dat->expout,
            dat->expcode
    );

    free(rule2);
    return success;
}

_Bool test_onerule(const char* elements, const char* input, int expcode)
{
    oneruledat dat = {elements, input, 0, input, expcode};
    return perform_test(&dat, test_onerule_helper);
}

_Bool test_onerule_nested_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    oneruledat* dat = (oneruledat*)extra;

    const char* rule1 = PARSERULE"="RETRIEVERULE;
    const char* rule2 = RETRIEVERULE"=nested";
    char *rule3 = concat_str("nested=", dat->elements);
    if (!rule3) {
        return false;
    }

    const char* rulelist[] = {rule1, rule2, rule3};

    _Bool success = execute_rules(
            grammar,
            result,
            rulelist,
            3,
            dat->input,
            RETRIEVERULE,
            dat->index,
            dat->expout,
            dat->expcode
    );

    free(rule3);
    return success;
}

_Bool test_onerule_nested(
        const char* elements,
        const char* input,
        int expcode
) {
    oneruledat dat = {elements, input, 0, input, expcode};
    return perform_test(&dat, test_onerule_nested_helper);
}

_Bool test_onerule_repeated_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    oneruledat* dat = (oneruledat*)extra;

    const char* rule1 = PARSERULE"=*"RETRIEVERULE;
    char* rule2 = concat_str(RETRIEVERULE"=", dat->elements);
    if (!rule2) {
        return false;
    }

    const char* rulelist[] = {rule1, rule2};

    _Bool success = execute_rules(
            grammar,
            result,
            rulelist, 
            2,
            dat->input,
            RETRIEVERULE,
            dat->index,
            dat->expout,
            dat->expcode
    );

    free(rule2);
    return success;
}

_Bool test_onerule_repeated(
        const char* elements,
        const char* input,
        int index,
        const char* expout,
        int expcode
) {
    oneruledat dat = {elements, input, index, expout, expcode};
    return perform_test(&dat, test_onerule_repeated_helper);
}

_Bool test_rules_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    concatruledat* dat = (concatruledat*)extra;
    return execute_rules(
        grammar,
        result,
        dat->rulelist,
        dat->rulec,
        dat->input,
        dat->retname,
        dat->retindex,
        dat->expout,
        dat->expcode
    );
}

_Bool test_rules(
        const char* elements,
        const char* input,
        const char* retname,
        int retindex,
        const char* expout,
        int expcode,
        ...
) {
    va_list args;
    va_start(args, expcode);

    char* baserule = concat_str(PARSERULE"=", elements);
    if (!baserule) {
        return false;
    }

    const char* rulelist[MAXRULEC];
    rulelist[0] = baserule;

    size_t rulec = 1;
    const char* rule = va_arg(args, const char*);
    while (strcmp(rule, "") != 0 && rulec <= MAXRULEC) {
        rulelist[rulec++] = rule;
        rule = va_arg(args, const char*);
    }

    concatruledat dat =
            {rulelist, rulec, input, retname, retindex, expout, expcode};
    _Bool success = perform_test(&dat, test_rules_helper);

    va_end(args);
    free(baserule);
    return success;
}

_Bool test_abnfgrammar(
        const char* rule,
        const char* input,
        const char* retrule,
        int retindex,
        const char* expout,
        int expcode
) {
    struct abnf_result result;
    abnf_initresult(&result);

    if (abnf_parse(&abnfgrammar, rule, input, &result) != ABNF_NOERR) {
        return false;
    }

    _Bool success = true;

    char* s;
    if (abnf_retrieve(&result, retrule, retindex, &s) == expcode) {
        if (expcode == ABNF_NOERR) {
            success = strcmp(s, expout) == 0;
        }
    }
    else {
        success = false;
    }

    abnf_freeresult(&result);
    return success;
}

//@PRE
_Bool setup()
{
    abnf_init(&abnfgrammar);

    /*
        abnf grammar definitions as described in
        RFC 5234 (Augmented BNF for Syntax Specifications: ABNF)
    */
    return abnf_define(&abnfgrammar,
            "rulelist\t =  1*( rule / (*c-wsp c-nl) )") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "rule\t =  rulename defined-as elements c-nl\r\n"
            "\t ; continues if next line starts\r\n"
            "\t ;  with white space") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "rulename\t =  ALPHA *(ALPHA / DIGIT / \"-\")") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "defined-as\t =  *c-wsp (\"=\" / \"=/\") *c-wsp\r\n"
            "\t\t ; basic rules definition and\r\n"
            "\t\t ;  incremental alternatives") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "elements\t =  alternation *c-wsp") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "c-wsp\t =  WSP / (c-nl WSP)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "c-nl\t =  comment / CRLF"
            "\r\n\t\t ; comment or newline") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "comment\t =  \";\" *(WSP / VCHAR) CRLF") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "alternation\t =  concatenation\r\n"
            "\t\t*(c-wsp \"/\" *c-wsp concatenation)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "concatenation\t =  "
            "repetition *(1*c-wsp repetition)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "repetition\t =  [repeat] element") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "repeat\t =  1*DIGIT / (*DIGIT \"*\" *DIGIT)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "element\t =  rulename / group / option /\r\n"
            "\t\tchar-val / num-val / prose-val") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "group\t =  \"(\" *c-wsp alternation *c-wsp \")\""
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "option\t =  \"[\" *c-wsp alternation *c-wsp \"]\""
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "char-val\t =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE\r\n"
            "\t\t ; quoted string of SP and VCHAR\r\n"
            "\t\t ;  without DQUOTE") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "num-val\t =  \"%\" (bin-val / dec-val / hex-val)"
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "bin-val\t =  \"b\" 1*BIT\r\n"
            "\t\t[ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]\r\n"
            "\t\t ; series of concatenated bit values\r\n"
            "\t\t ;   or single ONEOF range") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "dec-val\t =  \"d\" 1*DIGIT\r\n"
            "\t\t[ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]\r\n") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "hex-val\t =  \"x\" 1*HEXDIG\r\n"
            "\t\t[ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]\r\n") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "prose-val\t =  \"<\" *(%x20-3D / %x3F-7E) \">\"\r\n"
            "\t\t ; bracketed string of SP and VCHAR\r\n"
            "\t\t ;  without angles\r\n"
            "\t\t ; prose description, to be used as\r\n"
            "\t\t ;  last resort") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "ALPHA\t =  %x41-5A / %x61-7A\t; A-Z / a-z") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "BIT\t =  \"0\" / \"1\"") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CHAR\t =  %x01-7F\r\n"
            "\t\t ; any 7-bit US-ASCII character,\r\n"
            "\t\t ; excluding NUL") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CR\t =  %x0D\r\n\t\t ; carriage return") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CRLF\t =  CR LF\r\n"
            "\t\t ; Internet standard newline") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CTL\t =  %x00-1F / %x7F\r\n\t\t ; controls") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "DIGIT\t =  %x30-39\r\n\t\t ; 0-9") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "DQUOTE\t =  %x22\r\n\t\t ; \" (Double Quote)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "HEXDIG\t =  DIGIT / \"A\" / \"B\" / \"C\" / \"D\" / \"E\" "
            "/ \"F\"") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "HTAB\t =  %x09\r\n\t\t ; horizontal tab") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "LF\t =  %x0A\r\n\t\t ; linefeed") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "LWSP\t =  *(WSP / CRLF WSP)\r\n"
            "\t\t ; Use of this linear-white-space rule\r\n"
            "\t\t ;  permits lines containing only white\r\n"
            "\t\t ;  space that are no longer legal in\r\n"
            "\t\t ;  mail headers and have caused\r\n"
            "\t\t ;  interoperability problems in other\r\n"
            "\t\t ;  contexts.\r\n"
            "\t\t ; Do not use when defining mail\r\n"
            "\t\t ;  headers and use with caution in\r\n"
            "\t\t ;  other contexts.") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "OCTET\t =  %x00-FF\r\n\t\t ; 8 bits of data") == ABNF_NOERR
        && abnf_define(&abnfgrammar, "SP\t =  %x20") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "VCHAR\t =  %x21-7E\r\n"
            "\t\t ; visible (printing) characters") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "WSP\t =  SP / HTAB\r\n\t\t ; white space") == ABNF_NOERR;
}


//@POST
_Bool finish()
{
    abnf_free(&abnfgrammar);
    return true;
}



//@TEST
_Bool retrieve_onerule_char_one()
{
    return test_onerule_noerr("\"<\"", "<");
}

//@TEST
_Bool retrieve_onerule_char_many()
{
    return test_onerule_noerr("\"<><\"", "<><");
}

//@TEST
_Bool retrieve_onerule_binval_one()
{
    return test_onerule_noerr("%b01100101", "e");
}

//@TEST
_Bool retrieve_onerule_binval_many()
{
    return test_onerule_noerr("%b01100101.01100101.01100101", "eee");
}

//@TEST
_Bool retrieve_onerule_binval_range_min()
{
    return test_onerule_noerr("%b01100001-01100011", "a");
}

//@TEST
_Bool retrieve_onerule_binval_range_middle()
{
    return test_onerule_noerr("%b01100001-01100011", "b");
}

//@TEST
_Bool retrieve_onerule_binval_range_max()
{
    return test_onerule_noerr("%b01100001-01100011", "c");
}

//@TEST
_Bool retrieve_onerule_decval_one()
{
    return test_onerule_noerr("%d101", "e");
}

//@TEST
_Bool retrieve_onerule_decval_many()
{
    return test_onerule_noerr("%d101.101.101", "eee");
}

//@TEST
_Bool retrieve_onerule_decval_range_min()
{
    return test_onerule_noerr("%d97-99", "a");
}

//@TEST
_Bool retrieve_onerule_decval_range_middle()
{
    return test_onerule_noerr("%d97-99", "b");
}

//@TEST
_Bool retrieve_onerule_decval_range_max()
{
    return test_onerule_noerr("%d97-99", "c");
}

//@TEST
_Bool retrieve_onerule_hexval_one()
{
    return test_onerule_noerr("%x65", "e");
}

//@TEST
_Bool retrieve_onerule_hexval_many()
{
    return test_onerule_noerr("%x65.65.65", "eee");
}

//@TEST
_Bool retrieve_onerule_hexval_range_min()
{
    return test_onerule_noerr("%x61-63", "a");
}

//@TEST
_Bool retrieve_onerule_hexval_range_middle()
{
    return test_onerule_noerr("%x61-63", "b");
}

//@TEST
_Bool retrieve_onerule_hexval_range_max()
{
    return test_onerule_noerr("%x61-63", "c");
}

//@TEST
_Bool retrieve_onerule_prose_one()
{
    return test_onerule_noerr("<\">", "\"");
}

//@TEST
_Bool retrieve_onerule_prose_many()
{
    return test_onerule_noerr("<\"\"\">", "\"\"\"");
}

//@TEST
_Bool retrieve_onerule_group()
{
    return test_onerule_noerr("( \"abc\" )", "abc");
}

//@TEST
_Bool retrieve_onerule_group_repeat()
{
    return test_onerule_noerr("3( \"abc\" )", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_repeat_zero()
{
    return test_onerule_noerr("0( \"abc\" )", "");
}

//@TEST
_Bool retrieve_onerule_group_range_min()
{
    return test_onerule_noerr("2*4( \"abc\" )", "abcabc");
}

//@TEST
_Bool retrieve_onerule_group_range_middle()
{
    return test_onerule_noerr("2*4( \"abc\" )", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_range_max()
{
    return test_onerule_noerr("2*4( \"abc\" )", "abcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_range_nested_empty()
{
    // *8( "abc" )
    return test_onerule_noerr("1*2( 1*2( *2( \"abc\" )))", "");
}

//@TEST
_Bool retrieve_onerule_group_range_nested_one()
{
    // *8( "abc" )
    return test_onerule_noerr("1*2( 1*2( *2( \"abc\" )))", "abc");
}

//@TEST
_Bool retrieve_onerule_group_range_nested_some()
{
    // *8( "abc" )
    return test_onerule_noerr("1*2( 1*2( *2( \"abc\" )))", "abcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_range_nested_max()
{
    // *8( "abc" )
    return test_onerule_noerr(
            "1*2( 1*2( *2( \"abc\" )))", "abcabcabcabcabcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_minrange_min()
{
    return test_onerule_noerr("2*( \"abc\" )", "abcabc");
}

//@TEST
_Bool retrieve_onerule_group_minrange_some()
{
    return test_onerule_noerr("2*( \"abc\" )", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_minrange_lots()
{
    return test_onerule_noerr("2*( \"abc\" )", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_maxrange_empty()
{
    return test_onerule_noerr("*4( \"abc\" )", "");
}

//@TEST
_Bool retrieve_onerule_group_maxrange_one()
{
    return test_onerule_noerr("*4( \"abc\" )", "abc");
}

//@TEST
_Bool retrieve_onerule_group_maxrange_two()
{
    return test_onerule_noerr("*4( \"abc\" )", "abcabc");
}

//@TEST
_Bool retrieve_onerule_group_maxrange_three()
{
    return test_onerule_noerr("*4( \"abc\" )", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_maxrange_max()
{
    return test_onerule_noerr("*4( \"abc\" )", "abcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_group_nested()
{
    return test_onerule_noerr("((((( \"abc\" )))))", "abc");
}

//@TEST
_Bool retrieve_onerule_option_empty()
{
    return test_onerule_noerr("[ \"abc\" ]", "");
}

//@TEST
_Bool retrieve_onerule_option_nonempty()
{
    return test_onerule_noerr("[ \"abc\" ]", "abc");
}

//@TEST
_Bool retrieve_onerule_option_nested_empty()
{

    return test_onerule_noerr("[[[[[[ \"abc\" ]]]]]]", "");
}

//@TEST
_Bool retrieve_onerule_option_nested_nonempty()
{
    return test_onerule_noerr("[[[[[[ \"abc\" ]]]]]]", "abc");
}

//@TEST
_Bool retrieve_onerule_option_range_zero()
{
    // 0*4"abc"
    return test_onerule_noerr("2*4[ \"abc\" ]", "");
}

//@TEST
_Bool retrieve_onerule_option_range_one()
{
    // 0*4"abc"
    return test_onerule_noerr("2*4[ \"abc\" ]", "abc");
}

//@TEST
_Bool retrieve_onerule_option_range_two()
{
    // 0*4"abc"
    return test_onerule_noerr("2*4[ \"abc\" ]", "abcabc");
}

//@TEST
_Bool retrieve_onerule_option_range_three()
{
    // 0*4"abc"
    return test_onerule_noerr("2*4[ \"abc\" ]", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_range_max()
{
    // 0*4"abc"
    return test_onerule_noerr("2*4[ \"abc\" ]", "abcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_range_nested_empty()
{
    // *8"abc"
    return test_onerule_noerr("1*2[ 1*2[ 1*2[ \"abc\" ]]]", "");
}

//@TEST
_Bool retrieve_onerule_option_range_nested_one()
{
    // *8"abc"
    return test_onerule_noerr("1*2[ 1*2[ 1*2[ \"abc\" ]]]", "abc");
}

//@TEST
_Bool retrieve_onerule_option_range_nested_some()
{
    // *8"abc"
    return test_onerule_noerr("1*2[ 1*2[ 1*2[ \"abc\" ]]]", "abcabc");
}

//@TEST
_Bool retrieve_onerule_option_range_nested_max()
{
    // *8"abc"
    return test_onerule_noerr(
            "1*2[ 1*2[ 1*2[ \"abc\" ]]]", "abcabcabcabcabcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_minrange_min()
{
    return test_onerule_noerr("2*[ \"abc\" ]", "abcabc");
}

//@TEST
_Bool retrieve_onerule_option_minrange_some()
{
    return test_onerule_noerr("2*[ \"abc\" ]", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_minrange_lots()
{
    return test_onerule_noerr("2*[ \"abc\" ]", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_maxrange_zero()
{
    return test_onerule_noerr("*4[ \"abc\" ]", "");
}

//@TEST
_Bool retrieve_onerule_option_maxrange_one()
{
    return test_onerule_noerr("*4[ \"abc\" ]", "abc");
}

//@TEST
_Bool retrieve_onerule_option_maxrange_two()
{
    return test_onerule_noerr("*4[ \"abc\" ]", "abcabc");
}

//@TEST
_Bool retrieve_onerule_option_maxrange_three()
{
    return test_onerule_noerr("*4[ \"abc\" ]", "abcabcabc");
}

//@TEST
_Bool retrieve_onerule_option_maxrange_max()
{
    return test_onerule_noerr("*4[ \"abc\" ]", "abcabcabcabc");
}

//@TEST
_Bool retrieve_onerule_concat_char()
{
    return test_onerule_noerr("\"a\" \"b\"", "ab");
}

//@TEST
_Bool retrieve_onerule_concat_char_multiple()
{
    return test_onerule_noerr("\"a\" \"b\" \"c\" \"d\"", "abcd");
}

//@TEST
_Bool retrieve_onerule_concat_char_repeat_first()
{
    return test_onerule_noerr("3\"a\" \"b\" \"c\"", "aaabc");
}

//@TEST
_Bool retrieve_onerule_concat_char_repeat_middle()
{
    return test_onerule_noerr("\"a\" 3\"b\" \"c\"", "abbbc");
}

//@TEST
_Bool retrieve_onerule_concat_char_repeat_last()
{
    return test_onerule_noerr("\"a\" \"b\" 3\"c\"", "abccc");
}

//@TEST
_Bool retrieve_onerule_concat_char_repeat_all()
{
    return test_onerule_noerr("2\"a\" 3\"b\" 4\"c\"", "aabbbcccc");
}

//@TEST
_Bool retrieve_onerule_alt_first()
{
    return test_onerule_noerr("\"a\" / \"b\" / \"c\"", "a");
}

//@TEST
_Bool retrieve_onerule_alt_middle()
{
    return test_onerule_noerr("\"a\" / \"b\" / \"c\"", "b");
}

//@TEST
_Bool retrieve_onerule_alt_last()
{
    return test_onerule_noerr("\"a\" / \"b\" / \"c\"", "c");
}

//@TEST
_Bool retrieve_onerule_alt_cat_first()
{
    return test_onerule_noerr("\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\"", "a1");
}

//@TEST
_Bool retrieve_onerule_alt_cat_middle()
{
    return test_onerule_noerr("\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\"", "b2");
}

//@TEST
_Bool retrieve_onerule_alt_cat_last()
{
    return test_onerule_noerr("\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\"", "c3");
}

//@TEST
_Bool retrieve_onerule_cat_alt_variation1()
{
    return test_onerule_noerr("\"a\" ( \"1\" / \"2\" ) \"b\"", "a1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_variation2()
{
    return test_onerule_noerr("\"a\" ( \"1\" / \"2\" ) \"b\"", "a2b");
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat_empty()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            ""
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "a1a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat2()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat3()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "c3c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat12()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "a1b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat21()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat31()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "c3a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat23()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat32()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "c3b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat123()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "a1b2c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat132()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "a1c3b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat213()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2a1c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat231()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2c3a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat312()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "c3a1b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat321()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "c3b2a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_outer_repeat_mixed()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\" / \"b\" \"2\" / \"c\" \"3\")",
            "b2a1a1c3a1b2b2b2c3c3a1b2c3a1c3a1c3b2b2a1b2b2a1b2c3c3c3c3c3c3c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_empty()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            ""
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_first_one()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_first_two()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "a1a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_first_many()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_middle_one()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_middle_two()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "b2b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_middle_many()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_last_one()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_last_two()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "c3c3"
    );
}

//@TEST
_Bool retrieve_onerule_alt_cat_inner_repeat_last_many()
{
    return test_onerule_noerr(
            "*(\"a\" \"1\") / *(\"b\" \"2\") / *(\"c\" \"3\")",
            "c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_empty()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation1_one()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation1_two()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1ba1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation1_many()
{
    return test_onerule_noerr(
            "*(\"a\" (\"1\" / \"2\") \"b\")",
            "a1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1ba1b"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation2_one()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation2_two()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2ba2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_variation2_many()
{
    return test_onerule_noerr(
            "*(\"a\" (\"1\" / \"2\") \"b\")",
            "a2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2ba2b"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed12()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1ba2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed21()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2ba1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed112()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1ba1ba2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed121()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1ba2ba1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed122()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a1ba2ba2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed211()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2ba1ba1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed212()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2ba1ba2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed221()
{
    return test_onerule_noerr("*(\"a\" (\"1\" / \"2\") \"b\")", "a2ba2ba1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_outer_repeat_mixed()
{
    return test_onerule_noerr(
            "*(\"a\" (\"1\" / \"2\") \"b\")",
            "a1ba2ba2ba1ba1ba2ba2ba2ba1ba2ba1ba2ba1ba1ba2ba2ba2ba1ba1ba1ba1b"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_empty()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_first_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "a");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_first_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aa");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_first_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation1_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "1");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation1_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "11");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation1_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "11111111111111111111111111111111111111"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation2_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "2");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation2_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "22");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_middle_variation2_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "22222222222222222222222222222222222222"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_last_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_last_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "bb");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_last_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_1_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "a1");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_1_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aa11");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_1_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaa1111111111"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_2_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "a2");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_2_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aa22");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_12_2_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaa2222222222"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_13_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "ab");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_13_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aabb");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_13_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaabbbbbbbbbb"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "a1b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aa11bb");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaa1111111111bbbbbbbbbb"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_one()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "a2b");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_two()
{
    return test_onerule_noerr("*\"a\" *(\"1\" / \"2\") *\"b\"", "aa22bb");
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_many()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaaa2222222222bbbbbbbbbb"
    );
}

//@TEST
_Bool retrieve_onerule_cat_alt_inner_repeat_mixed()
{
    return test_onerule_noerr(
            "*\"a\" *(\"1\" / \"2\") *\"b\"",
            "aaaaaaaaa12111222112211122111222222221111121221bbbbb"
    );
}

//@TEST
_Bool retrieve_onerule_nested_string()
{
    return test_onerule_nested_noerr("\"abc\"", "abc");
}

_Bool retrieve_rule_dne_name_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    char* s;
    return abnf_retrieve(result, "flippy-floppy", 0, &s) == ABNF_DNE;
}

//@TEST
_Bool retrieve_rule_dne_name()
{
    return perform_test(NULL, retrieve_rule_dne_name_helper);
}

_Bool retrieve_rule_dne_index_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    if (
            abnf_define(grammar, "rule=\"a\"") != ABNF_NOERR
            || abnf_parse(grammar, "rule", "a", result) != ABNF_NOERR
    ) {
        return false;
    }

    char* s;
    return abnf_retrieve(result, "rule", 1, &s) == ABNF_DNE;
}

//@TEST
_Bool retrieve_rule_dne_index()
{
    return perform_test(NULL, retrieve_rule_dne_index_helper);
}

_Bool retrieve_out_null_helper(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    if (
            abnf_define(grammar, "rule=\"a\"") != ABNF_NOERR
            || abnf_parse(grammar, "rule", "a", result) != ABNF_NOERR
    ) {
        return false;
    }

    return abnf_retrieve(result, "rule", 1, NULL) == ABNF_NULL;
}

//@TEST
_Bool retrieve_out_null()
{
    return perform_test(NULL, retrieve_out_null_helper);
}

_Bool retrieve_result_null(
        struct abnf* grammar,
        struct abnf_result* result,
        void* extra
) {
    if (
            abnf_define(grammar, "rule=\"a\"") != ABNF_NOERR
            || abnf_parse(grammar, "rule", "a", result) != ABNF_NOERR
    ) {
        return false;
    }

    char* s;
    return abnf_retrieve(NULL, "rule", 1, &s) == ABNF_NULL;
}

//@TEST
_Bool retrieve_onerule_repeated_empty()
{
    return test_onerule_repeated_dne("\"a\"", "", 0);   
}

//@TEST
_Bool retrieve_onerule_repeated_char()
{
    _Bool success = true;

    for (int i = 0; i < 3; i++) {
        success = test_onerule_repeated_noerr("\"a\"", "aaa", i, "a");
        if (!success) {
            break;
        }
    }

    return success;
}

//@TEST
_Bool retrieve_onerule_repeated_char_range_i0()
{
    return test_onerule_repeated_noerr("%x61-63", "abc", 0, "a");
}

//@TEST
_Bool retrieve_onerule_repeated_char_range_i1()
{
    return test_onerule_repeated_noerr("%x61-63", "abc", 1, "b");
}

//@TEST
_Bool retrieve_onerule_repeated_char_range_i2()
{
    return test_onerule_repeated_noerr("%x61-63", "abc", 2, "c");
}

//@TEST
_Bool retrieve_rule_repeated_zero_empty_dne()
{
    return test_rules(
            "0rule",
            "",
            "rule",
            0,
            "",
            ABNF_DNE,
            "rule=\"abc\"",
            ""
    );
}
            
//@TEST
_Bool retrieve_rule_concat_first()
{
    return test_rules(
            "rule1 rule2 rule3",
            "abc", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_concat_middle()
{
    return test_rules(
            "rule1 rule2 rule3",
            "abc", 
            "rule2",
            0,
            "b",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_concat_last()
{
    return test_rules(
            "rule1 rule2 rule3",
            "abc", 
            "rule3",
            0,
            "c",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_concat_dne_name()
{
    return test_rules(
            "rule1 rule2 rule3",
            "abc", 
            "rule4",
            0,
            "",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_concat_dne_index()
{
    return test_rules(
            "rule1 rule2 rule3",
            "abc", 
            "rule1",
            1,
            "a",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation1()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "a", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation1_dne2()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "a", 
            "rule2",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation1_dne3()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "a", 
            "rule3",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation2()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "b", 
            "rule2",
            0,
            "b",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation2_dne1()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "b", 
            "rule1",
            0,
            "b",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation2_dne3()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "b", 
            "rule3",
            0,
            "b",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation3()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "c", 
            "rule3",
            0,
            "c",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation3_dne1()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "c", 
            "rule1",
            0,
            "c",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_varation3_dne2()
{
    return test_rules(
            "rule1 / rule2 / rule3",
            "c", 
            "rule2",
            0,
            "c",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"b\"",
            "rule3=\"c\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation1_first()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "a1", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation1_middle()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "a1", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation2_first()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "b2", 
            "rule3",
            0,
            "b",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation2_middle()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "b2", 
            "rule4",
            0,
            "2",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation3_first()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "c3", 
            "rule5",
            0,
            "c",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_varation3_middle()
{
    return test_rules(
            "rule1 rule2 / rule3 rule4 / rule5 rule6",
            "c3", 
            "rule6",
            0,
            "3",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"b\"",
            "rule4=\"2\"",
            "rule5=\"c\"",
            "rule6=\"3\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_first_variation1()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a1b", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_first_variation2()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a2b", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_alt_variation1()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a1b", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_alt_variation1_dne2()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a1b", 
            "rule3",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_alt_variation2_dne1()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a2b", 
            "rule2",
            0,
            "2",
            ABNF_DNE,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_alt_variation2()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a2b", 
            "rule3",
            0,
            "2",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_middle_variation1()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a1b", 
            "rule4",
            0,
            "b",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_middle_variation2()
{
    return test_rules(
            "rule1 ( rule2 / rule3 ) rule4",
            "a2b", 
            "rule4",
            0,
            "b",
            ABNF_NOERR,
            "rule1=\"a\"",
            "rule2=\"1\"",
            "rule3=\"2\"",
            "rule4=\"b\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_first_i0()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_first_i1()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_first_i2()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_middle_i0()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_middle_i1()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_middle_i2()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_last_i0()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_last_i1()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_last_i2()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_first_dne()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_middle_dne()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_outer_last_dne()
{
    return test_rules(
            "*(rule1 rule2 rule3)",
            "a1xb2yc3z", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_empty_dne1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "", 
            "rule1",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_empty_dne2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "", 
            "rule2",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_empty_dne3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "", 
            "rule3",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_dne_middle()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule2",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_first_dne_last()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc", 
            "rule3",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_dne_first()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule1",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_middle_dne_last()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "123", 
            "rule3",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_dne_first()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule1",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_third_dne_middle()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "xyz", 
            "rule2",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_first_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_first_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_first_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_first_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_middle_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_middle_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_middle_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_middle_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_last_i0()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_last_i1()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_last_i2()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_cat_repeated_inner_all_last_dne_i3()
{
    return test_rules(
            "*rule1 *rule2 *rule3",
            "abc123xyz", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_empty_dne1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "", 
            "rule1",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_empty_dne2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "", 
            "rule2",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_empty_dne3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "", 
            "rule3",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_dne_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_dne_var2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule2",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation1_dne_var3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "abc", 
            "rule3",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_dne_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_dne_var1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule1",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation2_dne_var3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "123", 
            "rule3",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_dne_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_dne_var1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule1",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_variation3_dne_var2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "xyz", 
            "rule2",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation1_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation1_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation1_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation1_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation2_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation2_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation2_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation3_i0()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation3_i1()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_variation3_i2()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_dne_variation1_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_dne_variation2_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_alt_repeated_outer_all_dne_variation3_i3()
{
    return test_rules(
            "*(rule1 / rule2 / rule3)",
            "a1xb2yc3z", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule2",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule3",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne4()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule4",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne5()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule5",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation1_dne6()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1b2c3", 
            "rule6",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule4",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule4",
            1,
            "-",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule4",
            2,
            "*",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule4",
            3,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule1",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule2",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne5()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule5",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation2_dne6()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "x+y-z*", 
            "rule6",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule5",
            0,
            ".",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule5",
            1,
            "!",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule5",
            2,
            "?",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule6",
            0,
            "@",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule6",
            1,
            "#",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule6",
            2,
            "$",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule5",
            3,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule6",
            3,
            "@",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule1",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule2",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule3",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_variation3_dne4()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            ".@!#?$", 
            "rule4",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation1_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule4",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule4",
            1,
            "-",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation2_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule4",
            2,
            "*",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_first_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule5",
            0,
            ".",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_first_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule5",
            1,
            "!",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_first_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule5",
            2,
            "?",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_second_i0()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule6",
            0,
            "@",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_second_i1()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule6",
            1,
            "#",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_variation3_second_i2()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule6",
            2,
            "$",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation1_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation1_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation2_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation2_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule4",
            3,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation3_first_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule5",
            3,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_outer_all_dne_variation3_second_i3()
{
    return test_rules(
            "*(rule1 rule2 / rule3 rule4 / rule5 rule6)",
            "a1x+.@b2y-!#c3z*?$", 
            "rule6",
            3,
            "$",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_first_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_first_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_first_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule1",
            2,
            "c",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_second_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_second_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_second_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule2",
            2,
            "3",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );

}
//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule1",
            3,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule2",
            3,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule3",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne4()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule4",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne5()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule5",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation1_dne6()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "a1b2c3", 
            "rule6",
            0,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_first_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule3",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_first_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule3",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_first_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule3",
            2,
            "z",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_second_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule4",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_second_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule4",
            1,
            "-",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_second_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule4",
            2,
            "*",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule3",
            3,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule4",
            3,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule1",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule2",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne5()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule5",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation2_dne6()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            "x+y-z*", 
            "rule6",
            0,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_first_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule5",
            0,
            ".",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_first_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule5",
            1,
            "!",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_first_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule5",
            2,
            "?",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_second_i0()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule6",
            0,
            "@",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_second_i1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule6",
            1,
            "#",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_second_i2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule6",
            2,
            "$",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne_first_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule5",
            3,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne_second_i3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule6",
            3,
            "@",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne1()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule1",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne2()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule2",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne3()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule3",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_altcat_repeated_inner_variation3_dne4()
{
    return test_rules(
            "*(rule1 rule2) / *(rule3 rule4) / *(rule5 rule6)",
            ".@!#?$", 
            "rule4",
            0,
            ".",
            ABNF_DNE,
            "rule1=\"a\" / \"b\" / \"c\"",
            "rule2=\"1\" / \"2\" / \"3\"",
            "rule3=\"x\" / \"y\" / \"z\"",
            "rule4=\"+\" / \"-\" / \"*\"",
            "rule5=\".\" / \"!\" / \"?\"",
            "rule6=\"@\" / \"#\" / \"$\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_first_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_first_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_first_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule1",
            2,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_second_variation1_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_second_variation1_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_second_variation1_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule2",
            2,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_second_variation2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule3",
            0,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_last_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_last_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v1_last_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb2y", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_first_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_first_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_second_variation1_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule2",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_second_variation2_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule3",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_second_variation2_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule3",
            1,
            "-",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_second_variation2_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule3",
            2,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_last_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_last_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_v2_last_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a+xb-y", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_first_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_first_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_first_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule1",
            2,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i1_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule2",
            1,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule3",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i1_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule3",
            1,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_last_i0()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_last_i1()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_outer_mixed_last_i2_dne()
{
    return test_rules(
            "*(rule1 (rule2 / rule3) rule4)",
            "a1xb+y", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_first_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_first_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_first_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule1",
            2,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_second_variation1_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_second_variation1_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule2",
            1,
            "2",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_second_variation1_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule2",
            2,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_second_variation2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule3",
            0,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_last_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_last_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v1_last_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab12xy", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_first_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_first_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_first_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule1",
            2,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_second_variation1_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule2",
            0,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_second_variation2_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule3",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_second_variation2_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule3",
            1,
            "-",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_second_variation2_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule3",
            2,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_last_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_last_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_v2_last_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab+-xy", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_first_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule1",
            0,
            "a",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_first_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule1",
            1,
            "b",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_first_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule1",
            2,
            "a",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule2",
            0,
            "1",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i1_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule2",
            1,
            "1",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule3",
            0,
            "+",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i1_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule3",
            1,
            "+",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_last_i0()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule4",
            0,
            "x",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_last_i1()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule4",
            1,
            "y",
            ABNF_NOERR,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_rule_catalt_repeated_inner_mixed_last_i2_dne()
{
    return test_rules(
            "*rule1 *(rule2 / rule3) *rule4",
            "ab1+xy", 
            "rule4",
            2,
            "x",
            ABNF_DNE,
            "rule1=\"a\" / \"b\"",
            "rule2=\"1\" / \"2\"",
            "rule3=\"+\" / \"-\"",
            "rule4=\"x\" / \"y\"",
            ""
    );
}

//@TEST
_Bool retrieve_abnf_hexval_i0()
{
    return test_abnfgrammar_noerr(
            "hex-val",
            "x1A",
            "HEXDIG",
            0,
            "1"
    );
}

//@TEST
_Bool retrieve_abnf_hexval_i1()
{
    return test_abnfgrammar_noerr(
            "hex-val",
            "x1A",
            "HEXDIG",
            1,
            "A"
    );
}

//@TEST
_Bool retrieve_abnf_numval()
{
    return test_abnfgrammar_noerr(
            "num-val",
            "%x123ABC",
            "hex-val",
            0,
            "x123ABC"
    );
}

//@TEST
_Bool retrieve_abnf_numval_dec_dne()
{
    return test_abnfgrammar_dne(
            "num-val",
            "%x123ABC",
            "dec-val",
            0,
            "x123ABC"
    );
}

//@TEST
_Bool retrieve_abnf_numval_bin_dne()
{
    return test_abnfgrammar_dne(
            "num-val",
            "%x123ABC",
            "bin-val",
            0,
            "x123ABC"
    );
}

//@TEST
_Bool retrieve_abnf_charval_i0()
{
    return test_abnfgrammar_noerr(
            "char-val",
            "\"abc\"",
            "DQUOTE",
            0,
            "\""
    );
}

//@TEST
_Bool retrieve_abnf_charval_i1()
{
    return test_abnfgrammar_noerr(
            "char-val",
            "\"abc\"",
            "DQUOTE",
            1,
            "\""
    );
}

//@TEST
_Bool retrieve_abnf_option()
{
    return test_abnfgrammar_noerr(
            "option",
            "[ *c-wsp alternation *c-wsp ]",
            "alternation",
            0,
            "*c-wsp alternation *c-wsp"
    );
}

//@TEST
_Bool retrieve_abnf_repeat_i0()
{
    return test_abnfgrammar_noerr(
            "repeat",
            "12*34",
            "DIGIT",
            0,
            "1"
    );
}

//@TEST
_Bool retrieve_abnf_repeat_i1()
{
    return test_abnfgrammar_noerr(
            "repeat",
            "12*34",
            "DIGIT",
            1,
            "2"
    );
}

//@TEST
_Bool retrieve_abnf_repeat_i2()
{
    return test_abnfgrammar_noerr(
            "repeat",
            "12*34",
            "DIGIT",
            2,
            "3"
    );
}

//@TEST
_Bool retrieve_abnf_repeat_i3()
{
    return test_abnfgrammar_noerr(
            "repeat",
            "12*34",
            "DIGIT",
            3,
            "4"
    );
}

//@TEST
_Bool retrieve_abnf_repeat_i4_dne()
{
    return test_abnfgrammar_dne(
            "repeat",
            "12*34",
            "DIGIT",
            4,
            "1"
    );
}

//@TEST
_Bool retrieve_abnf_repetition_repeat()
{
    return test_abnfgrammar_noerr(
            "repetition",
            "1*( rule / (*c-wsp c-nl) )",
            "repeat",
            0,
            "1*"
    );
}

//@TEST
_Bool retrieve_abnf_repetition_element()
{
    return test_abnfgrammar_noerr(
            "repetition",
            "1*( rule / (*c-wsp c-nl) )",
            "element",
            0,
            "( rule / (*c-wsp c-nl) )"
    );
}

//@TEST
_Bool retrieve_abnf_concatenation_i0()
{
    return test_abnfgrammar_noerr(
            "concatenation",
            "\"(\" *c-wsp alternation *c-wsp \")\"",
            "repetition",
            0,
            "\"(\""
    );
}

//@TEST
_Bool retrieve_abnf_concatenation_i1()
{
    return test_abnfgrammar_noerr(
            "concatenation",
            "\"(\" *c-wsp alternation *c-wsp \")\"",
            "repetition",
            1,
            "*c-wsp"
    );
}

//@TEST
_Bool retrieve_abnf_concatenation_i2()
{
    return test_abnfgrammar_noerr(
            "concatenation",
            "\"(\" *c-wsp alternation *c-wsp \")\"",
            "repetition",
            2,
            "alternation"
    );
}

//@TEST
_Bool retrieve_abnf_concatenation_i3()
{
    return test_abnfgrammar_noerr(
            "concatenation",
            "\"(\" *c-wsp alternation *c-wsp \")\"",
            "repetition",
            3,
            "*c-wsp"
    );
}

//@TEST
_Bool retrieve_abnf_concatenation_i4()
{
    return test_abnfgrammar_noerr(
            "concatenation",
            "\"(\" *c-wsp alternation *c-wsp \")\"",
            "repetition",
            4,
            "\")\""
    );
}

//@TEST
_Bool retrieve_abnf_alternation_i0()
{
    return test_abnfgrammar_noerr(
            "alternation",
            "1*DIGIT / (*DIGIT \"*\" *DIGIT)",
            "concatenation",
            0,
            "1*DIGIT"
    );
}

//@TEST
_Bool retrieve_abnf_alternation_i1()
{
    return test_abnfgrammar_noerr(
            "alternation",
            "1*DIGIT / (*DIGIT \"*\" *DIGIT)",
            "concatenation",
            1,
            "(*DIGIT \"*\" *DIGIT)"
    );
}

//@TEST
_Bool retrieve_abnf_rule_rulename()
{
        return test_abnfgrammar_noerr(
                "rule",

                "rule\t =  rulename defined-as elements c-nl\r\n"
                "\t\t ; continues if next line starts\r\n"
                "\t\t ;  with whitespace\r\n",

                "rulename",
                0,
                "rule"
        );
}

//@TEST
_Bool retrieve_abnf_rule_definedas()
{
        return test_abnfgrammar_noerr(
                "rule",

                "rule\t =  rulename defined-as elements c-nl\r\n"
                "\t\t ; continues if next line starts\r\n"
                "\t\t ;  with whitespace\r\n",

                "defined-as",
                0,
                "\t =  "
        );
}

//@TEST
_Bool retrieve_abnf_rule_elements()
{
        return test_abnfgrammar_noerr(
                "rule",

                "rule\t =  rulename defined-as elements c-nl\r\n"
                "\t\t ; continues if next line starts\r\n"
                "\t\t ;  with whitespace\r\n",

                "elements",
                0,

                "rulename defined-as elements c-nl\r\n"
                "\t\t ; continues if next line starts\r\n"
                "\t\t "
        );
}

//@TEST
_Bool retrieve_abnf_rule_cnl()
{
        return test_abnfgrammar_noerr(
                "rule",

                "rule\t =  rulename defined-as elements c-nl\r\n"
                "\t\t ; continues if next line starts\r\n"
                "\t\t ;  with whitespace\r\n",

                "c-nl",
                0,
                ";  with whitespace\r\n"
        );
}
