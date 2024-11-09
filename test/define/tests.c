#include "abnf.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define test_define_noerr(rule) test_define(rule, ABNF_NOERR)
#define test_define_parseerr(rule) test_define(rule, ABNF_PARSEERR)

#define test_elements_noerr(rule) test_elements(rule, ABNF_NOERR)
#define test_elements_parseerr(rule) test_elements(rule, ABNF_PARSEERR)

typedef struct {
    const char* rule;
    int expected;
} definedat;

char* concat_str(const char* s1, const char* s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    void* p = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!p) {
        return NULL;
    }

    char* str = (char*)p;

    size_t i;
    for (i = 0; i < len1; i++) {
        str[i] = s1[i];
    }
    for (size_t j = 0; j < len2; j++) {
        str[i++] = s2[j];
    }
    str[i] = '\0';

    return str;
}

_Bool perform_test(
        void* extra,
        _Bool(*test)(struct abnf*, void*)
) {
    struct abnf grammar;
    abnf_init(&grammar);

    _Bool success = test(&grammar, extra);

    abnf_free(&grammar);
    return success;
}

_Bool test_define_helper(struct abnf* grammar, void* extra)
{
    definedat* dat = (definedat*)extra;
    return abnf_define(grammar, dat->rule) == dat->expected;
}

_Bool test_define(const char* rule, int expected)
{
    definedat dat = {rule, expected};
    return perform_test(&dat, test_define_helper);
}

_Bool test_elements(const char* src, int expected) {
    char* rule = concat_str("rule=", src);
    if (!rule) {
        return false;
    }

    _Bool success = test_define(rule, expected);

    free(rule);
    return success;
}

//@TEST
_Bool define_name_alpha()
{
    return test_define_noerr("abnf=%x65");
}

//@TEST
_Bool define_name_lots_of_hyphens()
{
    return test_define_noerr("a----------=%x65");
}

//@TEST
_Bool define_name_lots_of_digits()
{
    return test_define_noerr("a0123456789=%x65");
}

//@TEST
_Bool define_name_digits_and_hyphens()
{
    return test_define_noerr("a-1-b-2-c-3-d-=%x65");
}

//@TEST
_Bool define_name_parse_error_leading_space()
{
    return test_define_parseerr(" abnf=%x65");
}

//@TEST
_Bool define_definedas_inc()
{
    return test_define_noerr("abnf=/%x65");
}

//@TEST
_Bool define_definedas_with_spaces()
{
    return test_define_noerr("abnf   =/   %x65");
}

//@TEST
_Bool define_definedas_with_tabs()
{
    return test_define_noerr("abnf\t\t\t=/\t\t\t%x65");
}

//@TEST
_Bool define_definedas_with_crlf()
{
    return test_define_noerr("abnf\r\n =/\r\n %x65");
}

//@TEST
_Bool define_definedas_with_space_tab_crlf()
{
    return test_define_noerr(
            "abnf\r\n \r\n\t\r\n \r\n\t=/\r\n\t\r\n \r\n\t\r\n %x65"
    );
}

//@TEST
_Bool define_definedas_with_comments()
{
    return test_define_noerr(
            "abnf;comment\r\n ;comment\r\n\t=/;\t\t\r\n ;comment\r\n %x65"
    );
}

//@TEST
_Bool define_definedas_with_comments_space_tab_crlf()
{
    return test_define_noerr(
            "abnf;;;  ;;\t\r\n \r\n ;this!\r\n \t;is@\r\n ;a#\r\n\t;comment\r\n   "
            "=/"
            ";,\t\t\t{}|^\r\n\t  ;;;\r\n \t\r\n ;\r\n\t;\r\n \t;!%()_+\r\n\t\t"
            "%x65"
    );
}

//@TEST
_Bool define_definedas_parse_error_crlf_no_wsp()
{
    return test_define_parseerr("abnf\r\n=/%x65");
}

//@TEST
_Bool define_definedas_parse_error_comment_no_wsp()
{
    return test_define_parseerr("abnf;this is a comment!\r\n=/%x65");
}

//@TEST
_Bool define_definedas_parse_error_many_comments_no_wsp()
{
    return test_define_parseerr(
            "abnf;this!\r\n ;is!\r\n ;a!\r\n;comment!\r\n =/%x65"
    );
}

