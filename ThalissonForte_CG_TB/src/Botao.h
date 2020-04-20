/*
  FUNCAO QUE IMPLEMENTA OS BOTOES, SUAS CARACTERISTICAS E ACOES
*/
#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

class Botao{
  float altura, largura, x, y;
  char label[100];
  int tam_label;
  Cor *cor;

public:
  Botao(float _x, float _y, float _larg, float _alt, char *_label, Cor *c_or)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     strcpy(label, _label);
    tam_label = strlen(_label);
    cor = c_or;
  }

  void Render()
  {
      cor->Aplica();
      rectFill(x, y, x + largura, y + altura);
      color(1, 1, 1);
      text(x+(largura/2)-tam_label*5, y+altura/2, label); //escreve o label do botao mais ou menos ao centro.
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }

};

  /* Botões utilizados*/
  Botao *btPreto = NULL;
  Botao *btBranco = NULL;

  Botao *btCinzaForte = NULL;
  Botao *btCinza = NULL;

  Botao *btVermelho = NULL;
  Botao *btAmarelo = NULL;

  Botao *btVerde = NULL;
  Botao *btAzul = NULL;

  Botao *btRosa = NULL;
  Botao *btRoxo = NULL;

  Botao *btAddRetangulo = NULL;
  Botao *btAddCirculo = NULL;
  Botao *btAddTriangulo = NULL;
  Botao *btAddHexagono = NULL;

  Botao *btFill = NULL;
  Botao *btRotEsquerda = NULL;
  Botao *btRotDireita = NULL;

  Botao *btFrente = NULL;
  Botao *btTras = NULL;

  Botao *btSalvar = NULL;
  Botao *btSair = NULL;
  Botao *btLimpar = NULL;

#endif
