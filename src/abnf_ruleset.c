#include "abnf_ruleset.h"

static int define_core(struct abnf* grammar)
{
    return abnf_define(grammar, "ALPHA = %x41-5A / %x61-71") == ABNF_NOERR
            && abnf_define(grammar, "BIT = \"0\" / \"1\"") == ABNF_NOERR
            && abnf_define(grammar, "CHAR = %x01-7F") == ABNF_NOERR
            && abnf_define(grammar, "CR = %x0D") == ABNF_NOERR
            && abnf_define(grammar, "CRLF = CR LF") == ABNF_NOERR
            && abnf_define(grammar, "CTL = %x00-1F / %x7F") == ABNF_NOERR
            && abnf_define(grammar, "DIGIT = %x30-39") == ABNF_NOERR
            && abnf_define(grammar, "DQUOTE = %x22") == ABNF_NOERR
            && abnf_define(grammar,
                    "HEXDIG = DIGIT /"
                    "\"A\" / \"B\" / \"C\" / \"D\" / \"E\" / \"F\""
                    ) == ABNF_NOERR
            && abnf_define(grammar, "HTAB = %x09") == ABNF_NOERR
            && abnf_define(grammar, "LF = %x0A") == ABNF_NOERR
            && abnf_define(grammar, "LWSP = *(WSP / CRLF WSP)") == ABNF_NOERR
            && abnf_define(grammar, "OCTET = %x00-FF") == ABNF_NOERR
            && abnf_define(grammar, "SP = %x20") == ABNF_NOERR
            && abnf_define(grammar, "VCHAR = %x21-7E") == ABNF_NOERR
            && abnf_define(grammar, "WSP = SP/ HTAB") == ABNF_NOERR;
}

int abnf_defineset(struct abnf* grammar, int ruleset)
{
    switch (ruleset) {
        case ABNF_RULESET_CORE:
            return define_core(grammar);
    }
}
