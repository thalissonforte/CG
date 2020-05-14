/*
  FUNCAO QUE IMPLEMENTA OS PONTOS
*/
#ifndef __PONTO_H__
#define __PONTO_H__

#include "gl_canvas2d.h"

class Ponto{
  int x, y, id, raio;

public:
  Ponto(int _id, int _x, int _y){
     id = _id;
     x = _x;
     y = _y;
     raio = 4;
  }

    // EXIBIR PONTOS DE CONTROLE
  void Render(){

      color(0,0,0);
      circle(x, y, raio, 100);
  
  }

  // VERIFICAR CLICK SOBRE PONTO
  int Colidiu(int mx, int my){
    if( (pow((mx - x), 2) + pow((my - y), 2)) < pow(raio, 2) ){
        return id;
    }

    return -1;
  }

  int getX(){
    return x;
  }

  int getY(){
    return y;
  }     

  void setX(int _x){
      x = _x;
  }

  void setY(int _y){
    y = _y;
  }

  void addX(int _x){
    x += _x;
  }

  void addY(int _y){
    y += _y;
  }


};

#endif
