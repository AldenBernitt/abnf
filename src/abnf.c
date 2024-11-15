#include "abnf.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "varr.h"
#include "strbuffer.h"
#include "chstream.h"
#include "convert.h"
#include "strutil.h"

#include "abnf_defs.c"

static struct abnf_rule* get_rule(struct abnf* grammar, const char* name)
{
    for (size_t i = 0; i < grammar->count; i++) {
        struct abnf_rule* rule = &grammar->rules[i];
        if (strcmp(rule->name, name) == 0) {
            return rule;
        }
    }
    return NULL;
}

int abnf_init(struct abnf* grammar)
{
    if (!grammar) {
        return ABNF_NULL;
    }

    grammar->rules = NULL;
    grammar->count = 0;

    return ABNF_NOERR;
}

void abnf_free(struct abnf* grammar)
{
    if (!grammar) {
        return;
    }

    if (grammar->rules != NULL) {
        for (size_t i = 0; i < grammar->count; i++) {
            abnf_freerule(&grammar->rules[i]);
        }

        free(grammar->rules);
        grammar->rules = NULL;
    }

    grammar->count = 0;
}

#include "abnf_result.c"
#include "define/abnf_define.c"
#include "parse/abnf_parse.c"

int abnf_retrieve(
        struct abnf_result* result,
        const char* name,
        int index,
        char** strp
) {
    if (!result || !strp) {
        return ABNF_NULL;
    }

    // make sure the name exists.
    _Bool match = false;
    size_t i;
    for (i = 0; i < result->count; i++) {
        if (strcmp(name, result->entries[i].name) == 0) {
            match = true;
            break;
        }
    }

    if (!match) {
        *strp = "";
        return ABNF_DNE;
    }

    struct result_entry* entry = &result->entries[i];

    if (index >= entry->len) {
        *strp = "";
        return ABNF_DNE;
    }

    *strp = entry->values[index];
    return ABNF_NOERR;
}
