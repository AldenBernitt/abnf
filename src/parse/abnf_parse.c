/*
    Author: Alden Bernitt 2024/10/24
    
    SUMMARY:

    this file provides the definitions of abnf_parse and its helper functions.
    the abnf_parse function parses an input string according to its passed
    rule name and stores the resulting data in an abnf_result structure. the
    rule name is expected to be defined in the abnf structure that is also
    passed to abnf_parse function.

    *NOTE* before calling abnf_parse, the passed-in rule name, and all names
    directly or indirectly referenced by the rule must be defined in the
    passed-in abnf structure.

    the stored results are determined by the rule names found in the passed
    rule. for example, suppose the abnf structure contains rules named "sum",
    "number", and "digit" defined as

        sum = number "+" number
        number = 1*digit
        digit = %x30-39 ; the characters '0' through '9'

    and suppose that abnf_parse is given the rule name "sum" and the text
    input "123+456". then the resulting abnf_result data will contain entries
    used to access "123" using the name "number" and index 0, and "456" using
    the name "number" and index 1. in particular, note that no data
    referencing the "+" is stored, there are no entries for the "digit" rule.
    that is, only data associated with a name in the top level rule is stored.

    OPERATION OVERVIEW:

    --components--
    abnf                --> stores rules (identified by rule names)
    abnf_rule           --> stores data defining the text sequences allowed by
                            the rule
    abnf_result         --> stores data which associates names with their
                            corresponding text as parsed by the abnf_parse
                            function
    "top-level rule"    --> the rule associated with the rule name originally
                            passed to the abnf_parse function.

    **  for more information about "rules" and other information relating to
        ABNF grammars, see RFC 5234

    the abnf_parse function works by making a chain of calls to helper
    functions to parse individual elements (stored in the abnf_rule data
    structure) from the input text. these functions attempt to match the text
    in the input to the text permitted by the given element.

    for example, if the current element permits any of the characters 'a'
    through 'z', then the input text 'w' would be parsed successfully, but the
    input text '5' would fail.

    through this chain of calls, text associated with a name in the top-level
    rule are accumulated and stored in the passed abnf_result structure.
    non-name text is also parsed, but not stored.

    this chain of calls very closely resembles the definition of an ABNF
    grammar as described in RFC 5234 (section 4).


    for example, given the example above, the call chain would roughly resemble
    the following (emphasis on "roughly"):

    (FUNCTION)      --> (INPUT STATE)
    ---------------------------------
    abnf_parse      --> "123+456"
    parse_alt       --> "123+456"
    parse_cat       --> "123+456"

    (ITERATION 1)
    parse_rep       --> "123+456"
    parse_ele       --> "123+456"
    parse_terminal  --> "+456"      // read the <number> "123" and associate
                                    // it with the name "number" at index 0
                                    // in the abnf_result structure.
    (ITERATION 2)
    parse_rep       --> "+456"
    parse_ele       --> "+456"
    parse_terminal  --> "456"       // read the "+", but do store it, since
                                    // it is not associated with a name in
                                    // the top-level rule ("sum").
    (ITERATION 3)
    parse_rep       --> "456"
    parse_ele       --> "456"
    parse_terminal  --> ""          // read the <number> "123" and associate
                                    // it with the name "number" at index 0
                                    // in the abnf_result structure.

    (FINISH)

    ERRORS:

    the abnf_parse function returns an integer indicating the result of the
    parse. the return values and their meanings are as follows:

        ABNF_NOERR      --> the input text was successfully parsed and stored

        ABNF_PARSEERR   --> there was an issue parsing the input text. (for
                            example, using the small grammar above, the input
                            string "123-456" would result in this error.)

        ABNF_MEMERR     --> an issue occurred while allocating memory for use
                            by this function or its helper functions

        ABNF_DNE        --> "Does Not Exist": inidicates that information that
                            is vital to processing is missing. for example,
                            attempting to reference (directly or indirectly)
                            a rule name that does not exist in the abnf
                            structure would result in this error.

    *NOTE*  excess characters at the end of the input text after parsing will
            also result in an ABNF_PARSEERR.

    DETAILS:
    
    while a straight-forward one-pass parse would be ideal, the ABNF grammar
    definition permits rules that can be parsed in multiple ways, with only
    some of those ways being correct.

    for example, consider the rule

        ambiguous1 = *a "a#"

    given the input "aaa#":

    --if the text is parsed as "aaa" followed by "a#", then the parse fails.
    however, if the text is parsed as "aa" followed by "a#", then it succeeds.

    similarly, consider the rule

        ambiguous2 = [ "a" ] "a#"

    given the input "a#":

    --if the text is parsed as "a" followed by "a#", then the parse fails.
    however, if the text is parsed as "" followed by "a#", then it succeeds.

    this illustrates a necessity to parse multiple possibilities until one of
    them yields a successful parse (or all possibilities have been exhausted).

    to this end, we define a "parsenode" structure to record parse state so
    that multiple calls to the same parsing function will be able to test
    different parse paths. that is, a parsenode will be passed to the parsing
    functions which will use that data to manipulate how they parse the input
    data. thus, multiple parse paths can be executed by repeatedly calling
    a parsing function using the same parsenode.

    for more information see parsenode.c

    finally, note that each "parse" function is designed to continuously try
    different paths until a successful path starting from the passed node is
    found, at which point the function returns ABNF_NOERR. in particular,
    ABNF_PARSEERR is only returned if every path starting from the passed node
    was unsuccessful.
*/
#include "abnf.h"

