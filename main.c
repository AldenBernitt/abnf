#include <stdio.h>
#include "abnf.h"

int main()
{
    struct abnf grammar;
    abnf_init(&grammar);

    int err = abnf_define(&grammar, "abnf=%x65");
    if (err == ABNF_NOERR) {
        printf("SUCCESS\n");
    }
    else {
        printf("FAILED\n");
    }

    abnf_free(&grammar);
    return 0;
}
