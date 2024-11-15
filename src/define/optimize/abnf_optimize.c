/*
    Author: Alden Bernitt 2024/10/25

    this contains the abnf_optimize function along with its helper functions.
    the purpose of these functions is to re-structure a given rule to be more
    performant when being used to parse text while remaining logically
    equivalent.
*/

#define abnf_optimize(rule) optimize_alt(&(rule)->alt)

static _Bool remove_index(
        void* array,
        size_t* len,
        size_t elemsize,
        size_t* index,
        void** newarr
) {
    // slide the frontal elements back
    for (size_t i = *index + 1; i < *len; i++) {
        void* dest = (char*)array + (i - 1) * elemsize;
        void* src = (char*)array + i * elemsize;
        memcpy(dest, src, elemsize);
    }

    (*len)--;


    void* p = NULL;
    if (*len > 0) {
        // shrink the array to fit its new size
        p = realloc(array, elemsize * *len);
        if (!p) {
            return false;
        }
    }
    else {
        free(array);
    }

    *newarr = p;

    return true;

}

#include "abnf_optimize_rep.c"
#include "abnf_optimize_cat.c"

static _Bool optimize_alt(struct abnf_alt* alt) 
{
    for (size_t i = 0; i < alt->count; i++) {
        if (!optimize_cat(&alt->cats[i])) {
            return false;
        }

        // remove empty concatenations.
        if (alt->cats[i].count == 0) {
            abnf_freecat(&alt->cats[i]);

            void *newarr;
            size_t elesize = sizeof(struct abnf_cat);

            // references to the count and loop index are passed so that they
            // can be adjusted after the removal of the element.
            _Bool success = remove_index(
                    alt->cats, &alt->count, elesize, &i, &newarr);

            if (!success) {
                return false;
            }

            alt->cats = (struct abnf_cat*)newarr;
        }
    }

    return true;
}
