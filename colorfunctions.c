#include <xc.h>
#include <stdio.h>
#include "colorclick.h"
#include "colorfunctions.h"
#include "dc_motor.h"
#include "main.h"

void colour_read_all(struct RGBC *cf,struct RGB_rel *rel) {
    //stores all absolute color values  in the struct
    float R = color_read_Red();
    float G= color_read_Green();
    float B= color_read_Blue();
    float C= color_read_Clear();
    //calculates total luminosity
    float total=(R)+(G)+(B);
    //stores all absolute values in relevant struct
    (cf->Rf)=R;
    (cf->Bf)=B;
    (cf->Gf)=G;
    (cf->Cf)=C; 
    (rel->R)=R/total;
    (rel->B)=B/total;
    (rel->G)=G/total;
}
//h is hue
void RGB2Hue(struct RGBC *cf){
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
            (cf->h)=60*(((cf->Gf)-(cf->Bf))/(max-min));
        } else if((cf->Gf > cf->Rf)&(cf->Gf > cf->Bf)){
            max=cf->Gf;//most prominent colour is green
            (cf->h)=60*(2+(((cf->Bf)-(cf->Rf))/(max-min)));
        }else{
            max=cf->Bf;//most prominent colour is blue
            (cf->h)=60*(4+(((cf->Rf)-(cf->Gf))/(max-min)));
        }
        if (cf->h < 0){
            cf->h= cf->h +360;
        }
    }
}

char* Hue2Colour(struct RGBC *cf, struct RGB_rel *rel)
{
    char* colourname = "";
    if ((((cf->h)>0)&&((cf->h)<3)|((cf->h)>352)&&((cf->h)<=360)))
    {
        //colourname for serial printing
        colourname = "Red";
        //colourindex can be picked up by other functions
        //to process relevant action
        cf->colourindex=0;
    }
    else if (((cf->h)>14)&&((cf->h)<18))
    {
        colourname = "Pink";
        cf->colourindex=4;
    }
    else if (((cf->h)>85)&&((cf->h)<130))
     {colourname = "Dark Blue";
        cf->colourindex=2;}
    else if (((cf->h)>6)&&((cf->h)<12))
    {
        colourname = "Orange";
        cf->colourindex=5;
    }
     else if (((cf->h)>65)&&((cf->h)<80))
    {
         //Green and blue valeus coincide but easy to differentiate when combined
         //with relative values so use that
        if (rel->B > 0.22)
        {     colourname = "Light Blue";
        cf->colourindex=6;}
        else {
            colourname = "Green";
        cf->colourindex=1; 
        }
    }
    else if (((cf->h)>18)&&((cf->h)<27))
    {
        if (rel->R > 0.51)
        {       
                  colourname = "Yellow";
        cf->colourindex=3;
            }
        else {
            colourname = "White";
       cf->colourindex=7; 
        } 
    }
    else{cf->colourindex=8;}
    return colourname;
}

void Colour2Action(struct RGBC *cf)
{
    if (cf->colourindex == 0)
    {
        Red_R90(&motorL,&motorR);
    }
    else if (cf->colourindex == 1)
    {
        Green_L90(&motorL,&motorR);
    }
    else if (cf->colourindex == 2)
    {
        Blue_T180(&motorL,&motorR);
    }
    else if (cf->colourindex == 3)
    {
        Yellow_rev1_R90(&motorL,&motorR);
    }
    else if (cf->colourindex == 4)
    {
        Pink_rev1_L90(&motorL,&motorR);
    }
    else if (cf->colourindex == 5)
    {
        Orange_R135(&motorL,&motorR);
    }
    else if (cf->colourindex == 6)
    {
        LightBlue_L135(&motorL,&motorR);
    }
    else if (cf->colourindex == 7)
    {
        White(&motorL,&motorR,movementCount,movementMemory,timerMemory);

    }//otherwise unknown colour most likely a black wall
    else {White(&motorL,&motorR,movementCount,movementMemory,timerMemory);}

}