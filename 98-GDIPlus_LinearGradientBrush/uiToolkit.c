#include <uiToolkit.h>

DWORD uiToolkit_ARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue){
    return ((alpha<<24)|(red<<16)|(green<<8)|blue);
}

