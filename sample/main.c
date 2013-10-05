/*
* Copyright (C) 2013, xerpi
*/

#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include "vram.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <tinyfont.h>

PSP_MODULE_INFO("tinyfont sample", 0, 1, 1);

#define millis() (clock()/1000)
#define BUF_WIDTH  (512)
#define SCR_WIDTH  (480)
#define SCR_HEIGHT (272)
#ifndef GU_RGB
	#define GU_RGB(r,g,b) GU_RGBA(r,g,b,255)
#endif
#define _2DEG(x) ((x*180.0f)/PI)
#define _2RAD(x) ((x*PI)/180.0f)

static unsigned int __attribute__((aligned(16))) guList[262144];
void *fbp0 = NULL, *fbp1 = NULL, *zbp = NULL;
u32 frame_count;
time_t currentTime, lastTime, diffTime;
float fps = 0.0f;
int run = 1;

typedef struct
{
	unsigned int color;
	short x, y, z;
}FVertex;

void initFPS();
void FPS(float *fps_p);
int initGU(void);
void SetupProjection(void);
void guBegin(void);
void guEnd(void);

void drawRectangle(int x, int y, int w, int h, unsigned int color);
 
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

char lorem_ipsum[] = {"Lorem ipsum ad his scripta blandit partiendo,\n"
					  "eum fastidii accumsan euripidis in,\n"
					  "eum liber hendrerit an.\n"
					  "Qui ut wisi vocibus suscipiantur,\n"
					  "quo dicit ridens inciderint id.\n"
					  "Quo mundi lobortis reformidans eu,\n"
					  "legimus senserit definiebas an eos.\n"
					  "Eu sit tincidunt incorrupte definitionem,\n"
					  "vis mutat affert percipit cu, eirmod consectetuer\n"
					  "signiferumque eu per. In usu latine equidem dolores.\n"
					  "Quo no falli viris intellegam,\n"
					  "ut fugit veritus placerat per."};

int main(void)
{
	SetupCallbacks();
	initGU();
	SetupProjection();
	initFPS();

	float angle = 0.0f;
	
	
	while(run) {
		guBegin();
		
		tinyfont_draw_string(10, 7,  GU_RGB(255, 0, 0), "libtinyfont by xerpi");
		//tinyfont_draw_string(20, 30,  GU_RGB(0,255,0), "tinyfont sample !! \nhere a new line and \there a tab");
		//tinyfont_draw_string(20, 70, GU_RGB(255,255,0), lorem_ipsum);
		
		tinyfont_draw_string_sinusodial(20, 136, GU_RGB(0,255,0),  
									  50.0f, 1.0f, angle/45.0f,  
									  "I'm a text, oh no I'm a stationary wave, oh no I'm both!");
		
		
		tinyfont_draw_stringf(380, 10,  GU_RGB(0,0,255), "FPS: %.2f", fps);

		guEnd();
		FPS(&fps);
		angle += 0.01;
	}
	
	sceGuTerm();
	sceKernelExitGame();
	return 0;
}


int initGU(void)
{
	fbp0 = (void*)getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	fbp1 = (void*)getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	zbp =  (void*)getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);
	
	sceGuInit();
	sceGuStart(GU_DIRECT, guList);
	
	sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	
	sceGuOffset( 2048 - (SCR_WIDTH/2), 2048 - (SCR_HEIGHT/2));
	sceGuViewport( 2048, 2048, SCR_WIDTH, SCR_HEIGHT);
	sceGuDepthRange(65535, 0);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuDisable(GU_TEXTURE_2D);
		
	//sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuFinish();

	sceGuSync(0, 0);
	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	sceKernelDcacheWritebackAll();  

	return 1;
}

void SetupProjection(void)
{
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1);
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	sceGuClearColor(GU_COLOR(0.0f, 0.0f, 0.0f, 1.0f));
}


void guBegin(void)
{
	sceGuStart(GU_DIRECT, guList);
	sceGuClearColor(0);
	sceGuClearDepth(0);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT );
}


void guEnd(void)
{
	sceGuFinish();
	sceGuSync(0,0);
	fbp0 = sceGuSwapBuffers();
}


void initFPS()
{
	currentTime = millis();
	lastTime = currentTime;
	frame_count = 0;       
}
 
void FPS(float *fps_p)
{
	frame_count++;
	currentTime = millis();
	diffTime = currentTime - lastTime;
	if(diffTime >= 1000) {
			*fps_p = (float)frame_count/(diffTime/1000.0f);
			frame_count = 0;
			lastTime = millis();
	}
}
 

int exit_callback(int arg1, int arg2, void *common)
{
	run = 0;
	return 0;
}
 
int CallbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("exit_callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}
 
int SetupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("callback_update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}
