/*
    Author: Alden Bernitt 2024/10/28

    the functions in this file are used add the results of a success parse
    to an abnf_result structure. this is done by using a parsenode chain,
    assumed to have already been constructed by a successful parse, and
    retracing the chain to retrieve the neccessary text values.
*/

#define write_results_alt(node, text, out)\
    write_results_cat(&(node)->children[0], text, out)

#define write_results_rule_sub(node, text, out)\
    write_results_alt(&(node)->children[0], text, out)

#define write_results_rule(node, text, out)\
    ((node)->depth == 0\
        ? write_results_rule_top(node, out)\
        : write_results_rule_sub(node, text, out))

#define write_results_grp_opt(node, text, out)\
    write_results_alt(&(node)->children[0], text, out)

#define write_results_terminal_and(node, text)\
        write_results_terminal(text, (node)->str)

static int write_results_cat(
        struct parsenode* node,
        strbuffer* buf,
        struct abnf_result* out
);

static int write_results_rule_top(struct parsenode* node, struct abnf_result* out) 
{
    strbuffer buf;
    if (strbuffer_init(&buf) != STRBUF_NOERR) {
        return ABNF_MEMERR;
    }
        
    int errcode = write_results_alt(&node->children[0], &buf, out);
    if (errcode != ABNF_NOERR) {
        return errcode;
    }

    errcode = results_add(out, node->name, strbuffer_buffer(&buf))
            ? ABNF_NOERR
            : ABNF_MEMERR;

    strbuffer_free(&buf);
    return errcode;
}

static int write_results_terminal(
        strbuffer* text,
        const char* str
) {
    if (!text) {
        return ABNF_NOERR;
    }

    return strbuffer_concatstr(text, str) == STRBUF_NOERR
            ? ABNF_NOERR
            : ABNF_MEMERR;
}

static int write_results_terminal_or(
        struct parsenode* node,
        strbuffer* text
) {
    char str[2];
    str[0] = node->ch;
    str[1] = '\0';

    return write_results_terminal(text, str);
}


static int write_results_ele(
        struct parsenode* node,
        strbuffer* text,
        struct abnf_result* out
) {
    struct parsenode* child = &node->children[0];

    int errcode;

    switch (child->type) {
        case NODE_ELE_RULE:
            errcode = write_results_rule(child, text, out);
            break;

        case NODE_ELE_GROUP: case NODE_ELE_OPTION:
            errcode = write_results_grp_opt(child, text, out);
            break;

        case NODE_ELE_TERMINAL_AND:
            errcode = write_results_terminal_and(child, text);
            break;

        case NODE_ELE_TERMINAL_OR:
            errcode = write_results_terminal_or(child, text);
            break;

        default:
            errcode = ABNF_DNE;
    }

    return errcode;
}

static int write_results_rep(
        struct parsenode* node,
        strbuffer* text,
        struct abnf_result* out
) {
    int errcode = ABNF_NOERR;

    // the parse_rep algorithm adds one extra child than what is needed.
    // the real number of children is node->len - 1
    for (size_t i = 0; i < node->len - 1; i++) {
        errcode = write_results_ele(&node->children[i], text, out);
        if (errcode != ABNF_NOERR) {
            break;
        }
    }

    return errcode;
}

static int write_results_cat(
        struct parsenode* node,
        strbuffer* text,
        struct abnf_result* out
) {
    int errcode = ABNF_NOERR;

    for (size_t i = 0; i < node->len; i++) {
        errcode = write_results_rep(&node->children[i], text, out);
        if (errcode != ABNF_NOERR) {
            break;
        }
    }

    return errcode;
}

static int write_results(struct parsenode* node, struct abnf_result* out)
{
    if (!out) {
        // writing the results is optional, so a NULL value is acceptable.
        return ABNF_NOERR;
    }

    return write_results_alt(node->children, NULL, out);
}
