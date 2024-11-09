_Bool compress_literal_seq(struct abnf_cat* cat, int start, int end)
{
    size_t len = 0;
    for (int i = start; i <= end; i++) {
        len += strlen(cat->reps[i].ele.value.term.values);
    }

    char* dest = cat->reps[start].ele.value.term.values;

    void *p = realloc(dest, sizeof(char) * (len + 1));
    if (!p) {
        return false;
    }
    dest = (char*)p;

    // copy the characters from all of the terminals into the first terminal.
    int j = start;
    while (j <= end) {
        char* values = cat->reps[j].ele.value.term.values;
        while (*values != '\0') {
            *(dest++) = *(values++);
        }
        j++;
    }
    *dest = '\0';

    return true;
}

_Bool remove_repseq(struct abnf_cat* cat, size_t start, size_t end)
{
    size_t i;

    for (i = start; i <= end; i++) {
        abnf_freerep(&cat->reps[i]);
    }
    
    // shift the frontal elements back to fill in the space left by the
    // removed elements.
    size_t shift = end - start + 1;

    for (i = end + 1; i < cat->count; i++) {
        size_t desti = start + i - (end + 1);
        cat->reps[desti] = cat->reps[i];
    }

    // note that the shift amount is equal to the amount of elements removed.
    cat->count -= shift;

    // shrink the repetition array to fit its new contents.
    void* p = realloc(cat->reps, sizeof(struct abnf_rep) * cat->count);

    if (!p) {
        return false;
    }
    
    cat->reps = (struct abnf_rep*)p;

    return true;
}

_Bool compress_cat_literals(struct abnf_cat* cat)
{
    int start = -1;
    int end = -1;

    // used to ensure that "and" terminals and "or" terminals are not
    // compressed together.
    enum term_m mode = TERM_NONE;

    // "abc" "123" --> "abc123"
    for (size_t i = 0; i + 1 < cat->count; i++) {
        struct abnf_rep* rep = &cat->reps[i];

        // is this the start of a literal sequence?
        if (
                rep->ele.type == ELE_TERMINAL
                && rep->min == rep->max && rep->max == 1
        ) {
            start = i;
            mode = rep->ele.value.term.mode;

            // find the end of the sequence of literals
            for (size_t j = start + 1; j < cat->count; j++) {
                struct abnf_rep* rep2 = &cat->reps[j];

                if (
                        !(rep2->ele.type == ELE_TERMINAL
                            && rep2->min == rep2->max && rep2->max == 1
                            && rep2->ele.value.term.mode == mode)
                ) {
                    break;
                }
                end = j;
            }

            // compress the sequence
            if (start < end) {
                if (
                        !(compress_literal_seq(cat, start, end)
                            && remove_repseq(cat, start + 1, end))
                ) {
                    return false;
                }
                i = start;
                start = end = -1;
                mode = TERM_NONE;
            }
        }
    }

    return true;
}

_Bool optimize_cat(struct abnf_cat* cat)
{
    for (size_t i = 0; i < cat->count; i++) {
        if (!optimize_rep(&cat->reps[i])) {
            return false;
        }

        // remove empty repetitions.
        // a pointer to i is passed to the removal function so that the loop
        // counter can be adjusted after the removal.

        // a*0( element ) --> x
        if (cat->reps[i].max == 0) {
            abnf_freerep(&cat->reps[i]);

            void* newarr;
            size_t elesize = sizeof(struct abnf_rep);

            // references to the count and loop index are passed so that they
            // can be adjusted after the removal of the element.
            _Bool success = remove_index(
                    cat->reps, &cat->count, elesize, &i, &newarr);

            if (!success) {
                return false;
            }

            cat->reps = (struct abnf_rep*)newarr;
        }
    }

    return compress_cat_literals(cat);
}

