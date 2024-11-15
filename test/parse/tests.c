#include "abnf.h"
#include <stdbool.h>

#define test_parse(rule, in, ex)\
        (abnf_parse(&grammar, rule, in, NULL) == ex)
#define test_parse_noerr(rule, in) test_parse(rule, in, ABNF_NOERR)
#define test_parse_parseerr(rule, in) test_parse(rule, in, ABNF_PARSEERR)
#define test_parse_dne(rule, in) test_parse(rule, in, ABNF_DNE)

struct abnf grammar;
struct abnf abnfgrammar;

//@PRE
_Bool setup()
{
    abnf_init(&grammar);

    _Bool success =  abnf_define(&grammar, "char = \"1\"") == ABNF_NOERR
        && abnf_define(&grammar, "chrange = 2*4\"a\"") == ABNF_NOERR
        && abnf_define(&grammar, "chmin = 2*\"#\"") == ABNF_NOERR
        && abnf_define(&grammar, "chmax = *2\".\"") == ABNF_NOERR
        && abnf_define(&grammar, "chnolim = *\".\"") == ABNF_NOERR
        && abnf_define(&grammar, "prose = <1>") == ABNF_NOERR
        && abnf_define(&grammar, "dec = %d49") == ABNF_NOERR
        && abnf_define(&grammar, "hex = %x31") == ABNF_NOERR
        && abnf_define(&grammar, "bin = %b110001") == ABNF_NOERR
        && abnf_define(&grammar, "zeromin = 0*2\".\"") == ABNF_NOERR
        && abnf_define(&grammar, "bigrange = 12*14\"+\"") == ABNF_NOERR
        && abnf_define(&grammar, "string = \"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "strrange = 2*4\"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "strzero = 0\"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "strmin = 2*\"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "strmax = *2\"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "strnolim = *\"ab12\"") == ABNF_NOERR
        && abnf_define(&grammar, "concat = \"a\" \"b\"") == ABNF_NOERR
        && abnf_define(&grammar, "catrange = 2*4(\"a\" \"b\")") == ABNF_NOERR
        && abnf_define(&grammar, "catmin = 2*(\"a\" \"b\")") == ABNF_NOERR
        && abnf_define(&grammar, "catmax = *2(\"a\" \"b\")") == ABNF_NOERR
        && abnf_define(&grammar, "catnolim = *(\"a\" \"b\")") == ABNF_NOERR
        && abnf_define(&grammar, "alt = \"a\" / \"b\" / \"c\"") == ABNF_NOERR
        && abnf_define(&grammar,
                "altrange = 2*4(\"a\" / \"b\" / \"c\")") == ABNF_NOERR
        && abnf_define(&grammar,
                "altmin = 2*(\"a\" / \"b\" / \"c\")") == ABNF_NOERR
        && abnf_define(&grammar,
                "altmax = *2(\"a\" / \"b\" / \"c\")") == ABNF_NOERR
        && abnf_define(&grammar,
                "altnolim = *(\"a\" / \"b\" / \"c\")") == ABNF_NOERR
        && abnf_define(&grammar,
                "catalt = \"a\" \"b\" / \"1\" \"2\"") == ABNF_NOERR
        && abnf_define(&grammar,
                "altcat = \"a\" (\"1\" / \"2\") \"b\"") == ABNF_NOERR
        && abnf_define(&grammar, "option = [ \"abc\" ]") == ABNF_NOERR
        && abnf_define(&grammar, "optrange = 2*4[ \"abc\" ]") == ABNF_NOERR
        && abnf_define(&grammar, "optmin = 2*[ \"abc\" ]") == ABNF_NOERR
        && abnf_define(&grammar, "optmax = *4[ \"abc\" ]") == ABNF_NOERR
        && abnf_define(&grammar, "optnolim = *[ \"abc\" ]") == ABNF_NOERR
        && abnf_define(&grammar, "optgroup = *(*1( \"abc\" ))") == ABNF_NOERR
        && abnf_define(&grammar,
                "nolimnest = *(*(*(*(*(*(*( \"abc\" )))))))") == ABNF_NOERR
        && abnf_define(&grammar,
                "nolimnestopt = *(*[ \"abc\"])") == ABNF_NOERR
        && abnf_define(&grammar, "space = %x20") == ABNF_NOERR
        && abnf_define(&grammar, "htab = %x09") == ABNF_NOERR
        && abnf_define(&grammar,
                "altoverlap1 = 1*\"a\" / *\"a\" \"#\"") == ABNF_NOERR
        && abnf_define(&grammar,
                "altoverlap2 = *\"a\" \"#\" / 1*\"a\"") == ABNF_NOERR
        && abnf_define(&grammar, "repoverlap = *\"a\" \"aa#\"") == ABNF_NOERR
        && abnf_define(&grammar,
                "optoverlap = [ \"a\" ] \"aab\"") == ABNF_NOERR

        // the characters from 'a' to 'c'
        && abnf_define(&grammar, "decrange = %d97-99") == ABNF_NOERR
        && abnf_define(&grammar, "hexrange = %x61-63") == ABNF_NOERR
        && abnf_define(&grammar,
                "binrange = %b01100001-01100011") == ABNF_NOERR

        // the string "abc"
        && abnf_define(&grammar, "deccat = %d97.98.99") == ABNF_NOERR
        && abnf_define(&grammar, "hexcat = %x61.62.63") == ABNF_NOERR
        && abnf_define(&grammar,
                "bincat = %b01100001.01100010.01100011") == ABNF_NOERR

        && abnf_define(&grammar, "prosequot = <\">") == ABNF_NOERR
        && abnf_define(&grammar, "quotprose = \"<\"") == ABNF_NOERR

        // names
        && abnf_define(&grammar, "badname = dne") == ABNF_NOERR
        && abnf_define(&grammar, "name = strrange") == ABNF_NOERR
        && abnf_define(&grammar, "namealt = decrange / chmin") == ABNF_NOERR
        && abnf_define(&grammar, "namecat = alt chmin") == ABNF_NOERR
        && abnf_define(&grammar, "namenested = namealt") == ABNF_NOERR
        && abnf_define(&grammar, "emptyname = 0string") == ABNF_NOERR

        /*
            abnf grammar definitions as described in
            RFC 5234 (Augmented BNF for Syntax Specifications: ABNF)
        */
        && abnf_define(&abnfgrammar,
            "rulelist\t =  1*( rule / (*c-wsp c-nl) )") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "rule\t =  rulename defined-as elements c-nl\r\n"
            "\t ; continues if next line starts\r\n"
            "\t ;  with white space") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "rulename\t =  ALPHA *(ALPHA / DIGIT / \"-\")") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "defined-as\t =  *c-wsp (\"=\" / \"=/\") *c-wsp\r\n"
            "\t\t ; basic rules definition and\r\n"
            "\t\t ;  incremental alternatives") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "elements\t =  alternation *c-wsp") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "c-wsp\t =  WSP / (c-nl WSP)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "c-nl\t =  comment / CRLF"
            "\r\n\t\t ; comment or newline") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "comment\t =  \";\" *(WSP / VCHAR) CRLF") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "alternation\t =  concatenation\r\n"
            "\t\t*(c-wsp \"/\" *c-wsp concatenation)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "concatenation\t =  "
            "repetition *(1*c-wsp repetition)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "repetition\t =  [repeat] element") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "repeat\t =  1*DIGIT / (*DIGIT \"*\" *DIGIT)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "element\t =  rulename / group / option /\r\n"
            "\t\tchar-val / num-val / prose-val") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "group\t =  \"(\" *c-wsp alternation *c-wsp \")\""
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "option\t =  \"[\" *c-wsp alternation *c-wsp \"]\""
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "char-val\t =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE\r\n"
            "\t\t ; quoted string of SP and VCHAR\r\n"
            "\t\t ;  without DQUOTE") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "num-val\t =  \"%\" (bin-val / dec-val / hex-val)"
            ) == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "bin-val\t =  \"b\" 1*BIT\r\n"
            "\t\t[ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]\r\n"
            "\t\t ; series of concatenated bit values\r\n"
            "\t\t ;   or single ONEOF range") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "dec-val\t =  \"d\" 1*DIGIT\r\n"
            "\t\t[ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]\r\n") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "hex-val\t =  \"x\" 1*HEXDIG\r\n"
            "\t\t[ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]\r\n") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "prose-val\t =  \"<\" *(%x20-3D / %x3F-7E) \">\"\r\n"
            "\t\t ; bracketed string of SP and VCHAR\r\n"
            "\t\t ;  without angles\r\n"
            "\t\t ; prose description, to be used as\r\n"
            "\t\t ;  last resort") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "ALPHA\t =  %x41-5A / %x61-7A\t; A-Z / a-z") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "BIT\t =  \"0\" / \"1\"") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CHAR\t =  %x01-7F\r\n"
            "\t\t ; any 7-bit US-ASCII character,\r\n"
            "\t\t ; excluding NUL") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CR\t =  %x0D\r\n\t\t ; carriage return") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CRLF\t =  CR LF\r\n"
            "\t\t ; Internet standard newline") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "CTL\t =  %x00-1F / %x7F\r\n\t\t ; controls") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "DIGIT\t =  %x30-39\r\n\t\t ; 0-9") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "DQUOTE\t =  %x22\r\n\t\t ; \" (Double Quote)") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "HEXDIG\t =  DIGIT / \"A\" / \"B\" / \"C\" / \"D\" / \"E\" "
            "/ \"F\"") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "HTAB\t =  %x09\r\n\t\t ; horizontal tab") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "LF\t =  %x0A\r\n\t\t ; linefeed") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "LWSP\t =  *(WSP / CRLF WSP)\r\n"
            "\t\t ; Use of this linear-white-space rule\r\n"
            "\t\t ;  permits lines containing only white\r\n"
            "\t\t ;  space that are no longer legal in\r\n"
            "\t\t ;  mail headers and have caused\r\n"
            "\t\t ;  interoperability problems in other\r\n"
            "\t\t ;  contexts.\r\n"
            "\t\t ; Do not use when defining mail\r\n"
            "\t\t ;  headers and use with caution in\r\n"
            "\t\t ;  other contexts.") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "OCTET\t =  %x00-FF\r\n\t\t ; 8 bits of data") == ABNF_NOERR
        && abnf_define(&abnfgrammar, "SP\t =  %x20") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "VCHAR\t =  %x21-7E\r\n"
            "\t\t ; visible (printing) characters") == ABNF_NOERR
        && abnf_define(&abnfgrammar,
            "WSP\t =  SP / HTAB\r\n\t\t ; white space") == ABNF_NOERR;

    return success;
}


