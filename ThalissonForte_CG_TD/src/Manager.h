/*
  FUNÇÃO QUE IMPLEMENTA O GERENCIAMENTO DOS BOTOES
*/

#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "gl_canvas2d.h"

class Manager{
  Botao **btn;
  int qtd_btn;

public:
  Manager(){
    btn = (Botao**)malloc(sizeof(Botao*));
    qtd_btn = 0;
  }

  void addBtn(Botao *bt){
      qtd_btn += 1;
      btn = (Botao**)realloc(btn, qtd_btn*sizeof(Botao*));
      btn[qtd_btn-1] = bt;
  }

  void renderBtns(){
      for(int i=0; i<qtd_btn; i++){
          btn[i]->Render();
      }
  }

};

#endif
