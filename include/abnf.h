/*
    Author: Alden Bernitt 2024/10/25
    
    GUIDE:

    the "abnf" class of functions provide facilities for defining and parsing
    ABNF grammars. these functions adhere to the specifications provided by
    RFC 5234 and RFC 7405.

    an intialized abnf structure is required to make use of any of the other
    functions provided by this module.

        struct abnf grammar;
        abnf_init(&grammar);

    -----
    once an abnf structure has been successfully initialized, the next step is
    to define the grammar's rules. this is done using the abnf_define function.

        abnf_define(&grammar, "digit = %x30-39");

    note that the string input for abnf_define must adhere to rules defined in
    RFC 5234 (section 4).


    furthermore, in accordance to the specifications of RFC 5234:
        
        abnf_define(&grammar, "bit = \"0\"");
        abnf_define(&grammar, "bit =/ \"1\"");

    is equivalent to

        abnf_define(&grammar, "bit = \"0\" / \"1\"");


    however, the following results in an error (notice the "=" instead of a
    "=/" in the second call).

        abnf_define(&grammar, "bit = \"0\"");
        abnf_define(&grammar, "bit = \"1\"");

    -----
    now suppose we use the follow definitions for our grammar:

        abnf_define(&grammar, "sum = number \"+\" number");
        abnf_define(&grammar, "number = 1*digit");
        abnf_define(&grammar, "digit = %x30-39");

    we can now use this grammar to parse text input according to a "base rule":

        int result = abnf_parse(&grammar, "sum", "123+456", NULL);

    once this call has been executed, the <result> will contain a code
    that can be used to identify the success or failure of the parse (see
    abnf_parse for detailed information).

    the results of a parse can also be stored and accessed using an
    abnf_result structure:

        struct abnf_result result;
        abnf_initresult(&result);
        abnf_parse(&grammar, "sum", "123+456", &result);

    if the result of the parse succeeds, <result> will contain data that can
    be used to access the strings corresponding to the "top-level" names in
    the rule. in this case, "top-level" names are the names that appear in the
    definition of the rule used to parse the string.

    using the example above, the "sum" rule was used to parse the string. the
    definition of the "sum" rule contains two names: one for each of the
    "number" names. to access the strings corresponding to those names, we use
    the abnf_retrieve function:

        const char* num1, num2;
        int code1 = abnf_retrieve(&result, "number", 0, &num1);
        int code2 = abnf_retrieve(&result, "number", 1, &num2);

    after these calls are executed, <num1> will contain the c-string "123" and
    <num2> will contain the c-string "456". in particular, note that it is not
    possible to retrieve the "+" that was in the original input string since
    it is not associated with a name in the "sum" rule. for the same reason,
    the name "digit" is also inaccessible.

    note that in the case of names in repetitions, each repetition of the name
    can be accessed independently of the others. for example, let's add an
    additional rule to the previous grammar.

        abnf_define(&grammar, "numberseq = number 1*( \"number\" ));

    then we make the call

        struct abnf_result result2;
        abnf_initresult(&result2);
        abnf_parse(&result, "numberseq", "123.456.789", &result2);

    now we have

        char char* num1, num2, num3;
        abnf_retrieve(&result2, "number", 0, &num1); // num1 == "123"
        abnf_retrieve(&result2, "number", 1, &num2); // num2 == "456"
        abnf_retrieve(&result2, "number", 2, &num3); // num3 == "789"

    abnf_retrieve returns a code that can be used to identify the success or
    failure of the retrieval (see abnf_retrieve for detailed information).

    ERRORS:

    functions that may fail will return an integer value used to identify the
    specific issue that occurred. while the interpretation of this code varies
    per function, every function uses ABNF_MEMERR to indicate that the cause
    of failure was due to a failed attempt to allocate memory. functions that
    take recieve a NULL value for mandatory pointers will return ABNF_NULL.

    WARNING:

    the abnf_parse function will try its damnedest to find a successful parse
    path, meaning it will search every single possible combination of
    interpretations (within the confines of the defined rules) before
    returning unsuccessfully. while this is not a problem for sufficiently
    small input, large inputs with no correct parse path may block execution
    for potentially a very long time. therefore, in live enviroments, it is
    recommended to take some countermeasure to this, such as calling
    abnf_parse on a thread set to timeout.
*/
#ifndef ABNF_H
#define ABNF_H

#include <stddef.h>
#include "abnf_errcode.h"

struct abnf_rule;
struct result_entry;

struct abnf {
    struct abnf_rule* rules;
    size_t count;
};

struct abnf_result {
    struct result_entry* entries;
    size_t count;
};

// returns ABNF_NULL if <grammar> is null. returns ABNF_NOERR otherwise.
int abnf_init(struct abnf* grammar);

void abnf_free(struct abnf* grammar);

// returns  ABNF_NOERR if the rule was successfully added.
// returns  ABNF_PARSEERR if the rule input text was malformed.
// returns  ABNF_DEFERR if a new rule tried to be added with the same name as
//          one that already exists.
int abnf_define(struct abnf* grammar, const char* rule);

// returns  ABNF_NOERR if the input text was successfully parsed and the
//          results were successfully stored.
// returns  ABNF_PARSEERR if the input did not conform to the definition of
//          the rule indicated by <rulename>
// returns  ABNF_DNE if a rule name that was not defined <grammar> was
//          (directly or indirectly) referenced.
//
// NULL is an acceptable value for <res>. in such a case, the results of the
// parse are not stored and only the result code is returned.
int abnf_parse(
        struct abnf* grammar,
        const char* rulename,
        const char* input,
        struct abnf_result* res
);

// returns  ABNF_NOERR if <strp> was successfully assigned the requested
//          string.
// returns  ABNF_DNE if a string was not associated with the given name or
//          index.
//
// *WARNING*    the persistence of the string referenced by <strp> after a
//              successful call to this function is not guaranteed. to
//              guarantee persistence, copy the contents of the string into
//              a different string.
//
// *NOTE*   once this function has been successfully called using <result>,
//          it must eventually be freed using abnf_freeresult to prevent a
//          memory leak. it is not necessary to call abnf_freeresult between
//          multiple calls of this function.
//
// *NOTE*   even if the retrieval fails, <strp> will be assigned an empty
//          string
//
// *NOTE*   any rule defined with a repetition of zero (e.g. example = 0rule)
//          will never be matched no matter the input. thus, attempting tok
//          retrieve such a rule will always result in an error code of
//          ABNF_DNE.
int abnf_retrieve(
        struct abnf_result* result,
        const char* name,
        int index,
        char** strp
);

// returns ABNF_NULL if <result> is null. returns ABNF_NOERR otherwise.
int abnf_initresult(struct abnf_result* result);

void abnf_freeresult(struct abnf_result* result);

#endif