//@TEST
_Bool define_elements_charval()
{
    char buf[512];
    buf[0] = 0x20;
    buf[1] = 0x21;
    int i = 2;
    for (char c = 0x23; c <= 0x7E; c++) {
        buf[i++] = c;
    }

    for (int j = 0; j < i; j++) {
        char src[4];
        src[0] = '\"';
        src[1] = buf[j];
        src[2] = '\"';
        src[3] = '\0';

        if (!test_elements_noerr(src)) {
            return false;
        }
    }

   return true;
}

//@TEST
_Bool define_elements_charval_empty()
{
    return test_elements_noerr("\"\"");
}

//@TEST
_Bool define_elements_charval_multiple()
{
    return test_elements_noerr("\"abcdef123456!@#$%^\"");
}

//@TEST
_Bool define_elements_charval_parse_error_no_open_quote()
{
    return test_elements_parseerr("1\"");
}

//@TEST
_Bool define_elements_charval_parse_error_no_close_quote()
{
    return test_elements_parseerr("\"1");
}

//@TEST
_Bool define_elements_charval_parse_error_invalid_char()
{
    return test_elements_parseerr("\"\22\"");
}

//@TEST
_Bool define_elements_binval()
{
    char values[2];
    values[0] = '0';
    values[1] = '1';

    for (size_t i = 0; i < 2; i++) {
        char src[4];
        src[0] = '%';
        src[1] = 'b';
        src[2] = values[i];
        src[3] = '\0';

        if (!test_elements_noerr(src)) {
            return false;
        }
    }

    return true;
}

//@TEST
_Bool define_elements_binval_multiple()
{
    return test_elements_noerr("%b010110111011110");
}

//@TEST
_Bool define_elements_binval_concat()
{
    return test_elements_noerr("%b0.10.110.1110.11110");
}

//@TEST
_Bool define_elements_binval_range()
{
    return test_elements_noerr("%b01100-10011");
}

//@TEST
_Bool define_elements_binval_invalid()
{
    return test_elements_parseerr("%b2");
}

//@TEST
_Bool define_elements_binval_parse_error_start_concat()
{
    return test_elements_parseerr("%b.0010");
}

//@TEST
_Bool define_elements_binval_parse_error_end_concat()
{
    return test_elements_parseerr("%b0010.");
}

//@TEST
_Bool define_elements_binval_parse_error_invalid_concat()
{
    return test_elements_parseerr("%b0000.2001");
}

//@TEST
_Bool define_elements_binval_parse_error_invalid_double_concat()
{
    return test_elements_parseerr("%b0000..1001");
}

//@TEST
_Bool define_elements_binval_parse_error_start_range()
{
    return test_elements_parseerr("%b-01100");
}

//@TEST
_Bool define_elements_binval_parse_error_end_range()
{
    return test_elements_parseerr("%b01100-");
}

//@TEST
_Bool define_elements_binval_parse_error_invalid_range()
{
    return test_elements_parseerr("%b01100-20011");
}

//@TEST
_Bool define_elements_binval_parse_error_double_range()
{
    return test_elements_parseerr("%b01100--10011");
}

//@TEST
_Bool define_elements_decval()
{
    char values[10];
    int i = 0;
    for (char c = '0'; c <= '9'; c++) {
        values[i++] = c;
    }

    for (size_t i = 0; i < 10; i++) {
        char src[4];
        src[0] = '%';
        src[1] = 'd';
        src[2] = values[i];
        src[3] = '\0';

        if (!test_elements_noerr(src)) {
            return false;
        }
    }

    return true;
}

//@TEST
_Bool define_elements_decval_multiple()
{
    return test_elements_noerr("%d1234567890");
}

//@TEST
_Bool define_elements_decval_concat()
{
    return test_elements_noerr("%d0.12.123.1234.12345");
}

//@TEST
_Bool define_elements_decval_range()
{
    return test_elements_noerr("%d01234-54321");
}

//@TEST
_Bool define_elements_decval_invalid()
{
    return test_elements_parseerr("%da");
}

//@TEST
_Bool define_elements_decval_parse_error_start_concat()
{
    return test_elements_parseerr("%d.9876");
}

//@TEST
_Bool define_elements_decval_parse_error_end_concat()
{
    return test_elements_parseerr("%d9876.");
}

//@TEST
_Bool define_elements_decval_parse_error_invalid_concat()
{
    return test_elements_parseerr("%d9876.a543");
}

