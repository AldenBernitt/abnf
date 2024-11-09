/*
    Author: Alden Bernitt 2024/10/24

    this file contains the parsenode data structure as well as functions
    to initialize and manipulate its instances.

    the parsenode data structure is designed to be used in conjunction with
    abnf_parse (see abnf_parse.c).

    conceptually, a parsenode represents a junction point in the parse path.
    it contains the data necessary to parse the text input from a specific
    starting point, as well as data used to indicate whether or not all
    parse paths originating from that node have been exhausted.
*/

enum node_t {
    NODE_NONE,
    NODE_ALT,
    NODE_CAT,
    NODE_REP,
    NODE_ELE,
    NODE_ELE_RULE,
    NODE_ELE_GROUP,
    NODE_ELE_OPTION,
    NODE_ELE_TERMINAL_AND,
    NODE_ELE_TERMINAL_OR
};

struct parsenode {
    enum node_t type;

    // points to the child parsenodes of this node.
    // this either points to one parsenode or NULL in all cases except for
    // parsenodes used by parse_cat.
    // for parsenodes used by parse_cat, this points to an array of parsenodes
    // where each parsenode corresponds to one repetition in the concatenation.
    struct parsenode* children;

    // the number of children that his node can hold using its current buffer.
    size_t buflen;

    // the number of child parsenodes contained in this parsenode.
    size_t len;

    // this value abstractly identifies which parse path the current parsing
    // function could take. the interpretation of this value is left to the
    // parsing function.
    int index;

    // this value stores the position in the input text to begin parsing from.
    size_t inpos;

    // the rule name associated with this node.
    const char* name;

    // used to indicate the name depth of this node.
    // the node used with the call to abnf_parse has a depth of zero.
    // every time a name is encountered, the child node used to parse
    // the name is given a depth of the parent node's depth plus one.
    int depth;

    // the terminal value associated with this node (only applicable to
    // terminal nodes).
    char* str;
    char ch;
};

void parsenode_init(struct parsenode* node, const char* name, int depth)
{
    node->type = NODE_NONE;
    node->children = NULL;
    node->buflen = 0;
    node->len = 0;
    node->index = 0;
    node->inpos = 0;
    node->name = name;
    node->depth = depth;
    node->str = NULL;
    node->ch = '\0';
}

void parsenode_free(struct parsenode* node)
{
    // free all of this node's children first.
    for (int i = 0; i < node->len; i++) {
        if (node->children[i].children != NULL) {
            parsenode_free(&node->children[i]);
        }
    }

    free(node->children);
    free(node->str);
    parsenode_init(node, "", 0);
}

_Bool parsenode_append(struct parsenode* parent) {
    size_t newlen = parent->len + 1;

    // allocate more space if needed.
    if (newlen > parent->buflen) {
        size_t buflen = parent->buflen == 0 ? 8 : parent->buflen * 2;
        void * p = realloc(parent->children, sizeof(struct parsenode) * buflen);

        if (!p) {
            return false;
        }

        parent->children = (struct parsenode*)p;
        parent->buflen = buflen;
    }

    struct parsenode* child = &parent->children[parent->len];
    parsenode_init(child, parent->name, parent->depth);

    parent->len = newlen;

    return true;
}
