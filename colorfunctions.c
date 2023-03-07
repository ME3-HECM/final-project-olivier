#include <xc.h>
#include "colorclick.h"
#include "colorfunctions.h"

void colour_read_all(struct RGBC_rel *cf) {
    //stores all absolute color values  in teh struct
    float R = color_read_Red();
    float G= color_read_Green();
    float B= color_read_Blue();
    float C= color_read_Clear();
    //calculates total luminosity
    //int total=(R)+(G)+(B);
    //stores all relative values in relevant struct
    (cf->Rf)=1000*(R/(R+G+B));//rel/255;
    (cf->Bf)=1000*(B/(R+G+B));//rel/255;
    (cf->Gf)=1000*(G/(R+G+B));//rel/255;
}
//h is hue
void RGB2Hue(struct RGBC_rel *cf){
    if ((cf->Rf == cf->Gf)&&(cf->Rf == cf->Bf)){
        cf->h=0;//hue is equal to 0 if there is equal prominence
    } else{//otherwise use hue functions 
        signed int max=0;
        signed int min=0;
        //first find out the least prominent colour for later use
        if ((cf->Rf < cf->Gf)&(cf->Rf < cf->Bf)){
            min=cf->Rf;
        } else if((cf->Gf < cf->Rf)&(cf->Gf < cf->Bf)){
            min=cf->Gf;
        } else{
            min=cf->Bf;
        }
        //Converting to hue depends on the most prominent colour so calculate it
        if ((cf->Rf > cf->Gf)&(cf->Rf > cf->Bf)){
            max=(cf->Rf);//most prominent colour is red
            //calculate hue with formula then multiply by 60 to get out of 360 degrees
            //the extra 10^2 factor is so we have  info of at least 2dp
            (cf->h)=100*(((cf->Gf)-(cf->Bf))/(max-min));
        } else if((cf->Gf > cf->Rf)&(cf->Gf > cf->Bf)){
            max=cf->Gf;//most prominent colour is blue
            cf->h=100*(((cf->Bf)-(cf->Rf))/(max-min));
        }else{
            max=cf->Bf;//most prominent colour is green
            cf->h=100*(4+(((cf->Rf)-(cf->Gf))/(max-min)));
        }
    }
}