//@TEST
_Bool define_elements_decval_parse_error_double_concat()
{
    return test_elements_parseerr("%d9876..a543");
}

//@TEST
_Bool define_elements_decval_parse_error_start_range()
{
    return test_elements_parseerr("%d-98765");
}

//@TEST
_Bool define_elements_decval_parse_error_end_range()
{
    return test_elements_parseerr("%d98765-");
}

//@TEST
_Bool define_elements_decval_parse_error_invalid_range()
{
    return test_elements_parseerr("%d98765-a4321");
}

//@TEST
_Bool define_elements_decval_parse_error_double_range()
{
    return test_elements_parseerr("%d98765--a4321");
}

//@TEST
_Bool define_elements_hexval()
{
    char values[22];
    int i = 0;
    for (char c = '0'; c <= '9'; c++) {
        values[i++] = c;
    }
    for (char c = 'a'; c <= 'f'; c++) {
        values[i++] = c;
    }
    for (char c = 'A'; c <= 'F'; c++) {
        values[i++] = c;
    }

    for (size_t i = 0; i < 22; i++) {
        char src[4];
        src[0] = '%';
        src[1] = 'x';
        src[2] = values[i];
        src[3] = '\0';

        if (!test_elements_noerr(src)) {
            return false;
        }
    }

    return true;
}

//@TEST
_Bool define_elements_hexval_multiple()
{
    return test_elements_noerr("%x1234567890abfcdefABCDEF");
}

//@TEST
_Bool define_elements_hexval_concat()
{
    return test_elements_noerr("%xabc.DEF.123.1234.98765");
}

//@TEST
_Bool define_elements_hexval_range()
{
    return test_elements_noerr("%xfed123-cba987");
}

//@TEST
_Bool define_elements_hexval_invalid()
{
    return test_elements_parseerr("%xg");
}

//@TEST
_Bool define_elements_hexval_parse_error_start_concat()
{
    return test_elements_parseerr("%x.9876");
}

//@TEST
_Bool define_elements_hexval_parse_error_end_concat()
{
    return test_elements_parseerr("%x9876.");
}

//@TEST
_Bool define_elements_hexval_parse_error_invalid_concat()
{
    return test_elements_parseerr("%x9876.g543");
}

//@TEST
_Bool define_elements_hexval_parse_error_double_concat()
{
    return test_elements_parseerr("%x9876..g543");
}

//@TEST
_Bool define_elements_hexval_parse_error_start_range()
{
    return test_elements_parseerr("%x-98765");
}

//@TEST
_Bool define_elements_hexval_parse_error_end_range()
{
    return test_elements_parseerr("%x98765-");
}

//@TEST
_Bool define_elements_hexval_parse_error_invalid_range()
{
    return test_elements_parseerr("%x98765-g4321");
}

//@TEST
_Bool define_elements_hexval_parse_error_double_range()
{
    return test_elements_parseerr("%x98765--g4321");
}

//@TEST
_Bool define_elements_proseval()
{
    char values[512];
    int i = 0;
    for (int c = 0x20; c <= 0x3D; c++) {
        values[i++] = c;
    }
    for (int c = 0x3F; c <= 0x7E; c++) {
        values[i++] = c;
    }

    for (int j = 0; j < i; j++) {
        char src[4];
        src[0] = '<';
        src[1] = values[j];
        src[2] = '>';
        src[3] = '\0';

        if (!test_elements_noerr(src)) {
            return false;
        }
    }

    return true;
}

//@TEST
_Bool define_elements_proseval_empty()
{
    return test_elements_noerr("<>");
}

//@TEST
_Bool define_elements_proseval_multiple()
{
    char src[512];
    int i = 0;

    src[i++] = '<';
    for (int c = 0x20; c <= 0x3D; c++) {
        src[i++] = c;
    }
    for (int c = 0x3F; c <= 0x7E; c++) {
        src[i++] = c;
    }
    src[i++] = '>';
    src[i++] = '\0';

    return test_elements_noerr(src);
}

//@TEST
_Bool define_elements_proseval_parse_error_invalid()
{
    return test_elements_parseerr("<\t>");
}

//@TEST
_Bool define_elements_proseval_parse_error_close_bracket()
{
    return test_elements_parseerr("<abc");
}

//@TEST
_Bool define_elements_name()
{
    return test_elements_noerr("a");
}