#include "parsenode.c"
#include "abnf_write.c"

static int parse_alt(
        struct abnf* grammar,
        struct abnf_alt* alt,
        struct parsenode* node,
        struct chstream* in
);

static void reset_node(struct parsenode* node)
{
    const char* name = node->name;
    int depth = node->depth;
    parsenode_free(node);
    parsenode_init(node, name, depth);
}

static int parse_ele_alt(
        struct abnf* grammar,
        struct abnf_alt* alt,
        struct parsenode* node,
        int depth,
        struct chstream* in
) {
    if (node->len <= 0) {
        if (!parsenode_append(node)) {
            return ABNF_MEMERR;
        }

        node->children[0].inpos = node->inpos;
    }

    struct parsenode* child = &node->children[0];
    child->depth = depth;

    int errcode = parse_alt(grammar, alt, child, in);

    return errcode;
}

static int parse_ele_rule(
        struct abnf* grammar,
        const char* name,
        struct parsenode* node,
        struct chstream* in
) {
    node->type = NODE_ELE_RULE;
    node->name = name;
    
    struct abnf_rule* rule = get_rule(grammar, name);
    if (rule == NULL) {
        return ABNF_DNE;
    }

    return parse_ele_alt(
            grammar, &rule->alt, node, node->depth + 1, in);
}

static int parse_ele_group(
        struct abnf* grammar,
        struct abnf_alt* alt,
        struct parsenode* node,
        struct chstream* in
) {
    node->type = NODE_ELE_GROUP;
    return parse_ele_alt(grammar, alt, node, node->depth, in);
}

static int parse_ele_option(
        struct abnf* grammar,
        struct abnf_alt* alt,
        struct parsenode* node,
        struct chstream* in
) {
    node->type = NODE_ELE_OPTION;
    
    if (node->index >= 2) {
        return ABNF_PARSEERR;
    }
    node->index++;

    int errcode;

    // attempt to parse on the first pass.
    if (node->index == 1) {
        errcode = parse_ele_alt(grammar, alt, node, node->depth, in);

        if (errcode == ABNF_PARSEERR) {
            // try the next pass.
            node->index++;
        }
    }

    // omit on the second pass.
    if (node->index == 2) {
        errcode = ABNF_NOERR;
    }

    return errcode;
}

