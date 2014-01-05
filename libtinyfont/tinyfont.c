/*
* Copyright (C) 2013, xerpi
*/


#include "tinyfont.h"
#include <pspgu.h>
#include <psputils.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern const char vincent_font_data[];
static Vertex3S __attribute__((aligned(16))) _tinyfont_static_vertices[8 * 8];

static void _tinyfont_draw_gu_char(int x, int y, unsigned int color, char c)
{
    if(c == ' ' || x > 480 || y > 272 || x < -8 || y < -8) return;
    int offset_pos = c * 8;
    
    int i, j, b, count = 0;
    for(i = 0; i < 8; i++) {
        b = vincent_font_data[offset_pos + i];
        for(j = 0; j < 8; j++) {
            if(b & (1<<j)) {
                _tinyfont_static_vertices[count] = (Vertex3S){(short)(x+(7-j)), (short)(y+i), 0};
                ++count;
            }
        }
    }
    Vertex3S *vertices = sceGuGetMemory(count * sizeof(Vertex3S));
    memcpy(vertices, _tinyfont_static_vertices, count * sizeof(Vertex3S));
    sceGuDrawArray(GU_POINTS, GU_VERTEX_16BIT|GU_TRANSFORM_2D, count, 0, vertices);	
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