//@TEST
_Bool define_elements_name_alpha()
{
    return test_elements_noerr("abcdefghijklmnopqrstuvwxyz");
}

//@TEST
_Bool define_elements_name_digits()
{
    return test_elements_noerr("a0123456789");
}

//@TEST
_Bool define_elements_name_hypens()
{
    return test_elements_noerr("a--------------");
}

//@TEST
_Bool define_elements_name_digits_and_hyphens()
{
    return test_elements_noerr("a-1-b-2-c-3-d-4-");
}

//@TEST
_Bool define_elements_name_parse_error_start_hyphen()
{
    return test_elements_parseerr("-abc");
}

//@TEST
_Bool define_elements_name_parse_error_empty()
{
    return test_elements_parseerr("");
}

//@TEST
_Bool define_elements_repetition()
{
    return test_elements_noerr("9abnf");
}

//@TEST
_Bool define_elements_repetition_multiple_digits()
{
    return test_elements_noerr("1234567890abnf");
}

//@TEST
_Bool define_elements_repetition_no_upper()
{
    return test_elements_noerr("1*abnf");
}

//@TEST
_Bool define_elements_repetition_no_upper_multiple_digits()
{
    return test_elements_noerr("1234567890*abnf");
}

//@TEST
_Bool define_elements_repetition_no_lower()
{
    return test_elements_noerr("*1abnf");
}

//@TEST
_Bool define_elements_repetition_no_lower_multiple_digits()
{
    return test_elements_noerr("*1234567890abnf");
}

//@TEST
_Bool define_elements_repetition_lower_and_upper()
{
    return test_elements_noerr("1*9abnf");
}

//@TEST
_Bool define_elements_repetition_lower_and_upper_multiple_digits()
{
    return test_elements_noerr("1234567890*1234567890abnf");
}

