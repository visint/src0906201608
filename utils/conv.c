#include <iconv.h>
static int code_convert(char* from_charset, char* to_charset,
        char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset, from_charset);
    if (cd ==  (iconv_t)-1)
    {
        perror("iconv_open:");
        return -1;
    }

    memset(outbuf,0,outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
    {
        //printf("%s: call iconv failed!\n", __FUNCTION__);
        printf("errno=%d\n", errno);
        perror("iconv failed:\n");
        iconv_close(cd);
        return -1;
    }
    iconv_close(cd);
    return 0;
}

int g2u(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}

int u2g(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return code_convert("utf-8", "gb2312",inbuf, inlen, outbuf, outlen);
}
