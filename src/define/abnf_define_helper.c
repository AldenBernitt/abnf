#include "convert.h"

#define parse_wsp(in) parse_chpred(in, is_wsp)
#define parse_vchar(in) parse_chpred(in, is_vchar)
#define parse_bit(in) parse_chpred(in, is_bit)
#define parse_cnl(in) (parse_comment(in) || parse_crlf(in))

static _Bool parse_int(struct chstream* in, int* out)
{
    if (!is_digit(chstream_peek(in, 0))) {
        return false;
    }

    int val = 0;

    for (
            char c = chstream_peek(in, 0);
            is_digit(c);
            c = chstream_peek(in, 0)
    ) {
        val = val * 10 + ctod(c);
        chstream_advance(in, 1);
    }

    *out = val;
    return true;
}

static _Bool parse_ch(struct chstream* in, char ch)
{
    if (chstream_peek(in, 0) == ch) {
        chstream_advance(in, 1);
        return true;
    }
    else {
        return false;
    }
}

static _Bool parse_str(struct chstream* in, const char* str)
{
    int reset = in->pos;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!parse_ch(in, str[i])) {
            chstream_set(in, reset);
            return false;
        }
    }

    return true;
}

static _Bool parse_chpred(struct chstream* in, _Bool(*pred)(char))
{
    if (pred(chstream_peek(in, 0))) {
        chstream_advance(in, 1);
        return true;
    }
    else {
        return false;
    }
}

static _Bool read_val(
        struct chstream* in,
        _Bool(*valpred)(char),
        char *out
) {
    char c = chstream_peek(in, 0);

    if (parse_chpred(in, valpred)) {
        *out = c;
        return true;
    }
    else {
        return false;
    }
}

static _Bool parse_crlf(struct chstream* in)
{
    int reset = in->pos;

    _Bool success = parse_ch(in, CR) && parse_ch(in, LF);

    if (!success) {
        chstream_set(in, reset);
    }

    return success;
}

static _Bool parse_comment(struct chstream* in)
{
    // comment = ";" *(WSP / VCHAR) CRLF
    
    if (!parse_ch(in, ';')) {
        return false;
    }

    while (parse_wsp(in) || parse_vchar(in))
        ; // EMPTY WHILE

    return parse_crlf(in);
}

static _Bool parse_cwsp(struct chstream* in)
{
    // c-ws = WSP / (c-nl WSP)

    int reset = in->pos;

    if (parse_wsp(in) || parse_cnl(in) && parse_wsp(in)) {
        return true;
    }
    else {
        chstream_set(in, reset);
        return false;
    }
}


