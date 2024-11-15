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

static int define_uri(struct abnf* grammar)
{
    return abnf_define(grammar, "ALPHA = %x41-5A / %x61-71") == ABNF_NOERR
            && abnf_define(grammar, "CR = %x0D") == ABNF_NOERR
            && abnf_define(grammar, "DIGIT = %x30-39") == ABNF_NOERR
            && abnf_define(grammar, "DQUOTE = %x22") == ABNF_NOERR
            && abnf_define(grammar,
                    "HEXDIG = DIGIT /"
                    "\"A\" / \"B\" / \"C\" / \"D\" / \"E\" / \"F\" / "
                    "\"a\" / \"b\" / \"c\" / \"d\" / \"e\" / \"f\""
                    ) == ABNF_NOERR
            && abnf_define(grammar, "LF = %x0A") == ABNF_NOERR
            && abnf_define(grammar, "SP = %x20") == ABNF_NOERR
            && abnf_define(grammar,
                    "pct-encoded = \"%\" HEXDIG HEXDIG"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "reserved = gen-delims / sub-delims"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "gen-delims = "
                    "\":\" / \"/\" / \"?\" / \"#\" / "
                    "\"[\" / \"]\" / \"@\""
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "sub-delims = "
                    "\"!\" / \"$\" / \"&\" / \"'\" / "
                    "\"(\" / \")\" / \"*\" / \",\" / "
                    "\";\" / \"=\""
                    ) == ABNF_NOERR  
            && abnf_define(grammar,
                    "unreserved = "
                    "ALPHA / DIGIT / \"-\" / \".\" / \"_\" / \"~\""
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "URI = " 
                    "scheme \":\" heir-part "
                    "[ \"?\" query ] [ \"#\" fragment ]"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "heir-part = "
                    " \"//\" authority path-abempty "
                    "/ path-absolute / path-rootless / path-empty"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "scheme = "
                    "ALPHA *( ALPHA / DIGIT / \"+\" / \"-\" / \".\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "authority = "
                    "[userinfo \"@\" ] host [ \":\" port ]"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "userinfo = "
                    "*( unreserved / pct-encoded / sub-delims / \":\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "host = "
                    "IP-literal / IPv4address / reg-name"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "IP-literal = \"[\" ( IPv6address / IPvFuture ) \"]\""
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "IPvFuture = "
                    "\"v\" 1*HEXDIG \".\" "
                    "1*( unreserved / sub-delims / \":\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "IPv6address = "
                    "6( h16 \":\" ) ls32 / "
                    "\"::\" 5( h16 \":\" ) ls32 / "
                    "[ h16 ] \"::\" 4( h16 \":\" ) ls32 / "
                    "[ *1( h16 \":\" ) h16 ] \"::\" 3( h16 \":\" ) ls32 / "
                    "[ *2( h16 \":\" ) h16 ] \"::\" 2( h16 \":\" ) ls32 / "
                    "[ *3( h16 \":\" ) h16 ] \"::\" h16 \":\" ls32 / "
                    "[ *4( h16 \":\" ) h16 ] \"::\" ls32 / "
                    "[ *5( h16 \":\" ) h16 ] \"::\" h16 / "
                    "[ *6( h16 \":\" ) h16 ] \"::\""
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "ls32 = ( h16 \":\" h16 ) / IPv4address"
                    ) == ABNF_NOERR
            && abnf_define(grammar, "h16 = 1*4HEXDIG") == ABNF_NOERR
            && abnf_define(grammar,
                    "IPv4address = "
                    "dec-octet \".\" dec-octet \".\" "
                    "dec-octet \".\" dec-octet"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "dec-octet = "
                    "DIGIT / %x31-39 DIGIT / \"1\" 2DIGIT / "
                    "\"2\" %x30-34 DIGIT / \"25\" %x30-35"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "reg-name = "
                    "*( unreserved / pct-encoded / sub-delims )"
                    ) == ABNF_NOERR
            && abnf_define(grammar, "port = *DIGIT") == ABNF_NOERR
            && abnf_define(grammar,
                    "path = "
                    "path-abempty / path-absolute / path-noscheme / "
                    "path-rootless / path-empty"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "path-abempty = *( \"/\" segment )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "path-absolute = "
                    "\"/\" [ segment-nz *( \"/\" segment ) ]"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "path-noscheme = "
                    "segment-nz-nc *( \"/\" segment )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "path-rootless = "
                    "segment-nz *( \"/\" segment )"
                    ) == ABNF_NOERR
            && abnf_define(grammar, "path-empty = 0pchar") == ABNF_NOERR
            && abnf_define(grammar, "segment = *pchar") == ABNF_NOERR
            && abnf_define(grammar, "segment-nz = 1*pchar") == ABNF_NOERR
            && abnf_define(grammar,
                    "segment-nz-nc = "
                    "1*( unreserved / pct-encoded / sub-delims / \"@\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "pchar = "
                    "unreserved / pct-encoded / sub-delims / \":\" / \"@\""
                    ) == ABNF_NOERR
            && abnf_define(grammar, 
                    "query = *( pchar / \"/\" / \"?\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "fragment = *( pchar / \"/\" / \"?\" )"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "URI-reference = URI / relative-ref"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "relative-ref = "
                    "relative-part [ \"?\" query ] [ \"#\" fragment ]"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "relative-part = "
                    "\"//\" authority path-abempty / "
                    "path-absolute / path-noscheme / path-empty"
                    ) == ABNF_NOERR
            && abnf_define(grammar,
                    "absolute-URI = "
                    "scheme \":\" heir-part [ \"?\" query ]"
                    ) == ABNF_NOERR;
}

int abnf_defineset(struct abnf* grammar, int ruleset)
{
    switch (ruleset) {
        case ABNF_RULESET_CORE:
            return define_core(grammar);
        case ABNF_RULESET_URI:
            return define_uri(grammar);
        default:
            return ABNF_DNE;
    }
}