//@POST
_Bool finish()
{
    abnf_free(&grammar);
    abnf_free(&abnfgrammar);
    return true;
}

//@TEST
_Bool parse_char()
{
    return test_parse_noerr("char", "1");
}

//@TEST
_Bool parse_char_error_empty()
{
    return test_parse_parseerr("char", "");
}

//@TEST
_Bool parse_char_error_invalid_character()
{
    return test_parse_parseerr("char", "a");
}

//@TEST
_Bool parse_char_error_remaining_characters()
{
    return test_parse_parseerr("char", "1a");
}

//@TEST
_Bool parse_char_error_too_many_characters()
{
    return test_parse_parseerr("char", "11");
}

//@TEST
_Bool parse_prose()
{
    return test_parse_noerr("prose", "1");
}

//@TEST
_Bool parse_prose_error_empty()
{
    return test_parse_parseerr("prose", "");
}

//@TEST
_Bool parse_prose_error_invalid_character()
{
    return test_parse_parseerr("prose", "a");
}

//@TEST
_Bool parse_prose_error_remaining_characters()
{
    return test_parse_parseerr("prose", "1a");
}

//@TEST
_Bool parse_prose_error_too_many_characters()
{
    return test_parse_parseerr("prose", "11");
}

//@TEST
_Bool parse_chrange_lower()
{
    return test_parse_noerr("chrange", "aa");
}

//@TEST
_Bool parse_chrange_middle()
{
    return test_parse_noerr("chrange", "aaa");
}

//@TEST
_Bool parse_chrange_upper()
{
    return test_parse_noerr("chrange", "aaaa");
}

//@TEST
_Bool parse_chrange_invalid_character()
{
    return test_parse_parseerr("chrange", "xx");
}

//@TEST
_Bool parse_chrange_error_empty()
{
    return test_parse_parseerr("chrange", "");
}

//@TEST
_Bool parse_chrange_error_too_few()
{
    return test_parse_parseerr("chrange", "a");
}

//@TEST
_Bool parse_chrange_error_too_many()
{
    return test_parse_parseerr("chrange", "aaaaa");
}

//@TEST
_Bool parse_chmin_lower()
{
    return test_parse_noerr("chmin", "##");
}

//@TEST
_Bool parse_chmin_lots()
{
    return test_parse_noerr("chmin", "#####################################");
}

//@TEST
_Bool parse_chmin_error_empty()
{
    return test_parse_parseerr("chmin", "");
}

//@TEST
_Bool parse_chmin_error_invalid_character()
{
    return test_parse_parseerr("chmin", "xx");
}

//@TEST
_Bool parse_chmin_error_too_few()
{
    return test_parse_parseerr("chmin", "#");
}

//@TEST
_Bool parse_chmax_zero()
{
    return test_parse_noerr("chmax", "");
}

//@TEST
_Bool parse_chmax_middle()
{
    return test_parse_noerr("chmax", ".");
}

//@TEST
_Bool parse_chmax_upper()
{
    return test_parse_noerr("chmax", "..");
}

//@TEST
_Bool parse_chmax_error_invalid_character()
{
    return test_parse_parseerr("chmax", "x");
}

//@TEST
_Bool parse_chmax_error_too_many()
{
    return test_parse_parseerr("chmax", "...");
}

//@TEST
_Bool parse_chnolim_zero()
{
    return test_parse_noerr("chnolim", "");
}

//@TEST
_Bool parse_chnolim_one()
{
    return test_parse_noerr("chnolim", ".");
}

//@TEST
_Bool parse_chnolim_many()
{
    return test_parse_noerr("chnolim", "......................");
}

//@TEST
_Bool parse_chnolim_error_invalid_character_one()
{
    return test_parse_parseerr("chnolim", "x");
}

//@TEST
_Bool parse_chnolim_error_invalid_character_many()
{
    return test_parse_parseerr("chnolim", "xxxxxxxxxxxxxxxxxxxxx");
}

//@TEST
_Bool parse_chnolim_error_invalid_character_first()
{
    return test_parse_parseerr("chnolim", "x....................");
}

//@TEST
_Bool parse_chnolim_error_invalid_character_middle()
{
    return test_parse_parseerr("chnolim", "........x............");
}

//@TEST
_Bool parse_chnolim_error_invalid_character_last()
{
    return test_parse_parseerr("chnolim", "....................x");
}

//@TEST
_Bool parse_dec()
{
    return test_parse_noerr("dec", "1");
}

//@TEST
_Bool parse_dec_error_empty()
{
    return test_parse_parseerr("dec", "");
}

//@TEST
_Bool parse_dec_error_invalid_character()
{
    return test_parse_parseerr("dec", "a");
}

//@TEST
_Bool parse_dec_error_remaining_characters()
{
    return test_parse_parseerr("dec", "1a");
}

//@TEST
_Bool parse_dec_error_too_many_characters()
{
    return test_parse_parseerr("dec", "11");
}

//@TEST
_Bool parse_hex()
{
    return test_parse_noerr("hex", "1");
}

//@TEST
_Bool parse_hex_error_empty()
{
    return test_parse_parseerr("hex", "");
}

//@TEST
_Bool parse_hex_error_invalid_character()
{
    return test_parse_parseerr("hex", "a");
}

//@TEST
_Bool parse_hex_error_remaining_characters()
{
    return test_parse_parseerr("hex", "1a");
}

//@TEST
_Bool parse_hex_error_too_many_characters()
{
    return test_parse_parseerr("hex", "11");
}

//@TEST
_Bool parse_bin()
{
    return test_parse_noerr("bin", "1");
}

//@TEST
_Bool parse_bin_error_empty()
{
    return test_parse_parseerr("bin", "");
}

//@TEST
_Bool parse_bin_error_invalid_character()
{
    return test_parse_parseerr("bin", "a");
}

//@TEST
_Bool parse_bin_error_remaining_characters()
{
    return test_parse_parseerr("bin", "1a");
}

//@TEST
_Bool parse_bin_error_too_many_characters()
{
    return test_parse_parseerr("bin", "11");
}

//@TEST
_Bool parse_zeromin_zero()
{
    return test_parse_noerr("zeromin", "");
}

//@TEST
_Bool parse_zeromin_middle()
{
    return test_parse_noerr("zeromin", ".");
}

//@TEST
_Bool parse_zeromin_upper()
{
    return test_parse_noerr("zeromin", "..");
}

//@TEST
_Bool parse_zeromin_error_invalid_character()
{
    return test_parse_parseerr("zeromin", "x");
}

//@TEST
_Bool parse_zeromin_error_too_many()
{
    return test_parse_parseerr("zeromin", "...");
}

//@TEST
_Bool parse_bigrange_lower()
{
    return test_parse_noerr("bigrange", "++++++++++++");
}

//@TEST
_Bool parse_bigrange_middle()
{
    return test_parse_noerr("bigrange", "+++++++++++++");
}

//@TEST
_Bool parse_bigrange_upper()
{
    return test_parse_noerr("bigrange", "+++++++++++++");
}

//@TEST
_Bool parse_bigrange_error_invalid_character()
{
    return test_parse_parseerr("bigrange", "-------------");
}

//@TEST
_Bool parse_bigrange_error_empty()
{
    return test_parse_parseerr("bigrange", "");
}

//@TEST
_Bool parse_bigrange_error_too_few()
{
    return test_parse_parseerr("bigrange", "+++++++++++");
}

//@TEST
_Bool parse_bigrange_error_too_many()
{
    return test_parse_parseerr("bigrange", "+++++++++++++++");
}

//@TEST
_Bool parse_string()
{
    return test_parse_noerr("string", "ab12");
}

//@TEST
_Bool parse_string_error_empty()
{
    return test_parse_parseerr("string", "");
}

//@TEST
_Bool parse_string_error_insufficient()
{
    return test_parse_parseerr("string", "a");
}

//@TEST
_Bool parse_string_error_insufficient2()
{
    return test_parse_parseerr("string", "ab");
}

//@TEST
_Bool parse_string_error_insufficient3()
{
    return test_parse_parseerr("string", "ab1");
}

//@TEST
_Bool parse_string_error_too_many()
{
    return test_parse_parseerr("string", "ab12a");
}

//@TEST
_Bool parse_string_error_repeat()
{
    return test_parse_parseerr("string", "ab12ab12");
}

//@TEST
_Bool parse_string_error_invalid_character_first()
{
    return test_parse_parseerr("string", "xb12");
}

//@TEST
_Bool parse_string_error_invalid_character_middle()
{
    return test_parse_parseerr("string", "ax12");
}

//@TEST
_Bool parse_string_error_invalid_character_last()
{
    return test_parse_parseerr("string", "ab1x");
}

//@TEST
_Bool parse_strrange_lower()
{
    return test_parse_noerr("strrange", "ab12ab12");
}

//@TEST
_Bool parse_strrange_middle()
{
    return test_parse_noerr("strrange", "ab12ab12ab12");
}

//@TEST
_Bool parse_strrange_upper()
{
    return test_parse_noerr("strrange", "ab12ab12ab12ab12");
}

//@TEST
_Bool parse_strrange_invalid_character()
{
    return test_parse_parseerr("strrange", "ax12ab12");
}

//@TEST
_Bool parse_strrange_error_empty()
{
    return test_parse_parseerr("strrange", "");
}

//@TEST
_Bool parse_strrange_error_too_few()
{
    return test_parse_parseerr("strrange", "ab12");
}

