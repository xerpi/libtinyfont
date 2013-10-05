#ifndef common_vram_h
#define common_vram_h

#ifdef __cplusplus
extern "C" {
#endif

/* make a static allocation of vram memory and return pointer relative to vram start */
void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
/* make a static allocation of vram memory and return absolute pointer */
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm);


#ifdef __cplusplus
}
#endif

#endif
