static _Bool optimize_alt(struct abnf_alt* alt);

static void steal_subrep(struct abnf_rep* rep)
{
    struct abnf_alt* subalt = rep->ele.value.alt;
    struct abnf_rep* subrep = &subalt->cats[0].reps[0];

    struct abnf_ele temp = subrep->ele;
    memset(&subrep->ele, 0, sizeof(subrep->ele));

    abnf_freeele(&rep->ele);

    rep->ele = temp;
}

static _Bool collapse_brackets(struct abnf_rep* rep)
{
    /*
        CASE 1:
        a*b( c*d( element )) -->
            if c == 0, 0*n( element ) where n = b * d
            if c == 1, a*n( element ) where n = b * d
            otherwise, cannot be simplified.
    
        CASE 2:
        a*b( c*d[ element ] )
        == a*b( c*d( 0*1( element )))
        == a*b( 0*d( element)) --> CASE 1

        CASE 3:
        a*b[ c*d( element )]
        == a*b( 0*1( c*d( element )))
        == 0*b( c*d( element )) --> CASE 1

        CASE 4:
        a*b[ c*d[ element]]
        == a*b( 0*1( c*d( 0*1( element ))))
        == a*b( 0*1( 0*d( element )))
        == a*b( 0*d( element )) --> CASE 1
    */


    struct abnf_alt* subalt = rep->ele.value.alt;

    if (subalt->count == 1 && subalt->cats[0].count == 1) {
        struct abnf_cat* subcat = &subalt->cats[0];
        struct abnf_rep* subrep = &subcat->reps[0];

        // assumed to be ELE_GROUP or ELE_OPTION
        enum ele_t type = rep->ele.type;

        enum ele_t subtype = subrep->ele.type;
        if (subtype != ELE_GROUP && subtype != ELE_OPTION) {
            // nothing to do.
            return true;
        }

         // see above comment for explanation.
        int a = rep->min, b = rep->max, c = subrep->min, d = subrep->max;

        // CASE 2
        if (type == ELE_GROUP && subtype == ELE_OPTION) {
            c = 0;
        }
        // CASE 3 and CASE 4
        else if (type == ELE_OPTION) {
            // CASE 3
            if (subtype == ELE_GROUP) {
                a = 0;
            }
            // CASE 4
            else {
                c = 0;
            }
        }

        // execute CASE 1
        if (c > 1) {
            // nothing to do.
            return true;
        }

        steal_subrep(rep);
        rep->ele.type = ELE_GROUP;
        rep->min = (c == 0) ? 0 : a;
        rep->max = (b == REP_UNDEF || d == REP_UNDEF) ? REP_UNDEF : b * d;
    }

    return true;
}

_Bool extract_group_literal(struct abnf_rep* rep)
{
    // a*b( c*d"abc" ) -->
    //      if c == 0, 0*n"abc" where n = b * d
    //      if c == 1, a*n"abc" where n = b * d
    //      otherwise, cannot be extracted

    // rep->ele.type is assumed to be ELE_GROUP or ELE_OPTION

    struct abnf_alt* subalt = rep->ele.value.alt;

    if (
            rep->ele.type == ELE_GROUP
            && subalt->count == 1 && subalt->cats[0].count == 1
    ) {
        struct abnf_cat* subcat = &subalt->cats[0];
        struct abnf_rep* subrep = &subcat->reps[0];

        if (subrep->ele.type == ELE_TERMINAL) {
            if (subrep->min > 1) {
                // nothing to be done
                return true;
            }

            int min = (subrep->min == 0) ? 0 : rep->min;
            int max = (rep->max == REP_UNDEF || subrep->max == REP_UNDEF)
                    ? REP_UNDEF : rep->max * subrep->max;

            steal_subrep(rep);

            rep->min = min;
            rep->max = max;
        }
    }

    return true;
}

_Bool optimize_subalt(struct abnf_rep* rep)
{
    struct abnf_alt* subalt = rep->ele.value.alt;

    return optimize_alt(subalt)
            && collapse_brackets(rep)
            && extract_group_literal(rep);
}

static _Bool compress_rep_literal(struct abnf_rep* rep)
{
    // 2"abc" --> "abcabc"
    if (
            rep->ele.type == ELE_TERMINAL
            && rep->ele.value.term.mode == TERM_AND
            && rep->min == rep->max
    ) {
        // concatenate the element's value to itself n-1 times.
        char* values = rep->ele.value.term.values;
        size_t len = strlen(values);
        size_t total = len * rep->min;

        
        void* p = realloc(values, sizeof(char) * (total + 1));
        if (!p) {
            return false;
        }
        values = (char*)p;
        
        // concatenate the value to itself.
        size_t i;
        for (i = 0; i < total; i++) {
            values[i] = values[i % len];
        }
        values[i] = '\0';

        // adjust the new repetition count
        rep->min = rep->max = 1;
    }

    return true;
}

static _Bool optimize_rep(struct abnf_rep* rep)
{
    if (
            (rep->ele.type == ELE_OPTION || rep->ele.type == ELE_GROUP)
            && !optimize_subalt(rep)
    ) {
        return false;
    }

    return compress_rep_literal(rep);
}