//@TEST
_Bool parse_strrange_error_too_many()
{
    return test_parse_parseerr("strrange", "ab12ab12ab12ab12ab12");
}

//@TEST
_Bool parse_strrange_error_incomplete()
{
    return test_parse_parseerr("strrange", "ab12ab12ab12ab");
}

//@TEST
_Bool parse_strzero_empty()
{
    return test_parse_noerr("strzero", "");
}

//@TEST
_Bool parse_strzero_error_nonempty()
{
    return test_parse_parseerr("strzero", "ab12");
}

//@TEST
_Bool parse_strmin_lower()
{
    return test_parse_noerr("strmin", "ab12ab12");
}

//@TEST
_Bool parse_strmin_lots()
{
    return test_parse_noerr("strmin", "ab12ab12ab12ab12ab12ab12ab12ab12ab12");
}

//@TEST
_Bool parse_strmin_error_empty()
{
    return test_parse_parseerr("strmin", "");
}

//@TEST
_Bool parse_strmin_error_incomplete()
{
    return test_parse_parseerr("strmin", "ab12ab12ab12ab12ab12ab12ab1");
}

//@TEST
_Bool parse_strmin_error_invalid_character_first()
{
    return test_parse_parseerr("strmin", "xb12ab12");
}

//@TEST
_Bool parse_strmin_error_invalid_character_middle()
{
    return test_parse_parseerr("strmin", "ab12xb12");
}

//@TEST
_Bool parse_strmin_error_invalid_character_last()
{
    return test_parse_parseerr("strmin", "ab12ab1x");
}

//@TEST
_Bool parse_strmax_zero()
{
    return test_parse_noerr("strmax", "");
}

//@TEST
_Bool parse_strmax_middle()
{
    return test_parse_noerr("strmax", "ab12");
}

//@TEST
_Bool parse_strmax_upper()
{
    return test_parse_noerr("strmax", "ab12ab12");
}

//@TEST
_Bool parse_strmax_error_incomplete()
{
    return test_parse_parseerr("strmax", "ab12ab1");
}

//@TEST
_Bool parse_strmax_error_too_many()
{
    return test_parse_parseerr("strmax", "ab12ab12ab12");
}

//@TEST
_Bool parse_strmax_error_invalid_character_first()
{
    return test_parse_parseerr("strmax", "xb12ab12");
}

//@TEST
_Bool parse_strmax_error_invalid_character_middle()
{
    return test_parse_parseerr("strmax", "ab12xb12");
}

//@TEST
_Bool parse_strmax_error_invalid_character_last()
{
    return test_parse_parseerr("strmax", "ab12ab1x");
}

//@TEST
_Bool parse_strnolim_zero()
{
    return test_parse_noerr("strnolim", "");
}

//@TEST
_Bool parse_strnolim_one()
{
    return test_parse_noerr("strnolim", "ab12");
}

//@TEST
_Bool parse_strnolim_many()
{
    return test_parse_noerr("strnolim", "ab12ab12ab12ab12ab12ab12ab12");
}

//@TEST
_Bool parse_strnolim_error_invalid()
{
    return test_parse_parseerr("strnolim", "x");
}

//@TEST
_Bool parse_strnolim_error_invalid2()
{
    return test_parse_parseerr("strnolim", "ab123");
}

//@TEST
_Bool parse_strnolim_error_invalid3()
{
    return test_parse_parseerr("strnolim", "xb12ab12ab12ab12");
}

//@TEST
_Bool parse_strnolim_error_invalid4()
{
    return test_parse_parseerr("strnolim", "ab12ab12xb12ab12");
}

//@TEST
_Bool parse_strnolim_error_invalid5()
{
    return test_parse_parseerr("strnolim", "ab12ab12ab12ab1x");
}

//@TEST
_Bool parse_strnolim_error_incomplete()
{
    return test_parse_parseerr("strnolim", "ab12ab12ab12ab1");
}

//@TEST
_Bool parse_concat()
{
    return test_parse_noerr("concat", "ab");
}

//@TEST
_Bool parse_concat_error_empty()
{
    return test_parse_parseerr("concat", "");
}

//@TEST
_Bool parse_concat_error_added_space()
{
    return test_parse_parseerr("concat", "a b");
}

//@TEST
_Bool parse_concat_error_invalid_order()
{
    return test_parse_parseerr("concat", "ba");
}

//@TEST
_Bool parse_concat_error_invalid_character()
{
    return test_parse_parseerr("concat", "xb");
}

//@TEST
_Bool parse_concat_error_invalid_character2()
{
    return test_parse_parseerr("concat", "ax");
}

//@TEST
_Bool parse_concat_error_too_many_characters()
{
    return test_parse_parseerr("concat", "abab");
}

//@TEST
_Bool parse_catrange_lower()
{
    return test_parse_noerr("catrange", "abab");
}

//@TEST
_Bool parse_catrange_middle()
{
    return test_parse_noerr("catrange", "ababab");
}

//@TEST
_Bool parse_catrange_upper()
{
    return test_parse_noerr("catrange", "abababab");
}

//@TEST
_Bool parse_catrange_error_empty()
{
    return test_parse_parseerr("catrange", "");
}

//@TEST
_Bool parse_catrange_error_incomplete()
{
    return test_parse_parseerr("catrange", "ababa");
}

//@TEST
_Bool parse_catrange_error_too_few()
{
    return test_parse_parseerr("catrange", "ab");
}

//@TEST
_Bool parse_catrange_error_too_many()
{
    return test_parse_parseerr("catrange", "ababababab");
}

//@TEST
_Bool parse_catrange_error_invalid_character_first()
{
    return test_parse_parseerr("catrange", "xbababab");
}

//@TEST
_Bool parse_catrange_error_invalid_character_middle()
{
    return test_parse_parseerr("catrange", "abaxabab");
}

//@TEST
_Bool parse_catrange_error_invalid_character_last()
{
    return test_parse_parseerr("catrange", "abababax");
}

//@TEST
_Bool parse_catmin_lower()
{
    return test_parse_noerr("catmin", "abab");
}

//@TEST
_Bool parse_catmin_lots()
{
    return test_parse_noerr("catmin", "abababababababababababab");
}

//@TEST
_Bool parse_catmin_error_empty()
{
    return test_parse_parseerr("catmin", "");
}

//@TEST
_Bool parse_catmin_error_incomplete()
{
    return test_parse_parseerr("catmin", "ababababababa");
}

//@TEST
_Bool parse_catmin_error_invalid_character_first()
{
    return test_parse_parseerr("catmin", "xbab");
}

//@TEST
_Bool parse_catmin_error_invalid_character_middle()
{
    return test_parse_parseerr("catmin", "abxb");
}

//@TEST
_Bool parse_catmin_error_invalid_character_last()
{
    return test_parse_parseerr("catmin", "abax");
}

//@TEST
_Bool parse_catmax_zero()
{
    return test_parse_noerr("catmax", "");
}

//@TEST
_Bool parse_catmax_middle()
{
    return test_parse_noerr("catmax", "ab");
}

//@TEST
_Bool parse_catmax_upper()
{
    return test_parse_noerr("catmax", "abab");
}

//@TEST
_Bool parse_catmax_error_incomplete()
{
    return test_parse_parseerr("catmax", "aba");
}

//@TEST
_Bool parse_catmax_error_too_many()
{
    return test_parse_parseerr("catmax", "ababab");
}

//@TEST
_Bool parse_catmax_error_invalid_character_first()
{
    return test_parse_parseerr("catmax", "xbab");
}

//@TEST
_Bool parse_catmax_error_invalid_character_middle()
{
    return test_parse_parseerr("catmax", "abxb");
}

//@TEST
_Bool parse_catmax_error_invalid_character_last()
{
    return test_parse_parseerr("catmax", "abax");
}

//@TEST
_Bool parse_catnolim_zero()
{
    return test_parse_noerr("catnolim", "");
}

//@TEST
_Bool parse_catnolim_one()
{
    return test_parse_noerr("catnolim", "ab");
}

//@TEST
_Bool parse_catnolim_many()
{
    return test_parse_noerr("catnolim", "ababababababab");
}

//@TEST
_Bool parse_catnolim_error_invalid()
{
    return test_parse_parseerr("catnolim", "x");
}

//@TEST
_Bool parse_catnolim_error_invalid2()
{
    return test_parse_parseerr("catnolim", "abc");
}

//@TEST
_Bool parse_catnolim_error_invalid3()
{
    return test_parse_parseerr("catnolim", "xbababab");
}

//@TEST
_Bool parse_catnolim_error_invalid4()
{
    return test_parse_parseerr("catnolim", "ababxbab");
}

//@TEST
_Bool parse_catnolim_error_invalid5()
{
    return test_parse_parseerr("catnolim", "ababababax");
}

//@TEST
_Bool parse_catnolim_error_incomplete()
{
    return test_parse_parseerr("catnolim", "ababababa");
}

//@TEST
_Bool parse_alt_first()
{
    return test_parse_noerr("alt", "a");
}

//@TEST
_Bool parse_alt_middle()
{
    return test_parse_noerr("alt", "b");
}

//@TEST
_Bool parse_alt_last()
{
    return test_parse_noerr("alt", "c");
}

//@TEST
_Bool parse_alt_error_empty()
{
    return test_parse_parseerr("alt", "");
}

//@TEST
_Bool parse_alt_error_invalid_character()
{
    return test_parse_parseerr("alt", "d");
}

//@TEST
_Bool parse_alt_error_too_many_characters()
{
    return test_parse_parseerr("alt", "ab");
}

//@TEST
_Bool parse_alt_error_too_many_characters2()
{
    return test_parse_parseerr("alt", "bc");
}

//@TEST
_Bool parse_alt_error_too_many_characters3()
{
    return test_parse_parseerr("alt", "abc");
}

//@TEST
_Bool parse_altrange_lower_first()
{
    return test_parse_noerr("altrange", "aa");
}

//@TEST
_Bool parse_altrange_lower_middle()
{
    return test_parse_noerr("altrange", "bb");
}