static int parse_ele_terminal_and(
        struct abnf_term* term,
        struct parsenode* node,
        struct chstream* in
) {
    node->type = NODE_ELE_TERMINAL_AND;

    // match the terminal to the input stream.
    const char* vals = term->values;
    size_t valslen = strlen(vals);

    char* result = calloc(valslen, sizeof(char));
    if (!result) {
        return ABNF_MEMERR;
    }

    size_t count = 0;
    char c = chstream_peek(in, count);
    // case-sensitivity adjusted c
    char sensc = term->sens == TERM_INSENS ? tolower(c) : c;

    while (vals[count] != '\0' && vals[count] == sensc) {
        result[count] = c;
        count++;

        c = chstream_peek(in, count);
        sensc = term->sens == TERM_INSENS ? tolower(c) : c;
    }
    
    // if a match could not be made, report an error.
    if (count != valslen) {
        free(result);
        return ABNF_PARSEERR;
    }

    // advance past the read characters.
    chstream_advance(in, count);

    node->str = result;

    return ABNF_NOERR;
}

static int parse_ele_terminal_or(
        struct abnf_term* term,
        struct parsenode* node,
        struct chstream* in
) {
    node->type = NODE_ELE_TERMINAL_OR;

    const char* vals = term->values;

    char c = chstream_peek(in, 0);

    // case-sensitivity adjusted c
    char sensc = term->sens == TERM_INSENS ? tolower(c) : c;
    int matchi = str_chfind(vals, strlen(vals), sensc);

    if (matchi < 0) {
        return ABNF_PARSEERR;
    }

    chstream_advance(in, 1);

    node->ch = c;

    return ABNF_NOERR;
}

static int parse_ele_terminal(
        struct abnf_term* term,
        struct parsenode* node,
        struct chstream* in
) {
    if (node->index > 0) {
        return ABNF_PARSEERR;
    }
    node->index++;

    switch (term->mode) {
        case TERM_AND:
            return parse_ele_terminal_and(term, node, in);

        case TERM_OR:
            return parse_ele_terminal_or(term, node, in);

        default:
            return ABNF_DNE;
    }
}

static int parse_ele(
        struct abnf* grammar,
        struct abnf_ele* ele,
        struct parsenode* node,
        struct chstream* in
) {
    if (node->len <= 0) {
        node->type = NODE_ELE;

        if (!parsenode_append(node)) {
            return ABNF_MEMERR;
        }

        node->children[0].inpos = node->inpos;
    }

    struct parsenode* child = &node->children[0];

    int errcode;

    switch (ele->type) {
        case ELE_RULE:
            errcode = parse_ele_rule(grammar, ele->value.rulename, child, in);
            break;

        case ELE_GROUP:
            errcode = parse_ele_group(grammar, ele->value.alt, child, in);
            break;

        case ELE_OPTION:
            errcode = parse_ele_option(grammar, ele->value.alt, child, in);
            break;

        case ELE_TERMINAL:
            errcode = parse_ele_terminal(&ele->value.term, child, in);
            break;

        default:
            errcode = ABNF_DNE;
    }

    return errcode;
}

