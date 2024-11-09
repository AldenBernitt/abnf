#define CR 0x0D
#define LF 0x0A
#define SP 0x20
#define HTAB 0x09
#define DQUOTE 0x22

static _Bool is_alpha(char c)
{
    return 0x41 <= c && c <= 0x5A || 0x61 <= c && c <= 0x7A;
}

static _Bool is_digit(char c)
{
    return 0x30 <= c && c <= 0x39;
}

static _Bool is_vchar(char c)
{
    return 0x21 <= c && c <= 0x7E;
}

static _Bool is_wsp(char c)
{
    return c == SP || c == HTAB;
}

static _Bool is_bit(char c)
{
    return c == '0' || c == '1';
}

static _Bool is_dec(char c)
{
    return '0' <= c && c <= '9';
}

static _Bool is_hex(char c)
{
    return is_dec(c) || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F';
}