//@TEST
_Bool parse_altrange_lower_last()
{
    return test_parse_noerr("altrange", "cc");
}

//@TEST
_Bool parse_altrange_lower_mixed()
{
    return test_parse_noerr("altrange", "ab");
}

//@TEST
_Bool parse_altrange_lower_mixed2()
{
    return test_parse_noerr("altrange", "bc");
}

//@TEST
_Bool parse_altrange_lower_mixed3()
{
    return test_parse_noerr("altrange", "ca");
}

//@TEST
_Bool parse_altrange_middle_first()
{
    return test_parse_noerr("altrange", "aaa");
}

//@TEST
_Bool parse_altrange_middle_middle()
{
    return test_parse_noerr("altrange", "bbb");
}

//@TEST
_Bool parse_altrange_middle_last()
{
    return test_parse_noerr("altrange", "ccc");
}

//@TEST
_Bool parse_altrange_middle_mixed()
{
    return test_parse_noerr("altrange", "abc");
}

//@TEST
_Bool parse_altrange_middle_mixed2()
{
    return test_parse_noerr("altrange", "acb");
}

//@TEST
_Bool parse_altrange_middle_mixed3()
{
    return test_parse_noerr("altrange", "bca");
}

//@TEST
_Bool parse_altrange_middle_mixed4()
{
    return test_parse_noerr("altrange", "cab");
}

//@TEST
_Bool parse_altrange_middle_mixed5()
{
    return test_parse_noerr("altrange", "ccb");
}

//@TEST
_Bool parse_altrange_upper_first()
{
    return test_parse_noerr("altrange", "aaaa");
}

//@TEST
_Bool parse_altrange_upper_middle()
{
    return test_parse_noerr("altrange", "bbbb");
}

//@TEST
_Bool parse_altrange_upper_last()
{
    return test_parse_noerr("altrange", "cccc");
}

//@TEST
_Bool parse_altrange_upper_mixed()
{
    return test_parse_noerr("altrange", "abca");
}

//@TEST
_Bool parse_altrange_upper_mixed2()
{
    return test_parse_noerr("altrange", "bcab");
}

//@TEST
_Bool parse_altrange_upper_mixed3()
{
    return test_parse_noerr("altrange", "cabc");
}

//@TEST
_Bool parse_altrange_upper_mixed4()
{
    return test_parse_noerr("altrange", "babc");
}

//@TEST
_Bool parse_altrange_upper_mixed5()
{
    return test_parse_noerr("altrange", "ccab");
}

//@TEST
_Bool parse_altrange_error_empty()
{
    return test_parse_parseerr("altrange", "");
}

//@TEST
_Bool parse_altrange_error_too_few()
{
    return test_parse_parseerr("altrange", "a");
}

//@TEST
_Bool parse_altrange_error_too_many()
{
    return test_parse_parseerr("altrange", "aacba");
}

//@TEST
_Bool parse_altrange_error_invalid_character_first()
{
    return test_parse_parseerr("altrange", "xbc");
}

//@TEST
_Bool parse_altrange_error_invalid_character_middle()
{
    return test_parse_parseerr("altrange", "axc");
}

//@TEST
_Bool parse_altrange_error_invalid_character_last()
{
    return test_parse_parseerr("altrange", "abx");
}

//@TEST
_Bool parse_altmin_lower_first()
{
    return test_parse_noerr("altmin", "aa");
}

//@TEST
_Bool parse_altmin_lower_middle()
{
    return test_parse_noerr("altmin", "bb");
}

//@TEST
_Bool parse_altmin_lower_last()
{
    return test_parse_noerr("altmin", "cc");
}

//@TEST
_Bool parse_altmin_lower_mixed()
{
    return test_parse_noerr("altmin", "ab");
}

//@TEST
_Bool parse_altmin_lower_mixed2()
{
    return test_parse_noerr("altmin", "bc");
}

//@TEST
_Bool parse_altmin_lower_mixed3()
{
    return test_parse_noerr("altmin", "ca");
}