//@TEST
_Bool define_elements_repetition_invalid()
{
    return test_elements_parseerr("123#456abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_double_star()
{
    return test_elements_parseerr("**abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_multiple_stars()
{
    return test_elements_parseerr("1*2*abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_multiple_stars2()
{
    return test_elements_parseerr("1*2*3abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_invalid_number()
{
    return test_elements_parseerr("1*-1abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_contains_spaces()
{
    return test_elements_parseerr("1 abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_contains_spaces2()
{
    return test_elements_parseerr("1 * 2abnf");
}

//@TEST
_Bool define_elements_repetition_parse_error_contains_spaces3()
{
    return test_elements_parseerr("* 2abnf");
}

//@TEST
_Bool define_elements_concatenation()
{
    return test_elements_noerr("1*2one 3*4\"1\"");
}

//@TEST
_Bool define_elements_concatenation_multiple()
{
    return test_elements_noerr("1*2one 3*4\"1\" 5*6%x0-f 7*8<\">");
}

//@TEST
_Bool define_elements_concatenation_multiple_spaces()
{
    return test_elements_noerr("1*2rule-one\t  \t3*4rule-two");
}

//@TEST
_Bool define_elements_alternation()
{
    return test_elements_noerr("one / \"1\"");
}

//@TEST
_Bool define_elements_alternation_multiple()
{
    return test_elements_noerr("1*2one / 2*3\"1\" / 3*4%x0-f / 5*6<\">");
}

//@TEST
_Bool define_elements_alternation_no_spaces()
{
    return test_elements_noerr("one/two/three/four");
}

//@TEST
_Bool define_elements_alternation_mixed_spaces()
{
    return test_elements_noerr("one/ two/three /four");
}

//@TEST
_Bool define_elements_alternation_multiple_spaces()
{
    return test_elements_noerr("one\t\t/  two \t/\t three   /   four");
}

//@TEST
_Bool define_elements_altnernation_newlines()
{
    return test_elements_noerr("one\r\n /\r\n two");
}

//@TEST
_Bool define_elements_group()
{
    return test_elements_noerr("( rule )");
}

//@TEST
_Bool define_elements_group_nested()
{
    return test_elements_noerr("((((( rule )))))");
}

//@TEST
_Bool define_elements_group_nested_mixed_spaces()
{
    return test_elements_noerr("( \t ( (rule ))\t )");
}

//@TEST
_Bool define_elements_group_no_spaces()
{
    return test_elements_noerr("(rule)");
}

//@TEST
_Bool define_elements_group_parse_error_empty()
{
    return test_elements_parseerr("()");
}

//@TEST
_Bool define_elements_group_parse_error_empty_nested()
{
    return test_elements_parseerr("((()))");
}

//@TEST
_Bool define_elements_group_parse_error_no_close()
{
    return test_elements_parseerr("(rule");
}

//@TEST
_Bool define_elements_group_parse_error_mismatched_brackets()
{
    return test_elements_parseerr("((rule)");
}

//@TEST
_Bool define_elements_group_parse_error_different_brackets()
{
    return test_elements_parseerr("(rule]");
}

//@TEST
_Bool define_elements_option()
{
    return test_elements_noerr("[ rule ]");
}

//@TEST
_Bool define_elements_option_nested()
{
    return test_elements_noerr("[[[[[ rule ]]]]]");
}

//@TEST
_Bool define_elements_option_nested_mixed_spaces()
{
    return test_elements_noerr("[ \t [ [rule ]]\t ]");
}

//@TEST
_Bool define_elements_option_no_spaces()
{
    return test_elements_noerr("[rule]");
}

//@TEST
_Bool define_elements_option_parse_error_empty()
{
    return test_elements_parseerr("[]");
}

//@TEST
_Bool define_elements_option_parse_error_empty_nested()
{
    return test_elements_parseerr("[[[]]]");
}

//@TEST
_Bool define_elements_option_parse_error_no_close()
{
    return test_elements_parseerr("[rule");
}

//@TEST
_Bool define_elements_option_parse_error_mismatched_brackets()
{
    return test_elements_parseerr("[[rule]");
}

//@TEST
_Bool define_elements_option_parse_error_different_brackets()
{
    return test_elements_parseerr("[rule)");
}

//@TEST
_Bool define_generic()
{
    return test_define_noerr("rulelist\t =  1*( rule / (*c-wsp c-nl) )");
}

//@TEST
_Bool define_generic1()
{
    return test_define_noerr(
            "rule\t =  rulename defined-as elements c-nl\r\n"
            "\t ; continues if next line starts\r\n"
            "\t ;  with white space"
    );
}

//@TEST
_Bool define_generic2()
{
    return test_define_noerr("rulename\t =  ALPHA *(ALPHA / DIGIT / \"-\")");
}

//@TEST
_Bool define_generic3()
{
    return test_define_noerr("elements\t =  alternation *c-wsp");
}

//@TEST
_Bool define_generic4()
{
    return test_define_noerr("c-wsp\t =  WSP / (c-nl WSP)");
}

//@TEST
_Bool define_generic5()
{
    return test_define_noerr(
            "c-nl\t =  comment / CRLF\r\n\t ; comment or newline"
    );
}

//@TEST
_Bool define_generic6()
{
    return test_define_noerr("comment\t =  \";\" *(WSP / VCHAR) CRLF");
}

//@TEST
_Bool define_generic7()
{
    return test_define_noerr(
            "alternation\t =  concatenation\r\n"
            "\t*(*c-wsp \"/\" *c-wsp concatenation)"
    );
}

//@TEST
_Bool define_generic8()
{
    return test_define_noerr(
            "concatenation\t =  repetition *(1*c-wsp repetition)"
    );
}

//@TEST
_Bool define_generic9()
{
    return test_define_noerr("repetition\t =  [repeat] element");
}

//@TEST
_Bool define_generic10()
{
    return test_define_noerr("repeat\t =  1*DIGIT / (*DIGIT \"*\" *DIGIT)");
}

//@TEST
_Bool define_generic11()
{
    return test_define_noerr(
            "element\t =  rulename / group / option /\r\n"
            "\tchar-val / num-val / prose-val"
    );
}

//@TEST
_Bool define_generic12()
{
    return test_define_noerr(
            "group\t =  \"(\" *c-wsp alternation *c-wsp \")\""
     );
}

//@TEST
_Bool define_generic13()
{
    return test_define_noerr(
            "option\t =  \"[\" *c-wsp alternation *c-wsp \"]\""
    );
}

//@TEST
_Bool define_generic14()
{
    return test_define_noerr(
            "char-val\t =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE\r\n"
            "\t\t ; quoted string of SP and VCHAR\r\n"
            "\t\t ;  without DQUOTE"
    );
}

//@TEST
_Bool define_generic15()
{
    return test_define_noerr(
            "num-val\t =  \"%\" (bin-val / dec-val / hex-val)"
    );
}

//@TEST
_Bool define_generic16()
{
    return test_define_noerr(
            "bin-val\t =  \"b\" 1*BIT\r\n"
            "\t[ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]\r\n"
            "\t\t ; series of concatenated bit values\r\n"
            "\t\t ;  or single ONEOF range"
    );
}

//@TEST
_Bool define_generic17()
{
    return test_define_noerr(
            "dec-val\t =  \"d\" 1*DIGIT\r\n"
            "\t[ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]\r\n"
    );
}

//@TEST
_Bool define_generic18()
{
    return test_define_noerr(
            "hex-val\t =  \"x\" 1*HEXDIG\r\n"
            "\t[ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]\r\n"
    );
}

//@TEST
_Bool define_generic19()
{
    return test_define_noerr(
            "prose-val\t =  \"<\" *(%x20-3D / %x3F-7E) \">\"\r\n"
            "\t\t ; bracketed string of SP and VCHAR\r\n"
            "\t\t ;  without angles\r\n"
            "\t\t ; prose description, to be used as\r\n"
            "\t\t ;  last resort"
    );
}

//@TEST
_Bool define_multiple_lines()
{
    return test_define_noerr("abnf\t = one\r\n two");
}

//@TEST
_Bool define_trailing_wsp()
{
    return test_define_noerr(
            "name\t =  ALPHA *(ALPHA / DIGIT / \"-\") \r\n\t"
    );
}

_Bool define_multiple_defines_helper(struct abnf* grammar, void* extra)
{
    // the full grammar for a valid ABNF grammar as described in
    // RFC 5234 (Augmented BNF for Syntax Specifications: ABNF)
    return abnf_define(grammar,
                "rulelist\t =  1*( rule / (*c-wsp c-nl) )") == ABNF_NOERR
            && abnf_define(grammar,
                "rule\t =  rulename defined-as elements c-nl\r\n"
                "\t ; continues if next line starts\r\n"
                "\t ;  with white space") == ABNF_NOERR
            && abnf_define(grammar,
                "rulename\t =  ALPHA *(ALPHA / DIGIT / \"-\")") == ABNF_NOERR
            && abnf_define(grammar,
                "defined-as\t =  *c-wsp (\"=\" / \"=/\") *c-wsp\r\n"
                "\t\t ; basic rules definition and\r\n"
                "\t\t ;  incremental alternatives") == ABNF_NOERR
            && abnf_define(grammar,
                "elements\t =  alternation *c-wsp") == ABNF_NOERR
            && abnf_define(grammar,
                "c-wsp\t =  WSP / (c-nl WSP)") == ABNF_NOERR
            && abnf_define(grammar,
                "c-nl\t =  comment / CRLF"
                "\r\n\t\t ; comment or newline") == ABNF_NOERR
            && abnf_define(grammar,
                "comment\t =  \";\" *(WSP / VCHAR) CRLF") == ABNF_NOERR
            && abnf_define(grammar,
                "alternation\t =  concatenation\r\n"
                "\t\t*(c-wsp \"/\" *c-wsp concatenation)") == ABNF_NOERR
            && abnf_define(grammar,
                "concatenation\t =  "
                "repetition *(1*c-wsp repetition)") == ABNF_NOERR
            && abnf_define(grammar,
                "repetition\t =  [repeat] element") == ABNF_NOERR
            && abnf_define(grammar,
                "repeat\t =  1*DIGIT / (*DIGIT \"*\" *DIGIT)") == ABNF_NOERR
            && abnf_define(grammar,
                "element\t =  rulename / group / option /\r\n"
                "\t\tchar-val / num-val / prose-val") == ABNF_NOERR
            && abnf_define(grammar,
                "group\t =  \"(\" *c-wsp alternation *c-wsp \")\""
                ) == ABNF_NOERR
            && abnf_define(grammar,
                "option\t =  \"[\" *c-wsp alternation *c-wsp \"]\""
                ) == ABNF_NOERR
            && abnf_define(grammar,
                "char-val\t =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE\r\n"
                "\t\t ; quoted string of SP and VCHAR\r\n"
                "\t\t ;  without DQUOTE") == ABNF_NOERR
            && abnf_define(grammar,
                "num-val\t =  \"%\" (bin-val / dec-val / hex-val)"
                ) == ABNF_NOERR
            && abnf_define(grammar,
                "bin-val\t =  \"b\" 1*BIT\r\n"
                "\t\t[ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]\r\n"
                "\t\t ; series of concatenated bit values\r\n"
                "\t\t ;   or single ONEOF range") == ABNF_NOERR
            && abnf_define(grammar,
                "dec-val\t =  \"d\" 1*DIGIT\r\n"
                "\t\t[ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]\r\n") == ABNF_NOERR
            && abnf_define(grammar,
                "hex-val\t =  \"x\" 1*HEXDIG\r\n"
                "\t\t[ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]\r\n") == ABNF_NOERR
            && abnf_define(grammar,
                "prose-val\t =  \"<\" *(%x20-3D / %x3F-7E) \">\"\r\n"
                "\t\t ; bracketed string of SP and VCHAR\r\n"
                "\t\t ;  without angles\r\n"
                "\t\t ; prose description, to be used as\r\n"
                "\t\t ;  last resort") == ABNF_NOERR
            && abnf_define(grammar,
                "ALPHA\t =  %x41-5A / %x61-7A\t; A-Z / a-z") == ABNF_NOERR
            && abnf_define(grammar,
                "BIT\t =  \"0\" / \"1\"") == ABNF_NOERR
            && abnf_define(grammar,
                "CHAR\t =  %x01-7F\r\n"
                "\t\t ; any 7-bit US-ASCII character,\r\n"
                "\t\t ; excluding NUL") == ABNF_NOERR
            && abnf_define(grammar,
                "CR\t =  %x0D\r\n\t\t ; carriage return") == ABNF_NOERR
            && abnf_define(grammar,
                "CRLF\t =  CR LF\r\n"
                "\t\t ; Internet standard newline") == ABNF_NOERR
            && abnf_define(grammar,
                "CTL\t =  %x00-1F / %x7F\r\n\t\t ; controls") == ABNF_NOERR
            && abnf_define(grammar,
                "DIGIT\t =  %x30-39\r\n\t\t ; 0-9") == ABNF_NOERR
            && abnf_define(grammar,
                "DQUOTE\t =  %x22\r\n\t\t ; \" (Double Quote)") == ABNF_NOERR
            && abnf_define(grammar,
                "HEXDIG\t =  DIGIT / \"A\" / \"B\" / \"C\" / \"D\" / \"E\" "
                "/ \"F\"") == ABNF_NOERR
            && abnf_define(grammar,
                "HTAB\t =  %x09\r\n\t\t ; horizontal tab") == ABNF_NOERR
            && abnf_define(grammar,
                "LF\t =  %x0A\r\n\t\t ; linefeed") == ABNF_NOERR
            && abnf_define(grammar,
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
            && abnf_define(grammar,
                "OCTET\t =  %x00-FF\r\n\t\t ; 8 bits of data") == ABNF_NOERR
            && abnf_define(grammar, "SP\t =  %x20") == ABNF_NOERR
            && abnf_define(grammar,
                "VCHAR\t =  %x21-7E\r\n"
                "\t\t ; visible (printing) characters") == ABNF_NOERR
            && abnf_define(grammar,
                "WSP\t =  SP / HTAB\r\n\t\t ; white space") == ABNF_NOERR;
}

//@TEST
_Bool define_multiple_defines()
{
    return perform_test(NULL, define_multiple_defines_helper);
}

//@TEST
_Bool define_parse_error_empty()
{
    return test_define_parseerr("");
}

//@TEST
_Bool define_parse_error_trailing_character()
{
    return test_define_parseerr("name\t =  ALPHA *(ALPHA / DIGIT / \"-\")#");
}

//@TEST
_Bool define_parse_error_nl_nospace()
{
    return test_define_parseerr("name\t =  ALPHA\r\n*(ALPHA / DIGIT / \"-\")");
}

//@TEST
_Bool define_option_nolim()
{
    return test_define_noerr("rule = *[ \"abc\" ]");
}

//@TEST
_Bool define_value_upperlimit()
{
    return test_define_noerr("rule = %b00000000-01111111");
}

_Bool define_error_double_define_helper(struct abnf* grammar, void* extra)
{
    return abnf_define(grammar, "rule = \"abc\"") == ABNF_NOERR
            && abnf_define(grammar, "rule = \"123\"") == ABNF_DEFERR;
}

//@TEST
_Bool define_error_double_define()
{
    return perform_test(NULL, define_error_double_define_helper);
}
