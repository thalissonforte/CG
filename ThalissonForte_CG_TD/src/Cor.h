/*
  FUNCAO QUE IMPLEMENTA O GERENCIAMENTO DAS CORES UTILIZADAS PELO PROGRAMA
*/
#ifndef __COR_H__
#define __COR_H__

#include "gl_canvas2d.h"

class Cor{
  float r, g, b;

public:
  Cor(float _r, float _g, float _b){
     r = _r;
     g = _g;
     b = _b;
  }

  void Aplica(){
      color(r, g, b);
  }

};

/* Algumas cores default */
Cor *cor_zooms = new Cor(0.5,0.3,0);
Cor *verm_fraco = new Cor(1, 0.42745, 0.42745);
Cor *azul_fraco = new Cor(0.42745, 0.7255, 1);
Cor *verde_fraco = new Cor(0.42745, 1, 0.60392);
Cor *cinza = new Cor(0.6078, 0.6078, 0.6078);
Cor *preto = new Cor(0, 0, 0);

#endif
