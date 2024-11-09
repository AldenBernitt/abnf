#define REP_UNDEF -1

enum {
    DEFINE_NEW,
    DEFINE_INC,
    DEFINE_ERR
};

#include "abnf_terminals.c"
#include "abnf_define_helper.c"

static int search_name(struct chstream* in)
{
    /*
        (RFC 5234 section 2.1)
        The name of a rule is simply the name itself, that is, a sequence of
        characters, beginning with an alphabetic character, and followed by a
        combination of alphabetics, digits, and hyphens (dashes).
    */

    // rulename = ALPHA *(ALPHA / DIGIT / "-")

    size_t i = 0;

    if (is_alpha(chstream_peek(in, i))) {
        i++;

        char c = chstream_peek(in, i);
        for(
                char c = chstream_peek(in, i); 
                is_alpha(c)
                || is_digit(c)
                || c == '-';
                c = chstream_peek(in, i)
        ) {
            i++;
        }
    }

    return i;
}

static void copy_from_stream(char* dest, struct chstream* in, int len)
{
    for (size_t i = 0; i < len; i++) {
        dest[i] = chstream_peek(in, i);
    }
    dest[len] = '\0';
}

static int define_name(struct chstream* in, char** out)
{
    size_t len = search_name(in);
    
    if (len > 0) {
        *out = (char*)malloc(sizeof(char) * len);
        if (*out) {
            copy_from_stream(*out, in, len);
            chstream_advance(in, len);
            return ABNF_NOERR;
        }
        else {
            return ABNF_MEMERR;
        }
    }
    else {
        return ABNF_PARSEERR;
    }
}

static int define_definedas(struct chstream* in)
{
    // defined-as = *c-wsp ("=" / "=/") *c-wsp

    int code;
    
    while (parse_cwsp(in))
        ; // EMPTY WHILE

    if (parse_str(in, "=/")) {
        code = DEFINE_INC;
    }
    else if (parse_ch(in, '=')) {
        code = DEFINE_NEW;
    }
    else {
        return DEFINE_ERR;
    }

    while (parse_cwsp(in))
        ; // EMPTY WHILE

    return code;
}

static _Bool append_rule(struct abnf* grammar, struct abnf_rule* rule)
{
    size_t newcount = grammar->count + 1;

    void *p = realloc(grammar->rules, sizeof(struct abnf_rule) * newcount);

    if (p) {
        grammar->rules = (struct abnf_rule*)p;
        grammar->rules[grammar->count++] = *rule;
        return true;
    }
    else {
        return false;
    }
}

#include "optimize/abnf_optimize.c"
#include "abnf_normalize.c"

static int add_new_rule(struct abnf* grammar, struct abnf_rule* rule)
{
    // make sure the rule name doesn't already exist.
    for (size_t i = 0; i < grammar->count; i++) {
        if (strcmp(grammar->rules[i].name, rule->name) == 0) {
            // the name already exists.
            return ABNF_DEFERR;
        }
    }

    if (!abnf_optimize(rule) || !abnf_normalize(rule)) {
        return ABNF_OPTERR;
    }

    return append_rule(grammar, rule) ? ABNF_NOERR : ABNF_MEMERR;
}

static int increment_rule(struct abnf* grammar, struct abnf_rule* rule)
{
    struct abnf_rule* existing = get_rule(grammar, rule->name);

    if (existing == NULL) {
        return add_new_rule(grammar, rule);
    }

    /*
        add all the concatenations in this rule's alternation to the
        existing rule's alternation.
    */
    
    // allocate memory for the new concatenations.
    size_t newcount = existing->alt.count + rule->alt.count;
    void* p = realloc(existing->alt.cats, sizeof(struct abnf_cat) * newcount);

    if (p == NULL) {
        return ABNF_MEMERR;
    }

    existing->alt.cats = (struct abnf_cat*)p;

    // append a copy of the new rule's concatenation list to the existing
    // rule's concatenation list.
    for (size_t i = 0; i < rule->alt.count; i++) {
        existing->alt.cats[existing->alt.count + i] = rule->alt.cats[i];
    }
    existing->alt.count = newcount;

    free(rule->name);
    rule->name = NULL;

    free(rule->alt.cats);
    rule->alt.cats = NULL;
    rule->alt.count = 0;

    if (!abnf_optimize(existing) || !abnf_normalize(existing)) {
        return ABNF_OPTERR;
    }

    return ABNF_NOERR;
}

static int add_rule(struct abnf* grammar, struct abnf_rule* rule, int deftype)
{
    switch (deftype) {
        case DEFINE_NEW:
            return add_new_rule(grammar, rule);

        case DEFINE_INC:
            return increment_rule(grammar, rule);

        default:
            return ABNF_PARSEERR;
    }
}

#include "abnf_define_elements.c"

static int define_rule(
        struct abnf* grammar,
        struct abnf_rule* rule,
        struct chstream* in,
        int* deftype
) {
    // rule = rulename defined-as elements c-nl

    int errcode;

    // unassigned boolean expression used for short-circuiting.
    (errcode = define_name(in, &rule->name)) == ABNF_NOERR
        && (*deftype = define_definedas(in)) != DEFINE_ERR
        && (errcode = define_elements(in, &rule->alt)) == ABNF_NOERR;

    // optionally parse c-nl
    parse_cnl(in);

    // report an error if the are characters left in the input.
    if (errcode == ABNF_NOERR && !chstream_eos(in)) {
        errcode = ABNF_PARSEERR;
    }

    return errcode;
}

int abnf_define(struct abnf* grammar, const char* rulestr)
{
    if (!grammar) {
        return ABNF_NULL;
    }

    int errcode;
    int deftype;
    struct chstream in;
    struct abnf_rule rule;
    abnf_initrule(&rule);

    if (chstream_init(&in, rulestr) != CHSTREAM_NOERR) {
        return ABNF_MEMERR;
    }

    // attempt to parse a rule and then add it to the grammar.
    // free allocated memory if an error occurred.
    if (
            (errcode = define_rule(grammar, &rule, &in, &deftype)) 
                != ABNF_NOERR
            || (errcode = add_rule(grammar, &rule, deftype)) != ABNF_NOERR
    ) {
        abnf_freerule(&rule);
    }

    chstream_free(&in);
    return errcode;
}