static int parse_rep(
        struct abnf* grammar,
        struct abnf_rep* rep,
        struct parsenode* node,
        struct chstream* in
) {
    /*
        note that parsing repetitions is very similar to parsing
        concatenations (see the algorithm listed below). for example,
        the repetition

            3element

        is the same as the concatenation

            element element element
    
        as such, we employ an algorithm that is very similar to the
        concatenation parsing algorithm.

        -----

        in the following description, we refer to this node's index
        *minus one* as "i". this way, this node's index refers to the
        current numbers of repetitions, and i refers to the index of the
        current child in the node array.

        INITIAL

        store the input position into the node at index 0

        if this node's index is 0 and the minimum number of repetitions is 0, 
        increment this node's index and return ABNF_NOERR.

        otherwise, increment this node's index and go to ITERATION.

        ITERATION

        if this node's index is greater than the maximum number of
        repetitions, go to FAILURE.

        reset the input position to the position saved in the node at index i.
        find the next successful parse path for element i.

        if a parse path was found, proceed to TERMINATION.
        otherwise, proceed to FAILURE.

        TERMINATION

        save the current input position into the node at index i + 1.

        if this node's index is greater than or equal to the minimum number of
        repetitions, increment this node's index and return ABNF_NOERR.

        if the input is empty, but this node's index is less than the minimum
        number of repetitions, return ABNF_PARSEERR.

        otherwise, increment this node's index and repeat ITERATION.

        FAILURE
        
        if i > 0, decrement this node's index and repeat ITERATION.
        otherwise, set this node to finished and return ABNF_PARSEERR.
        
        -----
    */

    // node is finished
    if (node->index < 0) {
        return ABNF_PARSEERR;
    }

    // INITIAL
    if (node->index == 0) {
        node->type = NODE_REP;

        if (!parsenode_append(node)) {
            return ABNF_MEMERR;
        }

        node->children[0].inpos = node->inpos;
        node->index++;

        if (rep->min == 0) {
            return ABNF_NOERR;
        }
    }

    int errcode;

    // ITERATION
    while (true) {
        // node->index refers to to the current repetition count, while i
        // refers to the index of the child node in the node array.
        int i = node->index - 1;

        struct parsenode* child = &node->children[i];

        if (rep->max == REP_UNDEF || node->index <= rep->max) {
            chstream_set(in, child->inpos);

            errcode = parse_ele(grammar, &rep->ele, child, in);
        }
        else {
            // go to FAILURE
            errcode = ABNF_PARSEERR;
        }

        // TERMINATION
        if (errcode == ABNF_NOERR) {
            // append another child node if needed
            if (i + 1 >= node->len && !parsenode_append(node)) {
                errcode = ABNF_MEMERR;
                break;
            }

            node->children[i + 1].inpos = in->pos;

            // the current number of parsed repetitions.
            int count = node->index;
            node->index++;

            if (count >= rep->min) {
                errcode = ABNF_NOERR;
                break;
            }

            if (chstream_eos(in)) {
                // undo the increment.
                node->index--;
                errcode = ABNF_PARSEERR;
                break;
            }
        }

        // FAILURE
        else if (errcode == ABNF_PARSEERR) {
            if (i > 0) {
                // free the current child node's children and reset it.
                reset_node(child);
                node->index--;
            }
            else {
                node->index = -1;
                errcode = ABNF_PARSEERR;
                break;
            }
        }
        
        // an unexpected error occurred. report it.
        else {
            break;
        }
    }

    return errcode;
}

static int parse_cat_init(struct abnf_cat* cat, struct parsenode* node)
{
    node->type = NODE_CAT;
    
    // append a child node for each repetition.
    for (size_t j = 0; j < cat->count; j++) {
        if (!parsenode_append(node)) {
            return ABNF_MEMERR;
        }
    }

    if (node->len <= 0) {
        node->index = -1;
        return ABNF_PARSEERR;
    }

    return ABNF_NOERR;
}