//@TEST
_Bool parse_altmin_lots_first()
{
    return test_parse_noerr("altmin", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
}

//@TEST
_Bool parse_altmin_lots_middle()
{
    return test_parse_noerr("altmin", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
}

//@TEST
_Bool parse_altmin_lots_last()
{
    return test_parse_noerr("altmin", "cccccccccccccccccccccccccccccc");
}

//@TEST
_Bool parse_altmin_lots_mixed()
{
    return test_parse_noerr("altmin", "baccbaabbbccabababcbababcbbcccaaabcbabc");
}

//@TEST
_Bool parse_altmin_error_empty()
{
    return test_parse_parseerr("altmin", "");
}

//@TEST
_Bool parse_altmin_error_invalid_character_first()
{
    return test_parse_parseerr("altmin", "xbca");
}

//@TEST
_Bool parse_altmin_error_invalid_character_middle()
{
    return test_parse_parseerr("altmin", "abxb");
}

//@TEST
_Bool parse_altmin_error_invalid_character_last()
{
    return test_parse_parseerr("altmin", "cbax");
}

//@TEST
_Bool parse_altmax_zero()
{
    return test_parse_noerr("altmax", "");
}

//@TEST
_Bool parse_altmax_middle_first()
{
    return test_parse_noerr("altmax", "a");
}

//@TEST
_Bool parse_altmax_middle_middle()
{
    return test_parse_noerr("altmax", "b");
}

//@TEST
_Bool parse_altmax_middle_last()
{
    return test_parse_noerr("altmax", "c");
}

//@TEST
_Bool parse_altmax_upper_first()
{
    return test_parse_noerr("altmax", "aa");
}

//@TEST
_Bool parse_altmax_upper_middle()
{
    return test_parse_noerr("altmax", "bb");
}

//@TEST
_Bool parse_altmax_upper_last()
{
    return test_parse_noerr("altmax", "cc");
}

//@TEST
_Bool parse_altmax_upper_mixed()
{
    return test_parse_noerr("altmax", "ab");
}

//@TEST
_Bool parse_altmax_upper_mixed2()
{
    return test_parse_noerr("altmax", "bc");
}

//@TEST
_Bool parse_altmax_upper_mixed3()
{
    return test_parse_noerr("altmax", "ca");
}

//@TEST
_Bool parse_altmax_error_too_many()
{
    return test_parse_parseerr("altmax", "aaa");
}

//@TEST
_Bool parse_altmax_error_invalid_character_first()
{
    return test_parse_parseerr("altmax", "xa");
}

//@TEST
_Bool parse_altmax_error_invalid_character_last()
{
    return test_parse_parseerr("altmax", "ax");
}

//@TEST
_Bool parse_altnolim_zero()
{
    return test_parse_noerr("altnolim", "");
}

//@TEST
_Bool parse_altnolim_first_one()
{
    return test_parse_noerr("altnolim", "a");
}

//@TEST
_Bool parse_altnolim_first_many()
{
    return test_parse_noerr("altnolim", "aaaaaaaaaaaaaaaaaaaaaaaa");
}

//@TEST
_Bool parse_altnolim_middle_one()
{
    return test_parse_noerr("altnolim", "b");
}

//@TEST
_Bool parse_altnolim_middle_many()
{
    return test_parse_noerr("altnolim", "bbbbbbbbbbbbbbbbbbbbbbbb");
}

//@TEST
_Bool parse_altnolim_last_one()
{
    return test_parse_noerr("altnolim", "c");
}

//@TEST
_Bool parse_altnolim_last_many()
{
    return test_parse_noerr("altnolim", "cccccccccccccccccccccccc");
}

//@TEST
_Bool parse_altnolim_many_mixed()
{
    return test_parse_noerr("altnolim", "acbbbcabaacbcbbaacbaccca");
}

//@TEST
_Bool parse_altnolim_error_invalid_character()
{
    return test_parse_parseerr("altnolim", "x");
}

//@TEST
_Bool parse_altnolim_error_invalid_character_first()
{
    return test_parse_parseerr("altnolim", "xaabcbcbabacbcabcbcbacbb");
}

//@TEST
_Bool parse_altnolim_error_invalid_character_middle()
{
    return test_parse_parseerr("altnolim", "caabcbcbabxcbcabcbcbacbb");
}

//@TEST
_Bool parse_altnolim_error_invalid_character_last()
{
    return test_parse_parseerr("altnolim", "caabcbcbabacbcabcbcbacbx");
}

//@TEST
_Bool parse_catalt_alt1()
{
    return test_parse_noerr("catalt", "ab");
}

//@TEST
_Bool parse_catalt_alt2()
{
    return test_parse_noerr("catalt", "12");
}

//@TEST
_Bool parse_catalt_error_empty()
{
    return test_parse_parseerr("catalt", "");
}

//@TEST
_Bool parse_catalt_error_mixed()
{
    return test_parse_parseerr("catalt", "a1");
}

//@TEST
_Bool parse_catalt_error_mixed2()
{
    return test_parse_parseerr("catalt", "a2");
}

//@TEST
_Bool parse_catalt_error_mixed3()
{
    return test_parse_parseerr("catalt", "b1");
}

//@TEST
_Bool parse_catalt_error_mixed4()
{
    return test_parse_parseerr("catalt", "b2");
}

//@TEST
_Bool parse_catalt_error_combined()
{
    return test_parse_parseerr("catalt", "ab12");
}

//@TEST
_Bool parse_catalt_error_invalid_character()
{
    return test_parse_parseerr("catalt", "x1");
}

//@TEST
_Bool parse_catalt_error_invalid_character2()
{
    return test_parse_parseerr("catalt", "x2");
}

//@TEST
_Bool parse_catalt_error_invalid_character3()
{
    return test_parse_parseerr("catalt", "ax");
}

//@TEST
_Bool parse_catalt_error_invalid_character4()
{
    return test_parse_parseerr("catalt", "bx");
}

//@TEST
_Bool parse_catalt_error_duplicate()
{
    return test_parse_parseerr("catalt", "abab");
}

//@TEST
_Bool parse_catalt_error_duplicate2()
{
    return test_parse_parseerr("catalt", "1212");
}

//@TEST
_Bool parse_altcat_alt1()
{
    return test_parse_noerr("altcat", "a1b");
}

//@TEST
_Bool parse_altcat_alt2()
{
    return test_parse_noerr("altcat", "a2b");
}

//@TEST
_Bool parse_altcat_error_empty()
{
    return test_parse_parseerr("altcat", "");
}

//@TEST
_Bool parse_altcat_error_missing_alt()
{
    return test_parse_parseerr("altcat", "ab");
}

//@TEST
_Bool parse_altcat_error_invalid_alt()
{
    return test_parse_parseerr("altcat", "a3b");
}

//@TEST
_Bool parse_altcat_error_invalid_cat()
{
    return test_parse_parseerr("altcat", "x1b");
}

//@TEST
_Bool parse_altcat_error_invalid_cat2()
{
    return test_parse_parseerr("altcat", "a1x");
}

//@TEST
_Bool parse_altcat_error_missing_cat()
{
    return test_parse_parseerr("altcat", "a1");
}

//@TEST
_Bool parse_altcat_error_missing_cat2()
{
    return test_parse_parseerr("altcat", "a2");
}

//@TEST
_Bool parse_altcat_error_missing_cat3()
{
    return test_parse_parseerr("altcat", "1b");
}

//@TEST
_Bool parse_altcat_error_missing_cat4()
{
    return test_parse_parseerr("altcat", "2b");
}

//@TEST
_Bool parse_option()
{
    return test_parse_noerr("option", "abc");
}

//@TEST
_Bool parse_option_empty()
{
    return test_parse_noerr("option", "");
}

//@TEST
_Bool parse_option_error_invalid()
{
    return test_parse_parseerr("option", "ab");
}

//@TEST
_Bool parse_option_error_invalid2()
{
    return test_parse_parseerr("option", "abcd");
}

//@TEST
_Bool parse_optrange_zero()
{
    return test_parse_noerr("optrange", "");
}

//@TEST
_Bool parse_optrange_one()
{
    return test_parse_noerr("optrange", "abc");
}

//@TEST
_Bool parse_optrange_min()
{
    return test_parse_noerr("optrange", "abcabc");
}

//@TEST
_Bool parse_optrange_middle()
{
    return test_parse_noerr("optrange", "abcabcabc");
}

//@TEST
_Bool parse_optrange_max()
{
    return test_parse_noerr("optrange", "abcabcabcabc");
}

//@TEST
_Bool parse_optrange_error_too_many()
{
    return test_parse_parseerr("optrange", "abcabcabcabcabc");
}

//@TEST
_Bool parse_optrange_error_invalid()
{
    return test_parse_parseerr("optrange", "x");
}

//@TEST
_Bool parse_optrange_error_invalid2()
{
    return test_parse_parseerr("optrange", "axc");
}

//@TEST
_Bool parse_optrange_error_invalid3()
{
    return test_parse_parseerr("optrange", "abx");
}

//@TEST
_Bool parse_optrange_error_invalid4()
{
    return test_parse_parseerr("optrange", "abcaxc");
}

//@TEST
_Bool parse_optrange_error_invalid5()
{
    return test_parse_parseerr("optrange", "abcabcabcx");
}

//@TEST
_Bool parse_optrange_error_invalid6()
{
    return test_parse_parseerr("optrange", "xabcabc");
}

//@TEST
_Bool parse_optmin_empty()
{
    return test_parse_noerr("optmin", "");
}

//@TEST
_Bool parse_optmin_one()
{
    return test_parse_noerr("optmin", "abc");
}

//@TEST
_Bool parse_optmin_min()
{
    return test_parse_noerr("optmin", "abcabc");
}

//@TEST
_Bool parse_optmin_some()
{
    return test_parse_noerr("optmin", "abcabcabc");
}

//@TEST
_Bool parse_optmin_lots()
{
    return test_parse_noerr("optmin", "abcabcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_optmin_error_invalid()
{
    return test_parse_parseerr("optmin", "x");
}

//@TEST
_Bool parse_optmin_error_invalid2()
{
    return test_parse_parseerr("optmin", "abcx");
}

//@TEST
_Bool parse_optmin_error_invalid3()
{
    return test_parse_parseerr("optmin", "xabc");
}

//@TEST
_Bool parse_optmin_error_invalid4()
{
    return test_parse_parseerr("optmin", "abx");
}

//@TEST
_Bool parse_optmin_error_invalid5()
{
    return test_parse_parseerr("optmin", "abcab");
}

//@TEST
_Bool parse_optmin_error_invalid6()
{
    return test_parse_parseerr("optmin", "abcabcabcaxc");
}

//@TEST
_Bool parse_optmin_error_invalid7()
{
    return test_parse_parseerr("optmin", "abcabcabcabx");
}

//@TEST
_Bool parse_optmin_error_invalid8()
{
    return test_parse_parseerr("optmin", "abcabcabcxbc");
}

//@TEST
_Bool parse_optmin_error_invalid9()
{
    return test_parse_parseerr("optmin", "abcabcabcabcabcab");
}

//@TEST
_Bool parse_optmax_empty()
{
    return test_parse_noerr("optmax", "");
}

//@TEST
_Bool parse_optmax_one()
{
    return test_parse_noerr("optmax", "abc");
}

//@TEST
_Bool parse_optmax_two()
{
    return test_parse_noerr("optmax", "abcabc");
}

//@TEST
_Bool parse_optmax_three()
{
    return test_parse_noerr("optmax", "abcabcabc");
}

//@TEST
_Bool parse_optmax_max()
{
    return test_parse_noerr("optmax", "abcabcabcabc");
}

//@TEST
_Bool parse_optmax_error_too_many_one()
{
    return test_parse_parseerr("optmax", "abcabcabcabcabc");
}

//@TEST
_Bool parse_optmax_error_too_many_double()
{
    return test_parse_parseerr("optmax", "abcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_optmax_error_too_many_lots()
{
    return test_parse_parseerr("optmax", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_optmax_error_invalid()
{
    return test_parse_parseerr("optmax", "x");
}

//@TEST
_Bool parse_optmax_error_invalid2()
{
    return test_parse_parseerr("optmax", "abcx");
}

//@TEST
_Bool parse_optmax_error_invalid3()
{
    return test_parse_parseerr("optmax", "xabc");
}

//@TEST
_Bool parse_optmax_error_invalid4()
{
    return test_parse_parseerr("optmax", "abx");
}

//@TEST
_Bool parse_optmax_error_invalid5()
{
    return test_parse_parseerr("optmax", "abcab");
}

//@TEST
_Bool parse_optmax_error_invalid6()
{
    return test_parse_parseerr("optmax", "abcabcabcaxc");
}

//@TEST
_Bool parse_optmax_error_invalid7()
{
    return test_parse_parseerr("optmax", "abcabcabcabx");
}

//@TEST
_Bool parse_optmax_error_invalid8()
{
    return test_parse_parseerr("optmax", "abcabcabcxbc");
}

//@TEST
_Bool parse_optmax_error_invalid9()
{
    return test_parse_parseerr("optmax", "abcabcabcabcabcab");
}

//@TEST
_Bool parse_optmax_error_invalid10()
{
    return test_parse_parseerr("optmax", "abcabcabcabca");
}

//@TEST
_Bool parse_optmax_error_invalid11()
{
    return test_parse_parseerr("optmax", "abcabcabcabcx");
}

//@TEST
_Bool parse_optnolim_one()
{
    return test_parse_noerr("optnolim", "abc");
}

//@TEST
_Bool parse_optnolim_empty()
{
    return test_parse_noerr("optnolim", "");
}

//@TEST
_Bool parse_optnolim_many()
{
    return test_parse_noerr("optnolim", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_optnolim_error_invalid()
{
    return test_parse_parseerr("optnolim", "ab");
}

//@TEST
_Bool parse_optnolim_error_invalid2()
{
    return test_parse_parseerr("optnolim", "abcd");
}

//@TEST
_Bool parse_optnolim_error_invalid3()
{
    return test_parse_parseerr("optnolim", "xbc");
}

//@TEST
_Bool parse_optnolim_error_invalid4()
{
    return test_parse_parseerr("optnolim", "abcabcabcaxcabc");
}

//@TEST
_Bool parse_optnolim_error_invalid5()
{
    return test_parse_parseerr("optnolim", "abcabcabcabcabcx");
}

//@TEST
_Bool parse_optnolim_error_incomplete()
{
    return test_parse_parseerr("optnolim", "abcabcabcabcabcabcab");
}

//@TEST
_Bool parse_optgroup()
{
    return test_parse_noerr("optgroup", "abc");
}

//@TEST
_Bool parse_optgroup_empty()
{
    return test_parse_noerr("optgroup", "");
}

//@TEST
_Bool parse_optgroup_many()
{
    return test_parse_noerr("optgroup", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_optgroup_error_invalid()
{
    return test_parse_parseerr("optgroup", "ab");
}

//@TEST
_Bool parse_optgroup_error_invalid2()
{
    return test_parse_parseerr("optgroup", "abcd");
}

//@TEST
_Bool parse_optgroup_error_invalid3()
{
    return test_parse_parseerr("optgroup", "xbc");
}

//@TEST
_Bool parse_optgroup_error_invalid4()
{
    return test_parse_parseerr("optgroup", "abcabcabcaxcabc");
}

//@TEST
_Bool parse_optgroup_error_invalid5()
{
    return test_parse_parseerr("optgroup", "abcabcabcabcabcx");
}

//@TEST
_Bool parse_optgroup_error_incomplete()
{
    return test_parse_parseerr("optgroup", "abcabcabcabcabcabcab");
}

//@TEST
_Bool parse_nolimnest()
{
    return test_parse_noerr("nolimnest", "abc");
}

//@TEST
_Bool parse_nolimnest_empty()
{
    return test_parse_noerr("nolimnest", "");
}

//@TEST
_Bool parse_nolimnest_many()
{
    return test_parse_noerr("nolimnest", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_nolimnest_error_invalid()
{
    return test_parse_parseerr("nolimnest", "ab");
}

//@TEST
_Bool parse_nolimnest_error_invalid2()
{
    return test_parse_parseerr("nolimnest", "abcd");
}

//@TEST
_Bool parse_nolimnest_error_invalid3()
{
    return test_parse_parseerr("nolimnest", "xbc");
}

//@TEST
_Bool parse_nolimnest_error_invalid4()
{
    return test_parse_parseerr("nolimnest", "abcabcabcaxcabc");
}

//@TEST
_Bool parse_nolimnest_error_invalid5()
{
    return test_parse_parseerr("nolimnest", "abcabcabcabcabcx");
}

//@TEST
_Bool parse_nolimnest_error_incomplete()
{
    return test_parse_parseerr("nolimnest", "abcabcabcabcabcabcab");
}

//@TEST
_Bool parse_nolimnestopt()
{
    return test_parse_noerr("nolimnestopt", "abc");
}

//@TEST
_Bool parse_nolimnestopt_empty()
{
    return test_parse_noerr("nolimnestopt", "");
}

//@TEST
_Bool parse_nolimnestopt_many()
{
    return test_parse_noerr("nolimnestopt", "abcabcabcabcabcabcabcabcabcabcabc");
}

//@TEST
_Bool parse_nolimnestopt_error_invalid()
{
    return test_parse_parseerr("nolimnestopt", "ab");
}

//@TEST
_Bool parse_nolimnestopt_error_invalid2()
{
    return test_parse_parseerr("nolimnestopt", "abcd");
}

//@TEST
_Bool parse_nolimnestopt_error_invalid3()
{
    return test_parse_parseerr("nolimnestopt", "xbc");
}

//@TEST
_Bool parse_nolimnestopt_error_invalid4()
{
    return test_parse_parseerr("nolimnestopt", "abcabcabcaxcabc");
}

//@TEST
_Bool parse_nolimnestopt_error_invalid5()
{
    return test_parse_parseerr("nolimnestopt", "abcabcabcabcabcx");
}

//@TEST
_Bool parse_nolimnestopt_error_incomplete()
{
    return test_parse_parseerr("nolimnestopt", "abcabcabcabcabcabcab");
}

//@TEST
_Bool parse_decrange_first()
{
    return test_parse_noerr("decrange", "a");
}

//@TEST
_Bool parse_decrange_middle()
{
    return test_parse_noerr("decrange", "b");
}

//@TEST
_Bool parse_decrange_last()
{
    return test_parse_noerr("decrange", "c");
}

//@TEST
_Bool parse_decrange_error_empty()
{
    return test_parse_parseerr("decrange", "");
}

//@TEST
_Bool parse_decrange_error_all()
{
    return test_parse_parseerr("decrange", "abc");
}

//@TEST
_Bool parse_decrange_error_some()
{
    return test_parse_parseerr("decrange", "ab");
}

//@TEST
_Bool parse_decrange_error_some2()
{
    return test_parse_parseerr("decrange", "bc");
}

//@TEST
_Bool parse_decrange_error_some3()
{
    return test_parse_parseerr("decrange", "ca");
}

//@TEST
_Bool parse_decrange_error_invalid()
{
    return test_parse_parseerr("decrange", "x");
}

//@TEST
_Bool parse_decrange_error_invalid2()
{
    return test_parse_parseerr("decrange", "ax");
}

//@TEST
_Bool parse_hexrange_first()
{
    return test_parse_noerr("hexrange", "a");
}

//@TEST
_Bool parse_hexrange_middle()
{
    return test_parse_noerr("hexrange", "b");
}

//@TEST
_Bool parse_hexrange_last()
{
    return test_parse_noerr("hexrange", "c");
}

//@TEST
_Bool parse_hexrange_error_empty()
{
    return test_parse_parseerr("hexrange", "");
}

//@TEST
_Bool parse_hexrange_error_all()
{
    return test_parse_parseerr("hexrange", "abc");
}

//@TEST
_Bool parse_hexrange_error_some()
{
    return test_parse_parseerr("hexrange", "ab");
}

//@TEST
_Bool parse_hexrange_error_some2()
{
    return test_parse_parseerr("hexrange", "bc");
}

//@TEST
_Bool parse_hexrange_error_some3()
{
    return test_parse_parseerr("hexrange", "ca");
}

//@TEST
_Bool parse_hexrange_error_invalid()
{
    return test_parse_parseerr("hexrange", "x");
}

//@TEST
_Bool parse_hexrange_error_invalid2()
{
    return test_parse_parseerr("hexrange", "ax");
}

//@TEST
_Bool parse_binrange_first()
{
    return test_parse_noerr("binrange", "a");
}

//@TEST
_Bool parse_binrange_middle()
{
    return test_parse_noerr("binrange", "b");
}

//@TEST
_Bool parse_binrange_last()
{
    return test_parse_noerr("binrange", "c");
}

//@TEST
_Bool parse_binrange_error_empty()
{
    return test_parse_parseerr("binrange", "");
}

//@TEST
_Bool parse_binrange_error_all()
{
    return test_parse_parseerr("binrange", "abc");
}

//@TEST
_Bool parse_binrange_error_some()
{
    return test_parse_parseerr("binrange", "ab");
}

//@TEST
_Bool parse_binrange_error_some2()
{
    return test_parse_parseerr("binrange", "bc");
}

//@TEST
_Bool parse_binrange_error_some3()
{
    return test_parse_parseerr("binrange", "ca");
}

//@TEST
_Bool parse_binrange_error_invalid()
{
    return test_parse_parseerr("binrange", "x");
}

//@TEST
_Bool parse_binrange_error_invalid2()
{
    return test_parse_parseerr("binrange", "ax");
}

//@TEST
_Bool parse_deccat()
{
    return test_parse_noerr("deccat", "abc");
}

//@TEST
_Bool parse_deccat_error_empty()
{
    return test_parse_parseerr("deccat", "");
}

//@TEST
_Bool parse_deccat_error_insufficient()
{
    return test_parse_parseerr("deccat", "a");
}

//@TEST
_Bool parse_deccat_error_insufficient2()
{
    return test_parse_parseerr("deccat", "ab");
}

//@TEST
_Bool parse_deccat_error_too_many()
{
    return test_parse_parseerr("deccat", "abca");
}

//@TEST
_Bool parse_deccat_error_repeat()
{
    return test_parse_parseerr("deccat", "abcabc");
}

//@TEST
_Bool parse_deccat_error_invalid_character_first()
{
    return test_parse_parseerr("deccat", "xbc");
}

//@TEST
_Bool parse_deccat_error_invalid_character_middle()
{
    return test_parse_parseerr("deccat", "axc");
}

//@TEST
_Bool parse_deccat_error_invalid_character_last()
{
    return test_parse_parseerr("deccat", "abx");
}

//@TEST
_Bool parse_hexcat()
{
    return test_parse_noerr("hexcat", "abc");
}

//@TEST
_Bool parse_hexcat_error_empty()
{
    return test_parse_parseerr("hexcat", "");
}

//@TEST
_Bool parse_hexcat_error_insufficient()
{
    return test_parse_parseerr("hexcat", "a");
}

//@TEST
_Bool parse_hexcat_error_insufficient2()
{
    return test_parse_parseerr("hexcat", "ab");
}

//@TEST
_Bool parse_hexcat_error_too_many()
{
    return test_parse_parseerr("hexcat", "abca");
}

//@TEST
_Bool parse_hexcat_error_repeat()
{
    return test_parse_parseerr("hexcat", "abcabc");
}

//@TEST
_Bool parse_hexcat_error_invalid_character_first()
{
    return test_parse_parseerr("hexcat", "xbc");
}

//@TEST
_Bool parse_hexcat_error_invalid_character_middle()
{
    return test_parse_parseerr("hexcat", "axc");
}

//@TEST
_Bool parse_hexcat_error_invalid_character_last()
{
    return test_parse_parseerr("hexcat", "abx");
}

//@TEST
_Bool parse_bincat()
{
    return test_parse_noerr("bincat", "abc");
}

//@TEST
_Bool parse_bincat_error_empty()
{
    return test_parse_parseerr("bincat", "");
}

//@TEST
_Bool parse_bincat_error_insufficient()
{
    return test_parse_parseerr("bincat", "a");
}

//@TEST
_Bool parse_bincat_error_insufficient2()
{
    return test_parse_parseerr("bincat", "ab");
}

//@TEST
_Bool parse_bincat_error_too_many()
{
    return test_parse_parseerr("bincat", "abca");
}

//@TEST
_Bool parse_bincat_error_repeat()
{
    return test_parse_parseerr("bincat", "abcabc");
}

//@TEST
_Bool parse_bincat_error_invalid_character_first()
{
    return test_parse_parseerr("bincat", "xbc");
}

//@TEST
_Bool parse_bincat_error_invalid_character_middle()
{
    return test_parse_parseerr("bincat", "axc");
}

//@TEST
_Bool parse_bincat_error_invalid_character_last()
{
    return test_parse_parseerr("bincat", "abx");
}

//@TEST
_Bool parse_prosequot()
{
    return test_parse_noerr("prosequot", "\"");
}

//@TEST
_Bool parse_prosequot_error_empty()
{
    return test_parse_parseerr("prosequot", "");
}

//@TEST
_Bool parse_prosequot_error_invalid_character()
{
    return test_parse_parseerr("prosequot", "a");
}

//@TEST
_Bool parse_prosequot_error_remaining_characters()
{
    return test_parse_parseerr("prosequot", "\"a");
}

//@TEST
_Bool parse_prosequot_error_too_many_characters()
{
    return test_parse_parseerr("prosequot", "\"\"");
}

//@TEST
_Bool parse_quotprose()
{
    return test_parse_noerr("quotprose", "<");
}

//@TEST
_Bool parse_quotprose_error_empty()
{
    return test_parse_parseerr("quotprose", "");
}

//@TEST
_Bool parse_quotprose_error_invalid_character()
{
    return test_parse_parseerr("quotprose", "a");
}

//@TEST
_Bool parse_quotprose_error_remaining_characters()
{
    return test_parse_parseerr("quotprose", "<a");
}

//@TEST
_Bool parse_quotprose_error_too_many_characters()
{
    return test_parse_parseerr("quotprose", "<<");
}

//@TEST
_Bool parse_badname_error()
{
    return test_parse_dne("badname", "abc");
}
//@TEST
_Bool parse_name_lower()
{
    return test_parse_noerr("name", "ab12ab12");
}

//@TEST
_Bool parse_name_middle()
{
    return test_parse_noerr("name", "ab12ab12ab12");
}

//@TEST
_Bool parse_name_upper()
{
    return test_parse_noerr("name", "ab12ab12ab12ab12");
}

//@TEST
_Bool parse_name_invalid_character()
{
    return test_parse_parseerr("name", "ax12ab12");
}

//@TEST
_Bool parse_name_error_empty()
{
    return test_parse_parseerr("name", "");
}

//@TEST
_Bool parse_name_error_too_few()
{
    return test_parse_parseerr("name", "ab12");
}

//@TEST
_Bool parse_name_error_too_many()
{
    return test_parse_parseerr("name", "ab12ab12ab12ab12ab12");
}

//@TEST
_Bool parse_name_error_incomplete()
{
    return test_parse_parseerr("name", "ab12ab12ab12ab");
}

//@TEST
_Bool parse_namealt_opt1_first()
{
    return test_parse_noerr("namealt", "a");
}

//@TEST
_Bool parse_namealt_opt1_middle()
{
    return test_parse_noerr("namealt", "b");
}

//@TEST
_Bool parse_namealt_opt1_last()
{
    return test_parse_noerr("namealt", "c");
}

//@TEST
_Bool parse_namealt_opt1_error_empty()
{
    return test_parse_parseerr("namealt", "");
}

//@TEST
_Bool parse_namealt_opt1_error_all()
{
    return test_parse_parseerr("namealt", "abc");
}

//@TEST
_Bool parse_namealt_opt1_error_some()
{
    return test_parse_parseerr("namealt", "ab");
}

//@TEST
_Bool parse_namealt_opt1_error_some2()
{
    return test_parse_parseerr("namealt", "bc");
}

//@TEST
_Bool parse_namealt_opt1_error_some3()
{
    return test_parse_parseerr("namealt", "ca");
}

//@TEST
_Bool parse_namealt_opt1_error_invalid()
{
    return test_parse_parseerr("namealt", "x");
}

//@TEST
_Bool parse_namealt_opt1_error_invalid2()
{
    return test_parse_parseerr("namealt", "ax");
}

//@TEST
_Bool parse_namealt_opt2_lower()
{
    return test_parse_noerr("namealt", "##");
}

//@TEST
_Bool parse_namealt_opt2_lots()
{
    return test_parse_noerr("namealt", "#####################################");
}

//@TEST
_Bool parse_namealt_opt2_error_empty()
{
    return test_parse_parseerr("namealt", "");
}

//@TEST
_Bool parse_namealt_opt2_error_invalid_character()
{
    return test_parse_parseerr("namealt", "xx");
}

//@TEST
_Bool parse_namealt_opt2_error_too_few()
{
    return test_parse_parseerr("namealt", "#");
}

//@TEST
_Bool parse_namealt_error_mixed()
{
    return test_parse_parseerr("namealt", "a##");
}

//@TEST
_Bool parse_namealt_error_mixed2()
{
    return test_parse_parseerr("namealt", "b##");
}

//@TEST
_Bool parse_namealt_error_mixed3()
{
    return test_parse_parseerr("namealt", "c##");
}

//@TEST
_Bool parse_namealt_error_mixed4()
{
    return test_parse_parseerr("namealt", "##a");
}

//@TEST
_Bool parse_namealt_error_mixed5()
{
    return test_parse_parseerr("namealt", "##b");
}

//@TEST
_Bool parse_namealt_error_mixed6()
{
    return test_parse_parseerr("namealt", "##c");
}

//@TEST
_Bool parse_namecat_first_lower()
{
    return test_parse_noerr("namecat", "a##");
}

//@TEST
_Bool parse_namecat_first_middle()
{
    return test_parse_noerr("namecat", "a###");
}

//@TEST
_Bool parse_namecat_first_upper()
{
    return test_parse_noerr("namecat", "a####");
}

//@TEST
_Bool parse_namecat_middle_lower()
{
    return test_parse_noerr("namecat", "b##");
}

//@TEST
_Bool parse_namecat_middle_middle()
{
    return test_parse_noerr("namecat", "b###");
}

//@TEST
_Bool parse_namecat_middle_upper()
{
    return test_parse_noerr("namecat", "b####");
}

//@TEST
_Bool parse_namecat_upper_lower()
{
    return test_parse_noerr("namecat", "c##");
}

//@TEST
_Bool parse_namecat_upper_middle()
{
    return test_parse_noerr("namecat", "c###");
}

//@TEST
_Bool parse_namecat_upper_upper()
{
    return test_parse_noerr("namecat", "c####");
}

//@TEST
_Bool parse_namecat_error_empty()
{
    return test_parse_parseerr("namecat", "");
}

//@TEST
_Bool parse_namecat_error_invalid()
{
    return test_parse_parseerr("namecat", "123");
}

//@TEST
_Bool parse_namecat_error_group1_invalid()
{
    return test_parse_parseerr("namecat", "d##");
}

//@TEST
_Bool parse_namecat_error_group2_invalid()
{
    return test_parse_parseerr("namecat", "a#");
}

//@TEST
_Bool parse_namecat_error_group1_missing()
{
    return test_parse_parseerr("namecat", "##");
}

//@TEST
_Bool parse_namecat_error_group2_missing()
{
    return test_parse_parseerr("namecat", "a");
}

//@TEST
_Bool parse_namenested_opt1_first()
{
    return test_parse_noerr("namenested", "a");
}

//@TEST
_Bool parse_namenested_opt1_middle()
{
    return test_parse_noerr("namenested", "b");
}

//@TEST
_Bool parse_namenested_opt1_last()
{
    return test_parse_noerr("namenested", "c");
}

//@TEST
_Bool parse_namenested_opt1_error_empty()
{
    return test_parse_parseerr("namenested", "");
}

//@TEST
_Bool parse_namenested_opt1_error_all()
{
    return test_parse_parseerr("namenested", "abc");
}

//@TEST
_Bool parse_namenested_opt1_error_some()
{
    return test_parse_parseerr("namenested", "ab");
}

//@TEST
_Bool parse_namenested_opt1_error_some2()
{
    return test_parse_parseerr("namenested", "bc");
}

//@TEST
_Bool parse_namenested_opt1_error_some3()
{
    return test_parse_parseerr("namenested", "ca");
}

//@TEST
_Bool parse_namenested_opt1_error_invalid()
{
    return test_parse_parseerr("namenested", "x");
}

//@TEST
_Bool parse_namenested_opt1_error_invalid2()
{
    return test_parse_parseerr("namenested", "ax");
}

//@TEST
_Bool parse_namenested_opt2_lower()
{
    return test_parse_noerr("namenested", "##");
}

//@TEST
_Bool parse_namenested_opt2_lots()
{
    return test_parse_noerr("namenested", "################################");
}

//@TEST
_Bool parse_namenested_opt2_error_empty()
{
    return test_parse_parseerr("namenested", "");
}

//@TEST
_Bool parse_namenested_opt2_error_invalid_character()
{
    return test_parse_parseerr("namenested", "xx");
}

//@TEST
_Bool parse_namenested_opt2_error_too_few()
{
    return test_parse_parseerr("namenested", "#");
}

//@TEST
_Bool parse_namenested_error_mixed()
{
    return test_parse_parseerr("namenested", "a##");
}

//@TEST
_Bool parse_namenested_error_mixed2()
{
    return test_parse_parseerr("namenested", "b##");
}

//@TEST
_Bool parse_namenested_error_mixed3()
{
    return test_parse_parseerr("namenested", "c##");
}

//@TEST
_Bool parse_namenested_error_mixed4()
{
    return test_parse_parseerr("namenested", "##a");
}

//@TEST
_Bool parse_namenested_error_mixed5()
{
    return test_parse_parseerr("namenested", "##b");
}

//@TEST
_Bool parse_namenested_error_mixed6()
{
    return test_parse_parseerr("namenested", "##c");
}

//@TEST
_Bool parse_emptyname_empty()
{
    return test_parse_noerr("emptyname", "");
}

//@TEST
_Bool parse_emptyname_error_nonempty()
{
    return test_parse_parseerr("emptyname", "ab12");
}

//@TEST
_Bool parse_space()
{
    return test_parse_noerr("space", " ");
}

//@TEST
_Bool parse_space_error_empty()
{
    return test_parse_parseerr("space", "");
}

//@TEST
_Bool parse_htab()
{
    return test_parse_noerr("htab", "\t");
}

//@TEST
_Bool parse_htab_error_empty()
{
    return test_parse_parseerr("htab", "");
}

//@TEST
_Bool parse_altoverlap1_opt1_one()
{
    return test_parse_noerr("altoverlap1", "a");
}

//@TEST
_Bool parse_altoverlap1_opt1_many()
{
    return test_parse_noerr("altoverlap1", "aaaaaaaaaaaaaaaa");
}

//@TEST
_Bool parse_altoverlap1_opt2_none()
{
    return test_parse_noerr("altoverlap1", "#");
}

//@TEST
_Bool parse_altoverlap1_opt2_one()
{
    return test_parse_noerr("altoverlap1", "a#");
}

//@TEST
_Bool parse_altoverlap1_opt2_many()
{
    return test_parse_noerr("altoverlap1", "aaaaaaaaaaaaaaaaa#");
}

//@TEST
_Bool parse_altoverlap1_error_empty()
{
    return test_parse_parseerr("altoverlap1", "");
}

//@TEST
_Bool parse_altoverlap1_error_opt2_too_many()
{
    return test_parse_parseerr("altoverlap1", "##");
}

//@TEST
_Bool parse_altoverlap1_error_opt2_too_many2()
{
    return test_parse_parseerr("altoverlap1", "a##");
}

//@TEST
_Bool parse_altoverlap1_error_opt2_too_many3()
{
    return test_parse_parseerr("altoverlap1", "aaaaaaaaa##");
}

//@TEST
_Bool parse_altoverlap1_error_invalid()
{
    return test_parse_parseerr("altoverlap1", "x");
}

//@TEST
_Bool parse_altoverlap1_error_invalid2()
{
    return test_parse_parseerr("altoverlap1", "x#");
}

//@TEST
_Bool parse_altoverlap2_opt2_one()
{
    return test_parse_noerr("altoverlap2", "a");
}

//@TEST
_Bool parse_altoverlap2_opt2_many()
{
    return test_parse_noerr("altoverlap2", "aaaaaaaaaaaaaaaa");
}

//@TEST
_Bool parse_altoverlap2_opt1_none()
{
    return test_parse_noerr("altoverlap2", "#");
}

//@TEST
_Bool parse_altoverlap2_opt1_one()
{
    return test_parse_noerr("altoverlap2", "a#");
}

//@TEST
_Bool parse_altoverlap2_opt1_many()
{
    return test_parse_noerr("altoverlap2", "aaaaaaaaaaaaaaaaa#");
}

//@TEST
_Bool parse_altoverlap2_error_empty()
{
    return test_parse_parseerr("altoverlap2", "");
}

//@TEST
_Bool parse_altoverlap2_error_opt1_too_many()
{
    return test_parse_parseerr("altoverlap2", "##");
}

//@TEST
_Bool parse_altoverlap2_error_opt1_too_many2()
{
    return test_parse_parseerr("altoverlap2", "a##");
}

//@TEST
_Bool parse_altoverlap2_error_opt1_too_many3()
{
    return test_parse_parseerr("altoverlap2", "aaaaaaaaa##");
}

//@TEST
_Bool parse_altoverlap2_error_invalid()
{
    return test_parse_parseerr("altoverlap2", "x");
}

//@TEST
_Bool parse_altoverlap2_error_invalid2()
{
    return test_parse_parseerr("altoverlap2", "x#");
}

//@TEST
_Bool parse_repoverlap_one()
{
    return test_parse_noerr("repoverlap", "aa#");
}

//@TEST
_Bool parse_repoverlap_two()
{
    return test_parse_noerr("repoverlap", "aaa#");
}

//@TEST
_Bool parse_repoverlap_many()
{
    return test_parse_noerr("repoverlap", "aaaaaaaaaaaaaaa#");
}

//@TEST
_Bool parse_repoverlap_error_empty()
{
    return test_parse_parseerr("repoverlap", "");
}

//@TEST
_Bool parse_repoverlap_error_insufficient()
{
    return test_parse_parseerr("repoverlap", "a#");
}

//@TEST
_Bool parse_repoverlap_error_norep_invalidcat()
{
    return test_parse_parseerr("repoverlap", "#");
}

//@TEST
_Bool parse_repoverlap_error_invalid_rep()
{
    return test_parse_parseerr("repoverlap", "xa#");
}

//@TEST
_Bool parse_repoverlap_error_invalid_cat()
{
    return test_parse_parseerr("repoverlap", "aa@");
}

//@TEST
_Bool parse_repoverlap_error_invalid_rep_cat()
{
    return test_parse_parseerr("repoverlap", "xa@");
}

//@TEST
_Bool parse_optoverlap_no_opt()
{
    return test_parse_noerr("optoverlap", "aab");
}

//@TEST
_Bool parse_optoverlap_with_opt()
{
    return test_parse_noerr("optoverlap", "aaab");
}

//@TEST
_Bool parse_optoverlap_error_empty()
{
    return test_parse_parseerr("optoverlap", "");
}

//@TEST
_Bool parse_optoverlap_error_insufficient()
{
    return test_parse_parseerr("optoverlap", "a");
}

//@TEST
_Bool parse_optoverlap_error_insufficient2()
{
    return test_parse_parseerr("optoverlap", "ab");
}

//@TEST
_Bool parse_optoverlap_error_insufficient3()
{
    return test_parse_parseerr("optoverlap", "b");
}

//@TEST
_Bool parse_optoverlap_error_invalid()
{
    return test_parse_parseerr("optoverlap", "#");
}

//@TEST
_Bool parse_optoverlap_error_invalid2()
{
    return test_parse_parseerr("optoverlap", "a#b");
}

//@TEST
_Bool parse_optoverlap_error_invalid3()
{
    return test_parse_parseerr("optoverlap", "aa#");
}

//@TEST
_Bool parse_abnf_grammar()
{
    return abnf_parse(&abnfgrammar, "rulelist",
            "rulelist\t =  1*( rule / (*c-wsp c-nl) )"
            "\r\n\r\n"
            "rule\t =  rulename defined-as elements c-nl\r\n"
            "\t ; continues if next line starts\r\n"
            "\t ;  with white space"
            "\r\n\r\n"       
            "rulename\t =  ALPHA *(ALPHA / DIGIT / \"-\")"
            "\r\n\r\n"       
            "defined-as\t =  *c-wsp (\"=\" / \"=/\") *c-wsp\r\n"
            "\t\t ; basic rules definition and\r\n"
            "\t\t ;  incremental alternatives"
            "\r\n\r\n"
            "elements\t =  alternation *c-wsp"
            "\r\n\r\n"       
            "c-wsp\t =  WSP / (c-nl WSP)"
            "\r\n\r\n"       
            "c-nl\t =  comment / CRLF"
            "\r\n\t\t ; comment or newline"
            "\r\n\r\n"       
            "comment\t =  \";\" *(WSP / VCHAR) CRLF"
            "\r\n\r\n"       
            "alternation\t =  concatenation\r\n"
            "\t\t*(c-wsp \"/\" *c-wsp concatenation)"
            "\r\n\r\n"       
            "concatenation\t =  "
            "repetition *(1*c-wsp repetition)"
            "\r\n\r\n"       
            "repetition\t =  [repeat] element"
            "\r\n\r\n"       
            "repeat\t =  1*DIGIT / (*DIGIT \"*\" *DIGIT)"
            "\r\n\r\n"       
            "element\t =  rulename / group / option /\r\n"
            "\t\tchar-val / num-val / prose-val"
            "\r\n\r\n"       
            "group\t =  \"(\" *c-wsp alternation *c-wsp \")\""
            "\r\n\r\n"       
            "option\t =  \"[\" *c-wsp alternation *c-wsp \"]\""
            "\r\n\r\n"       
            "char-val\t =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE\r\n"
            "\t\t ; quoted string of SP and VCHAR\r\n"
            "\t\t ;  without DQUOTE"
            "\r\n\r\n"       
            "num-val\t =  \"%\" (bin-val / dec-val / hex-val)"
            "\r\n\r\n"       
            "bin-val\t =  \"b\" 1*BIT\r\n"
            "\t\t[ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]\r\n"
            "\t\t ; series of concatenated bit values\r\n"
            "\t\t ;   or single ONEOF range"
            "\r\n\r\n"       
            "dec-val\t =  \"d\" 1*DIGIT\r\n"
            "\t\t[ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]\r\n"
            "\r\n\r\n"       
            "hex-val\t =  \"x\" 1*HEXDIG\r\n"
            "\t\t[ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]\r\n"
            "\r\n\r\n"       
            "prose-val\t =  \"<\" *(%x20-3D / %x3F-7E) \">\"\r\n"
            "\t\t ; bracketed string of SP and VCHAR\r\n"
            "\t\t ;  without angles\r\n"
            "\t\t ; prose description, to be used as\r\n"
            "\t\t ;  last resort"
            "\r\n\r\n"       
            "ALPHA\t =  %x41-5A / %x61-7A\t; A-Z / a-z"
            "\r\n\r\n"       
            "BIT\t =  \"0\" / \"1\""
            "\r\n\r\n"       
            "CHAR\t =  %x01-7F\r\n"
            "\t\t ; any 7-bit US-ASCII character,\r\n"
            "\t\t ; excluding NUL"
            "\r\n\r\n"       
            "CR\t =  %x0D\r\n\t\t ; carriage return"
            "\r\n\r\n"       
            "CRLF\t =  CR LF\r\n"
            "\t\t ; Internet standard newline"
            "\r\n\r\n"       
            "CTL\t =  %x00-1F / %x7F\r\n\t\t ; controls"
            "\r\n\r\n"       
            "DIGIT\t =  %x30-39\r\n\t\t ; 0-9"
            "\r\n\r\n"       
            "DQUOTE\t =  %x22\r\n\t\t ; \" (Double Quote)"
            "\r\n\r\n"       
            "HEXDIG\t =  DIGIT / \"A\" / \"B\" / \"C\" / \"D\" / \"E\" "
            "/ \"F\""
            "\r\n\r\n"       
            "HTAB\t =  %x09\r\n\t\t ; horizontal tab"
            "\r\n\r\n"       
            "LF\t =  %x0A\r\n\t\t ; linefeed"
            "\r\n\r\n"       
            "LWSP\t =  *(WSP / CRLF WSP)\r\n"
            "\t\t ; Use of this linear-white-space rule\r\n"
            "\t\t ;  permits lines containing only white\r\n"
            "\t\t ;  space that are no longer legal in\r\n"
            "\t\t ;  mail headers and have caused\r\n"
            "\t\t ;  interoperability problems in other\r\n"
            "\t\t ;  contexts.\r\n"
            "\t\t ; Do not use when defining mail\r\n"
            "\t\t ;  headers and use with caution in\r\n"
            "\t\t ;  other contexts."
            "\r\n\r\n"       
            "OCTET\t =  %x00-FF\r\n\t\t ; 8 bits of data"
            "\r\n\r\n"
            "SP\t =  %x20"
            "\r\n\r\n"
            "VCHAR\t =  %x21-7E\r\n"
            "\t\t ; visible (printing) characters"
            "\r\n\r\n"
            "WSP\t =  SP / HTAB\r\n\t\t ; white space"
            "\r\n",
            NULL
        ) == ABNF_NOERR;
}
