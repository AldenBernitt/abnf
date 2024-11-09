#ifndef ABNF_RULESET_H
#define ABNF_RULESET_H

#include "abnf.h"

enum {
    // RFC 5234 Appendix B.1.
    ABNF_RULESET_CORE
};

int abnf_defineset(struct abnf* grammar, int ruleset);

#endif