static int parse_cat(
        struct abnf* grammar,
        struct abnf_cat* cat,
        struct parsenode* node,
        struct chstream* in
) {
    /*
        this function is processed using the following algorithm:

        FIRST CALL:
        
        let i initially be 0.

        set the input position of the node at index 0 to this node's input
        position. proceed to ITERATION.

        REPEAT CALL:

        let i initially be the index of the final repetition.

        proceed to ITERATION

        ITERATION:

        reset the input position to the position saved in the node at index i.
        find the next successful parse path for repetition i.

        if a parse path was found, proceed to TERMINATION.
        otherwise, proceed to FAILURE.

        TERMINATION:
        
        if i is the final index, return ABNF_NOERR.
        otherwise, save the current input position into the node at index
        i + 1 and repeat ITERATION with i = i + 1.

        FAILURE:

        if i > 0, repeat ITERATION with i = i - 1.
        otherwise, set this node to finished and return ABNF_PARSEERR.

        *NOTE* a node index of less than 0 will indicate that the node is
        finished.
    */

    int errcode;
    size_t i;

    if (node->index < 0) {
        return ABNF_PARSEERR;
    }

    // FIRST CALL
    if (node->children == NULL) {
        errcode = parse_cat_init(cat, node);
        if (errcode != ABNF_NOERR) {
            return errcode;
        }
        i = 0;
        node->children[0].inpos = in->pos;
    }
    // REPEAT CALL
    else {
        i = cat->count - 1;
    }

    // ITERATION
    while (true) {
        struct parsenode* child = &node->children[i];
        chstream_set(in, child->inpos);

        errcode = parse_rep(grammar, &cat->reps[i], child, in);

        // TERMINATION
        if (errcode == ABNF_NOERR) {
            // non-final index
            if (i + 1 < node->len) {
                node->children[i + 1].inpos = in->pos;
                i++;
            }
            // final index
            else {
                return ABNF_NOERR;
            }
        }

        // FAILURE
        else if (errcode == ABNF_PARSEERR) {
            if (i > 0) {
                // free the current child node's children and reset it.
                reset_node(child);
                i--;
            }
            else {
                node->index = -1;
                return ABNF_PARSEERR;
            }
        }

        // An unexpected error occurred; report it.
        else {
            return errcode;
        }
    }
}

static int parse_alt(
        struct abnf* grammar,
        struct abnf_alt* alt,
        struct parsenode* node,
        struct chstream* in
) {
    // termination
    if (node->index >= alt->count) {
        return ABNF_PARSEERR;
    }

    // initialization
    if (node->len <= 0) {
        node->type = NODE_ALT;
        
        if (!parsenode_append(node)) {
            return ABNF_MEMERR;
        }

        node->children[0].inpos = node->inpos;
    }

    struct parsenode* child = &node->children[0];

    // keep checking parse paths until a successful path is found or all paths
    // have been exhausted.
    int errcode;
    do {
        errcode = parse_cat(grammar, &alt->cats[node->index], child, in);

        if (errcode == ABNF_PARSEERR) {
            // try the next concatenation.
            node->index++;

            reset_node(child);
            child->inpos = node->inpos;
        }
        else {
            // report any result that wasn't a parse error.
            break;
        }
    } while (node->index < alt->count);

    return errcode;
}

static int parse_elements(
        struct abnf* grammar,
        struct abnf_rule* rule,
        struct parsenode* root,
        struct chstream* in
) {
    int errcode;
    do {
        // reset the output data.
        errcode = parse_alt(grammar, &rule->alt, root->children, in);
        
        if (errcode != ABNF_NOERR) {
            break;
        }
        else if (!chstream_eos(in)){
            // a successful parse that leaves remaining input fails overall.
            errcode = ABNF_PARSEERR;
        }
    }
    while (errcode != ABNF_NOERR);

    return errcode;
}

static int parse_toplevel(
        struct abnf* grammar,
        const char* rulename,
        struct chstream* in,
        struct abnf_result* out
) {
    struct abnf_rule* rule = get_rule(grammar, rulename);
    if (rule == NULL) {
        return ABNF_DNE;
    }

    struct parsenode root;
    parsenode_init(&root, rulename, 0);
    if (!parsenode_append(&root)) {
        return ABNF_MEMERR;
    }
    
    int errcode = parse_elements(grammar, rule, &root, in);
    if (errcode == ABNF_NOERR) {
        errcode = write_results(&root, out);
    }
    
    parsenode_free(&root);
    return errcode;
}

int abnf_parse(
        struct abnf* grammar,
        const char* rulename,
        const char* input,
        struct abnf_result* out
) {
    int errcode;
    struct chstream in;
    abnf_freeresult(out);

    if (chstream_init(&in, input) != CHSTREAM_NOERR) {
        return ABNF_MEMERR;
    }

    errcode = parse_toplevel(grammar, rulename, &in, out);

    if (errcode != ABNF_NOERR) {
        abnf_freeresult(out);
    }

    chstream_free(&in);
    return errcode;
}

