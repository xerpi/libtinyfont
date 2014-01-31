/*
* Copyright (C) 2014, xerpi
*/


#ifndef _TINYFONT_H_
#define _TINYFONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI
	#define PI (3.14159265f)
#endif
#ifndef _2PI
	#define _2PI (6.28318531f)
#endif


void tinyfont_draw_char(int x, int y, unsigned int color, char c);
void tinyfont_draw_string(int x, int y, unsigned int color, const char *string);
void tinyfont_draw_stringf(int x, int y, unsigned int color, const char *s, ...);

void tinyfont_draw_char16x16(int x, int y, unsigned int color, char c);
void tinyfont_draw_string16x16(int x, int y, unsigned int color, const char *string);
void tinyfont_draw_stringf16x16(int x, int y, unsigned int color, const char *s, ...);


void tinyfont_draw_string_sinusodial(int start_x, int start_y, unsigned int color,
									  float amplitude, float frequency, float lambda,
									  float time, const char *string);
void tinyfont_draw_stringf_sinusodial(int start_x, int start_y, unsigned int color,
									  float amplitude, float frequency, float lambda,
									  float time, const char *s, ...);

#ifdef __cplusplus
}
#endif

#endif
