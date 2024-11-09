#define abnf_normalize(rule) normalize_alt(&(rule)->alt)

static _Bool set_sensitivity(struct abnf_rep* rep)
{
    // convert all characters to lowercase if the case-insensitive flag is
    // set.
    if (
            rep->ele.type == ELE_TERMINAL
            && rep->ele.value.term.sens == TERM_INSENS
    ) {
        char* vals = rep->ele.value.term.values;

        while (*vals != '\0') {
            *vals = tolower(*vals);
            vals++;
        }
    }

    return true;
}

static _Bool convert_option(struct abnf_rep* rep)
{
    // a*b[ element ] --> *b( element )
    if (rep->ele.type == ELE_OPTION) {
        rep->ele.type = ELE_GROUP;
        rep->min = 0;
    }

    return true;
}

static _Bool normalize_rep(struct abnf_rep* rep)
{
    convert_option(rep);
    set_sensitivity(rep);
    return true;
}

static _Bool normalize_cat(struct abnf_cat* cat)
{
    for (size_t i = 0; i < cat->count; i++) {
        if (!normalize_rep(&cat->reps[i])) {
            return false;
        }
    }
    return true;
}

static _Bool normalize_alt(struct abnf_alt* alt)
{
    for (size_t i = 0; i < alt->count; i++) {
        if (!normalize_cat(&alt->cats[i])) {
            return false;
        }
    }
    return true;
}
