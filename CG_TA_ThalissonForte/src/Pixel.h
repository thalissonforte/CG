/*
    FUNCÃO QUE ARMAZENA AS INFORMACOES SOBRE OS PIXELS LIDOS NA IMAGEM
*/
#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "gl_canvas2d.h"

class Pixel{
public:
    int cor[3]; // 0 = R, 1 = G, 2 = B
    int x, y;
    
  Pixel(int _r, int _g, int _b, int c_x, int c_y){
     cor[0] = _r;
     cor[1] = _g;
     cor[2] = _b;
     x = c_x;
     y = c_y;
  }

  int getI_R(){
      return cor[0];
  }
  int getI_G(){
      return cor[1];
  }
  int getI_B(){
      return cor[2];
  }

  float getR(){
      return ((float)cor[0]/255);
  }
  float getG(){
      return ((float)cor[1]/255);
  }
  float getB(){
      return ((float)cor[2]/255);
  }


};

#endif
