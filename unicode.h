#ifndef CARR_UNICODE_H_
#define CARR_UNICODE_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool     is_utf8_head(uint8_t ch);
size_t   utf8_len(uint8_t b);
int      utf8_to_utf32(uint8_t* in, uint32_t* out);
uint32_t utf32_from_utf8(uint8_t* in);
int      utf32_to_utf8(uint32_t in, uint8_t* out);


#ifdef CARR_UNICODE_IMPLEMENTATION

bool is_utf8_head(uint8_t ch)
{
    return ch >> 7 & 1 && ch >> 6 != 0b10;
}

size_t utf8_len(uint8_t b)
{
    size_t s = 0;
    while (s < 4) if (b >> (7 - s) & 1) s += 1; else break;
    return s ? s : 1;
}

int utf8_to_utf32(uint8_t* in, uint32_t* out)
{
    size_t utf8len = utf8_len(*in);
    *out = 0;
    switch (utf8len) {
        case 1: {
            *out = in[0] & 0b01111111;
        } break;
        case 2: {
            *out |= (in[0] & 0b11111 ) << 6*1 ;
            *out |= (in[1] & 0b111111) << 6*0 ;
        } break;
        case 3: {
            *out |= (in[0] & 0b1111  ) << 6*2 ;
            *out |= (in[1] & 0b111111) << 6*1 ;
            *out |= (in[2] & 0b111111) << 6*0 ;
        } break;
        case 4: {
            *out |= (in[0] & 0b111   ) << 6*3 ;
            *out |= (in[1] & 0b111111) << 6*2 ;
            *out |= (in[2] & 0b111111) << 6*1 ;
            *out |= (in[3] & 0b111111) << 6*0 ;
        } break;
    }

    return 1;
}

uint32_t utf32_from_utf8(uint8_t* in)
{
    uint32_t ret = 0;
    int res = utf8_to_utf32(in, &ret);
    return ret == -1 ? 0 : ret;
}


int utf32_to_utf8len(uint32_t in)
{
    if (0x0000   <= in && in <= 0x007F)   return 1;
    if (0x0080   <= in && in <= 0x07FF)   return 2;
    if (0x0800   <= in && in <= 0x0FFF)   return 3;
    if (0x1000   <= in && in <= 0xCFFF)   return 3;
    if (0xD000   <= in && in <= 0xD7FF)   return 3;
    if (0xE000   <= in && in <= 0xFFFF)   return 3;
    if (0x10000  <= in && in <= 0x3FFFF)  return 4;
    if (0x40000  <= in && in <= 0xFFFFF)  return 4;
    if (0x100000 <= in && in <= 0x10FFFF) return 4;
    return -1;
}

int utf32_to_utf8(uint32_t in, uint8_t* out)
{
    int utf8_len = utf32_to_utf8len(in);
    if (utf8_len == -1) return -1;

    switch (utf8_len) {
        case 1: {
            out[0] = in & 0b01111111;
            out[1] = 0b0;
        } break;
        case 2: {
            out[0] = 0b11000000 | ( (in >> 6*1) & 0b11111  ); 
            out[1] = 0b10000000 | ( (in >> 6*0) & 0b111111 );
            out[2] = 0b0;
        } break;
        case 3: {
            out[0] = 0b11100000 | ( (in >> 6*2) & 0b1111   );
            out[1] = 0b10000000 | ( (in >> 6*1) & 0b111111 );
            out[2] = 0b10000000 | ( (in >> 6*0) & 0b111111 );
            out[3] = 0b0;
        } break;
        case 4: {
            out[0] = 0b11110000 | ( (in >> 6*3) & 0b111    );
            out[1] = 0b10000000 | ( (in >> 6*2) & 0b111111 );
            out[2] = 0b10000000 | ( (in >> 6*1) & 0b111111 );
            out[3] = 0b10000000 | ( (in >> 6*0) & 0b111111 );
            out[4] = 0b0;
        } break;
    }
    return utf8_len;
}

#endif // CARR_UNICODE_IMPLEMENTATION
#endif // CARR_UNICODE_H_
