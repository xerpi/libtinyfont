/*
* Copyright (C) 2014, xerpi
*/


#include "tinyfont.h"
#include <pspgu.h>
#include <psputils.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern const unsigned char msx_font[];

typedef struct
{
	short x, y, z;
} TinyFont_Vertex;


static void _tinyfont_draw_gu_char(int x, int y, unsigned int color, char c)
{
    //if(c == ' ' || x > 480 || y > 272 || x < -8 || y < -8) return;
    if(c == ' ') return;
    unsigned char *glyph = (unsigned char*)(msx_font + c * 8);
    int i, j;
    for (i = 0; i < 8; ++i, ++glyph) {
        for (j = 0; j < 8; ++j) {
            if ((*glyph & (128 >> j))) {
                TinyFont_Vertex *vertex = sceGuGetMemory(sizeof(TinyFont_Vertex));
                *vertex = (TinyFont_Vertex){x+j, y+i, 0};
                sceGuDrawArray(GU_POINTS, GU_VERTEX_16BIT|GU_TRANSFORM_2D, 1, 0, vertex);	   
            }
        }
    }
}

void tinyfont_draw_char(int x, int y, unsigned int color, char c)
{
	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	_tinyfont_draw_gu_char(x, y, color, c);
}

void tinyfont_draw_string(int x, int y, unsigned int color, const char *string)
{
    if(string == NULL) return;
    int startx = x;
    const char *s = string;
    sceGuDisable(GU_TEXTURE_2D);
    sceGuColor(color);
    while(*s) {
        if(*s == '\n') {
            x = startx;
            y+=8;
        } else if(*s == '\t') {
            x+=8*4;
        } else {
            _tinyfont_draw_gu_char(x, y, color, *s);
            x+=8;
        }
        ++s;
    }
}


void tinyfont_draw_stringf(int x, int y, unsigned int color, const char *s, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, s);
    vsnprintf(buffer, 256, s, args);
    tinyfont_draw_string(x, y, color, buffer);
    va_end(args);
}


void tinyfont_draw_char16x16(int x, int y, unsigned int color, char c)
{
    if(c == ' ') return;
    unsigned char *glyph = (unsigned char*)(msx_font + c * 8);
    int i, j;
    for (i = 0; i < 8; ++i, ++glyph) {
        for (j = 0; j < 8; ++j) {
            if ((*glyph & (128 >> j))) {
                TinyFont_Vertex *vertex = sceGuGetMemory(4 * sizeof(TinyFont_Vertex));
                register int i2 = i*2, j2 = j*2; 
                vertex[0] = (TinyFont_Vertex){x+j2,   y+i2,   0};
                vertex[1] = (TinyFont_Vertex){x+j2+1, y+i2,   0};
                vertex[2] = (TinyFont_Vertex){x+j2,   y+i2+1 ,0};
                vertex[3] = (TinyFont_Vertex){x+j2+1, y+i2+1, 0};
                sceGuDrawArray(GU_POINTS, GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertex);	
            }
        }
    }
}

void tinyfont_draw_string16x16(int x, int y, unsigned int color, const char *string)
{
    if(string == NULL) return;
    int startx = x;
    const char *s = string;
    while(*s) {
        if(*s == '\n') {
            x = startx;
            y+=16;
        } else if(*s == '\t') {
            x+=16*4;
        } else {
            tinyfont_draw_char16x16(x, y, color, *s);
            x+=16;
        }
        ++s;
    }
}

void tinyfont_draw_stringf16x16(int x, int y, unsigned int color, const char *s, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, s);
    vsnprintf(buffer, 256, s, args);
    tinyfont_draw_string16x16(x, y, color, buffer);
    va_end(args);
}

void tinyfont_draw_string_sinusodial(int start_x, int start_y, unsigned int color,
                                     float amplitude, float frequency, float lambda,
                                     float time, const char *string)
{
	
    if(string == NULL || lambda == 0.0f) return;
    const char *s = string;
    //float length = strlen(string) * 8.0f;
    float x = 0.0f, y = 0.0f;
    float _2A = 2.0f * amplitude;
    float k = _2PI/lambda;
    float omega = _2PI*frequency;
    
    sceGuDisable(GU_TEXTURE_2D);
    sceGuColor(color);
    while(*s) {
        if (*s != '\n' && *s != '\t') {
            y = _2A*cosf(k*x)*sinf(omega*time);
            _tinyfont_draw_gu_char(start_x + x, start_y + y, color, *s);
            x+=8;
        }
        ++s;
    }
}
									  

void tinyfont_draw_stringf_sinusodial(int start_x, int start_y, unsigned int color,
                                      float amplitude, float frequency, float lambda,
                                      float time, const char *s, ...)
{
	
    char buffer[256];
    va_list args;
    va_start(args, s);
    vsnprintf(buffer, 256, s, args);
    tinyfont_draw_string_sinusodial(start_x, start_y, color, amplitude, frequency, lambda, time, buffer);
    va_end(args);	
}
