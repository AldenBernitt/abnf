#include <stdio.h>
#include "abnf.h"
#include "abnf_ruleset.h"

int main()
{
    struct abnf grammar;
    struct abnf core_grammar;
    struct abnf uri_grammar;
    abnf_init(&grammar);
    abnf_init(&core_grammar);
    abnf_init(&uri_grammar);

    int code = abnf_define(&grammar, "abnf=0rule");
    abnf_parse(&grammar, "abnf", "", NULL);
    int core_success = abnf_defineset(&core_grammar, ABNF_RULESET_CORE);
    int uri_success = abnf_defineset(&uri_grammar, ABNF_RULESET_URI);

    if (
            code == ABNF_NOERR
            && core_success
            && uri_success
    ) {
        printf("SUCCESS\n");
    }
    else {
        printf(
                "FAILED\n"
                "DEFINE: %d\n"
                "CORE: %d\n"
                "URI: %d\n",
                code == ABNF_NOERR, core_success, uri_success
        );
    }

    abnf_free(&grammar);
    abnf_free(&core_grammar);
    abnf_free(&uri_grammar);
    return 0;
}
