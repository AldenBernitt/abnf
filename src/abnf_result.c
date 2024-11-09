struct result_entry {
    char* name;
    char** values;
    size_t len;
};

void free_entry(struct result_entry* entry)
{
    for (size_t i = 0; i < entry->len; i++) {
        free(entry->values[i]);
    }

    free(entry->name);
    free(entry->values);

    entry->name = NULL;
    entry->values = NULL;
    entry->len = 0;
}

_Bool result_add_existing(
        struct abnf_result* result,
        size_t index,
        const char* value
) {
    struct result_entry* entry = &result->entries[index];

    size_t newlen = entry->len + 1;
    void* p = realloc(entry->values, sizeof(char*) * newlen);

    _Bool success = true;

    if (p) {
        entry->values = (char**)p;
        entry->len = newlen;

        char* val = str_alloc(value);
        
        if (val == NULL) {
            success = false;
        }
        else {
            entry->values[newlen - 1] = val;
        }
    }
    else {
        success = false;
    }

    return success;
}

_Bool result_add_new(
        struct abnf_result* result,
        const char* name,
        const char* value
) {
    size_t newcount = result->count + 1;
    void* p = realloc(result->entries, sizeof(struct result_entry) * newcount);

    _Bool success = true;

    if (p) {
        result->entries = (struct result_entry*)p;
        result->count = newcount;

        struct result_entry* entry = &result->entries[newcount - 1];

        entry->name = str_alloc(name);
        entry->values = NULL;
        entry->len = 0;

        if (entry->name == NULL) {
            success = false;
        }
        else if (!result_add_existing(result, result->count - 1, value)) {
            free(entry->name);
            success = false;
        }
    }
    else {
        success = false;
    }
    
    return success;
}

_Bool results_add(
        struct abnf_result* result,
        const char* name,
        const char* value
) {
    _Bool newname = true;
    size_t i;
    for (i = 0; i < result->count; i++) {
        if (strcmp(name, result->entries[i].name) == 0) {
            newname = false;
            break;
        }
    }

    return newname
            ? result_add_new(result, name, value)
            : result_add_existing(result, i, value);
}

int abnf_initresult(struct abnf_result* result)
{
    if (!result) {
        return ABNF_NULL;
    }

    result->entries = NULL;
    result->count = 0;

    return ABNF_NOERR;
}

void abnf_freeresult(struct abnf_result* result)
{
    if (!result) {
        return;
    }

    for (size_t i = 0; i < result->count; i++) {
        free_entry(&result->entries[i]);
    }
    free(result->entries);
    abnf_initresult(result);
}
