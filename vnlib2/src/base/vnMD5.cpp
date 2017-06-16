//
//  vnMD5.cpp
//  vnlib
//
//  Created by Wayne on 8/13/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnMD5.h"

#include "vnMemory.h"
#include <string.h>
_vn_begin

namespace {
    void MD5Init(MD5::MD5_CTX *context);
    void MD5Update(MD5::MD5_CTX *context,u8 *input,u32 inputlen);
    void MD5Final(MD5::MD5_CTX *context,u8 digest[16]);
    void MD5Transform(u32 state[4],u8 block[64]);
    void MD5Encode(u8 *output,u32 *input,u32 len);
    void MD5Decode(u32 *output,u8 *input,u32 len);
    
    void byte2hex_uppercase(u8 byte, c8 hex[2]);
    void byte2hex_lowercase(u8 byte, c8 hex[2]);
}

MD5::MD5() {
    MD5Init(&m_ctx);
}

std::vector<u8> MD5::fetch() {
    std::vector<u8> ret(16);
    MD5Final(&m_ctx, ret.data());
    MD5Init(&m_ctx);
    m_processed = 0;
    return std::move(ret);
}

str8 MD5::fetch_hex_uppercase() {
    u8 digest[16];
    MD5Final(&m_ctx, digest);
    MD5Init(&m_ctx);
    m_processed = 0;
    
    str8 ret(32, '\0');
    c8 *p = &ret[0];
    for (size_t i = 0; i < 16; ++i, p += 2) {
        byte2hex_uppercase(digest[i], p);
    }
    return std::move(ret);
}

str8 MD5::fetch_hex_lowercase() {
    u8 digest[16];
    MD5Final(&m_ctx, digest);
    MD5Init(&m_ctx);
    m_processed = 0;
    
    str8 ret(32, '\0');
    c8 *p = &ret[0];
    for (size_t i = 0; i < 16; ++i, p += 2) {
        byte2hex_lowercase(digest[i], p);
    }
    return std::move(ret);
}

size_t MD5::read(void *buffer, size_t size) {
    return 0;
}

size_t MD5::write(const void *buffer, size_t size) {
    MD5Update(&m_ctx, (u8 *)buffer, (u32)size);
    m_processed += size;
    return size;
}

bool MD5::seek(s64 offset, SeekDir way) {
    return false;
}

s64 MD5::tell() {
    return m_processed;
}


namespace {
    
    void byte2hex_uppercase(u8 byte, c8 hex[2]) {
        const c8 *codes = "0123456789ABCDEF";
        hex[1] = codes[byte & 0xF];
        hex[0] = codes[byte >> 4];
    }
    
    void byte2hex_lowercase(u8 byte, c8 hex[2]) {
        const c8 *codes = "0123456789abcdef";
        hex[1] = codes[byte & 0xF];
        hex[0] = codes[byte >> 4];
    }
    
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
{ \
a += F(b,c,d) + x + ac; \
a = ROTATE_LEFT(a,s); \
a += b; \
}
#define GG(a,b,c,d,x,s,ac) \
{ \
a += G(b,c,d) + x + ac; \
a = ROTATE_LEFT(a,s); \
a += b; \
}
#define HH(a,b,c,d,x,s,ac) \
{ \
a += H(b,c,d) + x + ac; \
a = ROTATE_LEFT(a,s); \
a += b; \
}
#define II(a,b,c,d,x,s,ac) \
{ \
a += I(b,c,d) + x + ac; \
a = ROTATE_LEFT(a,s); \
a += b; \
}
    u8 PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    void MD5Init(MD5::MD5_CTX *context)
    {
        context->count[0] = 0;
        context->count[1] = 0;
        context->state[0] = 0x67452301;
        context->state[1] = 0xEFCDAB89;
        context->state[2] = 0x98BADCFE;
        context->state[3] = 0x10325476;
    }
    void MD5Update(MD5::MD5_CTX *context,u8 *input,u32 inputlen)
    {
        u32 i = 0,index = 0,partlen = 0;
        index = (context->count[0] >> 3) & 0x3F;
        partlen = 64 - index;
        context->count[0] += inputlen << 3;
        if(context->count[0] < (inputlen << 3))
            context->count[1]++;
        context->count[1] += inputlen >> 29;
        
        if(inputlen >= partlen)
        {
            memcpy(&context->buffer[index],input,partlen);
            MD5Transform(context->state,context->buffer);
            for(i = partlen;i+64 <= inputlen;i+=64)
                MD5Transform(context->state,&input[i]);
            index = 0;
        }
        else
        {
            i = 0;
        }
        memcpy(&context->buffer[index],&input[i],inputlen-i);
    }
    void MD5Final(MD5::MD5_CTX *context,u8 digest[16])
    {
        u32 index = 0,padlen = 0;
        u8 bits[8];
        index = (context->count[0] >> 3) & 0x3F;
        padlen = (index < 56)?(56-index):(120-index);
        MD5Encode(bits,context->count,8);
        MD5Update(context,PADDING,padlen);
        MD5Update(context,bits,8);
        MD5Encode(digest,context->state,16);
    }
    void MD5Encode(u8 *output,u32 *input,u32 len)
    {
        u32 i = 0,j = 0;
        while(j < len)
        {
            output[j] = input[i] & 0xFF;
            output[j+1] = (input[i] >> 8) & 0xFF;
            output[j+2] = (input[i] >> 16) & 0xFF;
            output[j+3] = (input[i] >> 24) & 0xFF;
            i++;
            j+=4;
        }
    }
    void MD5Decode(u32 *output,u8 *input,u32 len)
    {
        u32 i = 0,j = 0;
        while(j < len)
        {
            output[i] = (input[j]) |
            (input[j+1] << 8) |
            (input[j+2] << 16) |
            (input[j+3] << 24);
            i++;
            j+=4;
        }
    }
    void MD5Transform(u32 state[4],u8 block[64])
    {
        u32 a = state[0];
        u32 b = state[1];
        u32 c = state[2];
        u32 d = state[3];
        u32 x[64];
        MD5Decode(x,block,64);
        
        /* Round 1 */
        FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
        FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
        FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
        FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
        FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
        FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
        FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
        FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
        FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
        FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
        FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
        FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
        FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
        FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
        FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
        FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
        
        /* Round 2 */
        GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
        GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
        GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
        GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
        GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
        GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
        GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
        GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
        GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
        GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
        GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
        GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
        GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
        GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
        GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
        GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
        
        /* Round 3 */
        HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
        HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
        HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
        HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
        HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
        HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
        HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
        HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
        HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
        HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
        HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
        HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
        HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
        HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
        HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
        HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
        
        /* Round 4 */
        II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
        II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
        II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
        II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
        II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
        II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
        II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
        II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
        II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
        II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
        II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
        II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
        II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
        II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
        II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
        II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
    }
}

_vn_end
