enum ele_t {
    ELE_NONE,
    ELE_RULE,
    ELE_GROUP,
    ELE_OPTION,
    ELE_TERMINAL
};

enum term_m {
    TERM_NONE,
    TERM_AND,
    TERM_OR
};

// denotes the case sensitivity of the terminal values.
enum term_sens {
    TERM_SENS,
    TERM_INSENS
};

struct abnf_alt;

struct abnf_term {
    enum term_m mode;
    enum term_sens sens;
    char* values;
};

struct abnf_ele {
    enum ele_t type;
    union {
        char* rulename;
        struct abnf_alt* alt;
        struct abnf_term term;
    } value;
};

struct abnf_rep {
    int min;
    int max;
    struct abnf_ele ele;
};

struct abnf_cat {
    struct abnf_rep* reps;
    size_t count;
};

struct abnf_alt {
    struct abnf_cat* cats;
    size_t count;
};

struct abnf_rule {
    char* name;
    struct abnf_alt alt;
};

void abnf_freealt(struct abnf_alt* alt);

void abnf_initele(struct abnf_ele* ele)
{
    ele->type = ELE_NONE;
    ele->value.term.mode = TERM_NONE;
    ele->value.term.sens = TERM_INSENS;
    memset(&ele->value, 0, sizeof(ele->value));
}

void abnf_freeele(struct abnf_ele* ele)
{
    switch (ele->type)
    {
        case ELE_RULE:
            free(ele->value.rulename);
            break;
        case ELE_GROUP: case ELE_OPTION:
            if (ele->value.alt != NULL) {
                abnf_freealt(ele->value.alt);
            }
            free(ele->value.alt);
            break;
         case ELE_TERMINAL:
            free(ele->value.term.values);
            break;
    }
}

void abnf_initrep(struct abnf_rep* rep)
{
    rep->min = rep->max = 0;
    abnf_initele(&rep->ele);
}

void abnf_freerep(struct abnf_rep* rep)
{
    rep->min = rep->max = 0;
    abnf_freeele(&rep->ele);
}

void abnf_initcat(struct abnf_cat* cat)
{
    cat->reps = NULL;
    cat->count = 0;
}

void abnf_freecat(struct abnf_cat* cat)
{
    if (cat->reps != NULL) {
        for (size_t i = 0; i < cat->count; i++) {
            abnf_freerep(&cat->reps[i]);
        }

        free(cat->reps);
        cat->reps = NULL;
    }

    cat->count = 0;
}

void abnf_initalt(struct abnf_alt* alt)
{
    alt->cats = NULL;
    alt->count = 0;
}

void abnf_freealt(struct abnf_alt* alt)
{
    if (alt->cats != NULL) {
        for (size_t i = 0; i < alt->count; i++) {
            abnf_freecat(&alt->cats[i]);
        } 

        free(alt->cats);
        alt->cats = NULL;
    }

    alt->count = 0;
}

void abnf_initrule(struct abnf_rule* rule)
{
    rule->name = NULL;
    abnf_initalt(&rule->alt);
}

void abnf_freerule(struct abnf_rule* rule)
{
    free(rule->name);
    abnf_freealt(&rule->alt);
}
