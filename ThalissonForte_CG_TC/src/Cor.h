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

  float getR(){
    return r;
  }

  float getG(){
    return g;
  }

  float getB(){
    return b;
  }

};
// Vetor Cores
Cor **cores = NULL;

/* Algumas cores default */
Cor *preto = new Cor(0, 0, 0);
Cor *branco = new Cor(1, 1, 1);

Cor *cinzaForte = new Cor(0.4, 0.4, 0.4);
Cor *cinza = new Cor(0.75, 0.75, 0.75);

Cor *vermelho = new Cor(1, 0, 0);
Cor *amarelo = new Cor(1, 1, 0);

Cor *verde = new Cor(0, 1, 0);
Cor *azul = new Cor(0, 0, 1);

Cor *rosa = new Cor(1, 0, 1);
Cor *roxo = new Cor(0.5, 0, 1);

Cor *cor_zooms = new Cor(0.95,0.95,0.95);
Cor *azul_fraco = new Cor(0.42745, 0.7255, 1);




#endif
