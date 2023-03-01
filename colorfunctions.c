#include <xc.h>
#include "colorclick.h"
#include "i2c.h"
#include "colorfunctions.h"

void colour_read_all(struct RGBC_rel *cf) {
    //stores all absolute color values  in teh struct
    float R = color_read_Red();
    float G = color_read_Green();
    float B = color_read_Blue();
    float C = color_read_Clear();
    //calculates total luminosity
    int total=(R)+(G)+(B)+(C);
    //stores all relative values in relevant struct
    float rel= ((R) * 10000);
    (cf->Rf)=rel/total;
    rel= ((B) * 10000);
    (cf->Bf)=rel/total;
     rel= ((G) * 10000);
    (cf->Gf)=rel/total;
     rel= ((C) * 10000);
    (cf->Cf)=rel/total;
}

void RGB2Hue(void){
    
}