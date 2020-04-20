/***************************************************************************************************
//  Trabalho 2 da disciplina de Computação Gráfica
//  Author: Thalisson Forte
//
//      Foram implementados todos os requisitos (básicos e alguns extras)
//  para criação e modificação de figuras utilizando a API Canvas 2D
//  implementada pelo professor da disciplina Cesar Tadeu Pozzer.
//
//  Instruções:
//
//      Todos os botões visíveis no programa realizam alterações nas
//  figuras, utilize o mouse para selecionar figuras.
//
//  Na parte inferior encontram-se:
//          - Paleta de cores disponíveis
//          - Botões de rotação (45º) esquerda e direita
//          - Botões de posição (trazer para frente ou levar para trás a figura)
//          - Cor selecionada no momento
//          - Botões de Salvar e Sair
//                (o botão Sair não salva as figuras atuais, certifique-se que salvou
//                   antes de Sair caso não queira perde-las)
//          - Botão Limpar (exclui todas as figuras existentes)
//
//  Na lateral esquerda encontram-se:
//          - Botões de criação de figuras (Retangulo/Quadrado, Circulo, Triangulo e Hexagono)
//          - Botão de preenchimento de cor (próximo a paleta de cores, realiza o preenchimento
//                                           de cor da figura selecionada com a cor selecionada)
//
//
//    Atalhos importantes:
//          SETAS  - Movimenta a figura selecionada
//                (a figura também pode ser movida clicando e arrastando o mouse)
//          DELETE - Exclui a figura selecionada
//          ENTER  - Desseleciona a figura
//          ESC    - Salva a configuração atual e fecha o programa
//          F      - Ativa o modo 'fill' (preenchimento de imagem)
//
// *********************************************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Cor.h"
#include "Botao.h"
#include "Manager.h"
#include "Figura.h"
#include "Bmp.h"
#include "Pixel.h"

/* Manager */
Manager *manager_btn = NULL;

/* Figuras e cores*/
int QTD_FIGURAS = 0;
Figura **figura = NULL;
Cor *corSelecionada = preto;
FILE *arquivo;

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 530; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

/* Imagens Utilizadas */
Bmp *fillImg = NULL;
Bmp *rotEsqImg = NULL;
Bmp *rotDirImg = NULL;
Bmp *trazerFrente = NULL;
Bmp *levarTras = NULL;

// VARIAVEIS PARA CONTROLE DE DISTANCIAS E PROPORCOES
int inicio_botoes_x = 5;
int inicio_botoes_y = screenHeight - 65;
int inicio_botoes_cor_x = 15;
int inicio_botoes_cor_y = 5;

// DEFINICOES DE ALGUMAS FUNCOES UTILIZADAS (COMENTARIOS ADICIONAIS EM CIMA DAS FUNCOES)
void criaRetangulo(float x1, float y1, float x2, float y2, Cor* corz, bool fill_, int rot);
void criaCirculo(float x, float y, float raio, Cor* corz, bool fill_, int rot);
void criaTriangulo(float x1, float y1, float x2, float y2, float x3, float y3, Cor* corz, bool fill_, int rot);
void criaHexagono(float x, float y, float raio, Cor* corz, bool fill_, int rot);
void exibeImg(int offset_X, int offset_Y, Bmp *img);
void salvarArquivo();

// EXIBICAO DA COR SELECIONADA
void exibeCorSelecionada(){
    char str[100];
    int offset_x_ = 400;
    sprintf(str, "Cor selecionada");
    text(screenWidth-offset_x_,45, str);
}

// DESENHO DA TELA
void desenhaTela(){

   // FUNDO
   clear(0.85, 0.85, 0.85);

   // PARTE PRINCIPAL (MEIO)
      // PARTE DESENHAVEL
   color(0.92, 0.92, 0.92);
   rectFill(80,60, screenWidth-30, screenHeight-30);

   color(0.54,0.54,0.54);
   rect(80,60, screenWidth-30, screenHeight-30);

   // COR SELECIONADA
   corSelecionada->Aplica();
   rectFill(screenWidth-380, 5, screenWidth-270, 35);
}

// DESENHO DOS BOTOES DE CRIACAO
void desenhaBotoes(){
    int dist_y = 34;
   // BOTAO RETANGULO
   color(0,0,0);
   rect(inicio_botoes_x+6, inicio_botoes_y+6, inicio_botoes_x+24, inicio_botoes_y+24);

   //BOTAO CIRCULO
   circle(inicio_botoes_x+55, inicio_botoes_y+15, 10, 50);

   //BOTAO TRIANGULO
   float cordX[] = {inicio_botoes_x+6, inicio_botoes_x+24, inicio_botoes_x+15};
   float cordY[] = {inicio_botoes_y-dist_y, inicio_botoes_y-dist_y, inicio_botoes_y-dist_y+17};
   polygon(cordX, cordY, 3);

   // BOTAO HEXAGONO
   float cordX2[] = {inicio_botoes_x+55, inicio_botoes_x+47, inicio_botoes_x+47, inicio_botoes_x+55, inicio_botoes_x+63, inicio_botoes_x+63};
   float cordY2[] = {inicio_botoes_y-dist_y, inicio_botoes_y-dist_y+5, inicio_botoes_y-dist_y+13, inicio_botoes_y-dist_y+18, inicio_botoes_y-dist_y+13, inicio_botoes_y-dist_y+5};
   polygon(cordX2, cordY2, 6);

}

// FUNCAO QUE DESENHA SOBRE O OBJETO SELECIONADO
void desenhaSelecao(){
   if(figura_selecionada == -1) return;
   Figura *fig = figura[figura_selecionada];
   int t = fig->getTipo();

   float *xs = fig->getXsRot();
   float *ys = fig->getYsRot();
   int tam = 3;
   float x_medio, y_medio;

   color(0, 0, 0);
   if(t == 1){
      for(int i=0; i<4; i++){
         x_medio = (xs[i%4] + xs[(i+1)%4]) / 2;
         y_medio = (ys[i%4] + ys[(i+1)%4]) / 2;
         rect(x_medio-tam, y_medio-tam, x_medio+tam, y_medio+tam);
      }
   }
   if(t == 2){
      int raio = fig->getRaio();
      for(int i=0; i<4; i++){
         x_medio = xs[0] + sin(i*PI/2)*raio;
         y_medio = ys[0] + cos(i*PI/2)*raio;
         rect(x_medio-tam, y_medio-tam, x_medio+tam, y_medio+tam);
      }
   }
   if(t == 3){
      for(int i=0; i<3; i++){
         rect(xs[i]-tam, ys[i]-tam, xs[i]+tam, ys[i]+tam);
      }
   }

   if(t == 4){
      for(int i=0; i<6; i++){
         rect(xs[i]-tam, ys[i]-tam, xs[i]+tam, ys[i]+tam);
      }
   }

}

// FUNCAO QUE PRINTA A IMAGEM
void exibeImg(int offset_X, int offset_Y, Bmp *img){

   int alt, lar, inc;

   alt = img->getHeight();
   lar = img->getWidth();
   inc = 1;

   for(int i=0; i < alt; i+=inc){
      for(int j = 0; j < lar; j+=inc){
         color(img->pixel[i][j]->getR(), img->pixel[i][j]->getG(), img->pixel[i][j]->getB());
         point(offset_X + j, offset_Y + i);
      }
   }
}

// FUNCAO DE CONTROLE DE EXIBICAO DE IMAGENS
void exibirImagens(){
   exibeImg(inicio_botoes_cor_x+18, inicio_botoes_cor_y + 67, fillImg);
   exibeImg(inicio_botoes_cor_x + 6*30 + 7, inicio_botoes_cor_y + 7, rotDirImg);
   exibeImg(inicio_botoes_cor_x + 7*30 + 17, inicio_botoes_cor_y + 7, rotEsqImg);
   exibeImg(inicio_botoes_cor_x + 9*30 + 7, inicio_botoes_cor_y + 7, trazerFrente);
   exibeImg(inicio_botoes_cor_x + 10*30 + 17, inicio_botoes_cor_y + 7, levarTras);
}

// FUNCAO QUE REMOVE A FIGURA
void removeFigura(int id){
   Figura *fAux;
   figura_selecionada = -1;
   int idxAux = id;
   Figura *selecionada = figura[id];

   for(int i = id; i < QTD_FIGURAS-1; i++){
      fAux = figura[i+1];
      figura[i] = fAux;
   }

   free(selecionada);
   QTD_FIGURAS--;
   //printf("\nFigura %i removida", id);
}

// FUNCAO QUE MODIFICA A FIGURA (POSICAO E TAMANHO)
void movimentacaoHold(){
   if(modoHold && figura_selecionada != -1){
      if(redimensionar != -1){
         figura[figura_selecionada]->addResize(mouseX - inicioHoldX, mouseY - inicioHoldY, redimensionar);
      }else{
         figura[figura_selecionada]->addX(mouseX - inicioHoldX);
         figura[figura_selecionada]->addY(mouseY - inicioHoldY);
      }

      inicioHoldX = mouseX;
      inicioHoldY = mouseY;
   }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
void render(){

   // DESENHO DA TELA
   desenhaTela();

   // COR SELECIONADA
   preto->Aplica();
   exibeCorSelecionada();

   // RENDER DOS BOTOES
   manager_btn->renderBtns();

   // BOTÕES
   desenhaBotoes();

   // IMAGENS
   exibirImagens();

   // FIGURAS
   for(int i=0; i<QTD_FIGURAS; i++){
      figura[i]->desenhaFigura();
   }

   // MODO FILL
   if(fillSelect) exibeImg(mouseX-8, mouseY-5, fillImg);

   // DESENHO SELECAO
   desenhaSelecao();

   // ALTERACAO NA FIGURA
   movimentacaoHold();

}

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO TECLADO
void keyboard(int key){

   if(key < 200){
      opcao = key;
   }

   switch(key){
      case 27: // ESC
         salvarArquivo();
         exit(0);
      break;

      case 13: // ENTER
         figura_selecionada = -1;
      break;

      case 127: // DELETE
         if(figura_selecionada != -1) removeFigura(figura_selecionada);
      break;

      case 102: // LETRA F
         fillSelect = !fillSelect;
      break;

      case 200: // ESQUERDA
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addX(-10);
         }
      break;

      case 201: // CIMA
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addY(10);
         }
      break;

      case 202:  // DIREITA
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addX(10);
         }
      break;

      case 203:  // BAIXO
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addY(-10);
         }
      break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   //printf("\nLiberou: %d" , key);
}

bool colidiuFiguras(int x, int y){

   if(figura_selecionada != -1){
      if((redimensionar = figura[figura_selecionada]->ColidiuHold(x, y)) != -1){
         //printf("\nRedimensionar %i", redimensionar);
      }
   }

   for(int i=QTD_FIGURAS-1; i>=0; i--){
      if(figura[i]->Colidiu(x, y)){
         //printf("\nColidiu figura[%i]", i);
         figura_selecionada = i;

         if(fillSelect) {
            figura[i]->setFill(corSelecionada);
            fillSelect = false;
            figura_selecionada = -1;
         }

         return true;
      }
   }

   return false;

}

// FUNCAO LIMPAR
void limparFiguras(){
    int quantia = QTD_FIGURAS;
    for(int i=0; i<quantia; i++){
        removeFigura(0);
    }
}

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO MOUSE
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x;
   mouseY = y;

   if (state == 0){ //clicou

      modoHold = true;
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;

      if(btAddRetangulo->Colidiu(x, y)){
         //printf("\nCriando Retangulo\n");
         criaRetangulo(200, 300, 400, 400, corSelecionada, false, 0);
      }

      if(btAddCirculo->Colidiu(x, y)){
         //printf("\nCriando Circulo\n");
         criaCirculo(200, 200, 50, corSelecionada, false, 0);
      }

      if(btAddTriangulo->Colidiu(x, y)){
         //printf("\nCriando Triangulo\n");
         criaTriangulo(550, 300, 600, 400, 650, 300, corSelecionada, false, 0);
      }

      if(btAddHexagono->Colidiu(x, y)){
         //printf("\nCriando Hexagono\n");
         criaHexagono(500, 200, 50, corSelecionada, false, 0);
      }

      if(btRotEsquerda->Colidiu(x, y)){
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addRotacao(-1);
         }
      }

      if(btRotDireita->Colidiu(x, y)){
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addRotacao(1);
         }
      }

      if(btFrente->Colidiu(x, y)){
         if(figura_selecionada != -1){
            Figura *aux = figura[figura_selecionada]; // ULTIMA FIGURA
            for(int i=figura_selecionada; i<QTD_FIGURAS-1; i++){
               figura[i] = figura[i+1];
            }
            figura[QTD_FIGURAS-1] = aux;
            figura_selecionada = QTD_FIGURAS-1;
         }
      }

      if(btTras->Colidiu(x, y)){
         if(figura_selecionada != -1){
            Figura *aux = figura[figura_selecionada]; // PRIMEIRA FIGURA
            for(int i=figura_selecionada; i>0; i--){
               figura[i] = figura[i-1];
            }
            figura[0] = aux;
            figura_selecionada = 0;
         }
      }

      if(btSalvar->Colidiu(x, y)){
         salvarArquivo();
         printf("\nFiguras salvas.");
      }

      if(btSair->Colidiu(x, y)){
         exit(0);
      }

      if(btLimpar->Colidiu(x, y)){
         limparFiguras();
      }

       if(colidiuFiguras(x, y)){


       }else if( btPreto->Colidiu(x, y)){
         corSelecionada = preto;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(preto);
       }else if( btBranco->Colidiu(x, y)){
          corSelecionada = branco;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(branco);
       }else if( btCinza->Colidiu(x, y)){
         corSelecionada = cinza;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(cinza);
       }else if( btCinzaForte->Colidiu(x, y)){
         corSelecionada = cinzaForte;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(cinzaForte);
       }else if( btVermelho->Colidiu(x, y)){
         corSelecionada = vermelho;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(vermelho);
       }else if( btAmarelo->Colidiu(x, y)){
         corSelecionada = amarelo;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(amarelo);
       }else if( btVerde->Colidiu(x, y)){
         corSelecionada = verde;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(verde);
       }else if( btAzul->Colidiu(x, y)){
         corSelecionada = azul;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(azul);
       }else if( btRosa->Colidiu(x, y)){
         corSelecionada = rosa;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(rosa);
       }else if( btRoxo->Colidiu(x, y)){
         corSelecionada = roxo;
         if(figura_selecionada != -1) figura[figura_selecionada]->setCor(roxo);
       }else{
          if(fillSelect) fillSelect = false;
       }

      if(btFill->Colidiu(x, y)){
         fillSelect = !fillSelect;
       }

   }
   if(state == 1){
      // SOLTOU
      modoHold = false;
      redimensionar = -1;
   }
}

// FUNCAO QUE INICIALIZA OS BOTOES UTILIZADOS
void inicializaBotoes(){
   int espaco_entre = 10, larg_btn = 30, larg_btn_cor = 20;
   int offset_cor = larg_btn_cor + espaco_entre;

   // MANAGER
   manager_btn = new Manager();

   // BOTOES CRIACAO
   btAddRetangulo = new Botao(inicio_botoes_x, inicio_botoes_y, larg_btn, larg_btn, "", cor_zooms);
   btAddCirculo = new Botao(inicio_botoes_x+larg_btn+espaco_entre, inicio_botoes_y, larg_btn, larg_btn, "", cor_zooms);
   btAddTriangulo = new Botao(inicio_botoes_x, inicio_botoes_y-larg_btn-espaco_entre, larg_btn, larg_btn, "", cor_zooms);
   btAddHexagono = new Botao(inicio_botoes_x+larg_btn+espaco_entre, inicio_botoes_y-larg_btn-espaco_entre, larg_btn, larg_btn, "", cor_zooms);

   // BOTOES FERRAMENTAS
   btFill = new Botao(inicio_botoes_x+0.7*larg_btn, inicio_botoes_cor_y + 2*larg_btn, larg_btn, larg_btn, "", cor_zooms);
   btRotDireita = new Botao(inicio_botoes_cor_x + 7*offset_cor + espaco_entre, inicio_botoes_cor_y, larg_btn, larg_btn, "", cor_zooms);
   btRotEsquerda = new Botao(inicio_botoes_cor_x + 6*offset_cor, inicio_botoes_cor_y, larg_btn, larg_btn, "", cor_zooms);
   btFrente = new Botao(inicio_botoes_cor_x + 9*offset_cor, inicio_botoes_cor_y, larg_btn, larg_btn, "", cor_zooms);
   btTras = new Botao(inicio_botoes_cor_x + 10*offset_cor + espaco_entre, inicio_botoes_cor_y, larg_btn, larg_btn, "", cor_zooms);
   btLimpar = new Botao(550, inicio_botoes_cor_y - 2, larg_btn + 40, larg_btn - 5, "Limpar", preto);

   // BOTOES FINALIZACAO
   btSalvar = new Botao(630, inicio_botoes_cor_y + offset_cor - 2, larg_btn + 40, larg_btn - 5, "Salvar", cinzaForte);
   btSair = new Botao(630, inicio_botoes_cor_y - 2, larg_btn + 40, larg_btn - 5, "Sair", cinzaForte);


   // BOTOES DE COR
   btPreto = new Botao(inicio_botoes_cor_x, inicio_botoes_cor_y, larg_btn_cor, larg_btn_cor, "", preto);
   btBranco = new Botao(inicio_botoes_cor_x, inicio_botoes_cor_y + offset_cor, larg_btn_cor, larg_btn_cor, "", branco);

   btCinzaForte = new Botao(inicio_botoes_cor_x + offset_cor, inicio_botoes_cor_y, larg_btn_cor, larg_btn_cor, "", cinzaForte);
   btCinza = new Botao(inicio_botoes_cor_x + offset_cor, inicio_botoes_cor_y + offset_cor, larg_btn_cor, larg_btn_cor, "", cinza);

   btVermelho = new Botao(inicio_botoes_cor_x + 2*offset_cor, inicio_botoes_cor_y, larg_btn_cor, larg_btn_cor, "", vermelho);
   btAmarelo = new Botao(inicio_botoes_cor_x+ 2*offset_cor, inicio_botoes_cor_y + offset_cor, larg_btn_cor, larg_btn_cor, "", amarelo);

   btVerde = new Botao(inicio_botoes_cor_x + 3*offset_cor, inicio_botoes_cor_y, larg_btn_cor, larg_btn_cor, "", verde);
   btAzul = new Botao(inicio_botoes_cor_x + 3*offset_cor, inicio_botoes_cor_y + offset_cor, larg_btn_cor, larg_btn_cor, "", azul);

   btRosa = new Botao(inicio_botoes_cor_x + 4*offset_cor, inicio_botoes_cor_y, larg_btn_cor, larg_btn_cor, "", rosa);
   btRoxo = new Botao(inicio_botoes_cor_x + 4*offset_cor, inicio_botoes_cor_y + offset_cor, larg_btn_cor, larg_btn_cor, "", roxo);

   // MANAGER ADD BTNS
   manager_btn->addBtn(btAddRetangulo);
   manager_btn->addBtn(btAddCirculo);
   manager_btn->addBtn(btAddTriangulo);
   manager_btn->addBtn(btAddHexagono);

   manager_btn->addBtn(btFill);
   manager_btn->addBtn(btRotDireita);
   manager_btn->addBtn(btRotEsquerda);
   manager_btn->addBtn(btFrente);
   manager_btn->addBtn(btTras);

   manager_btn->addBtn(btSalvar);
   manager_btn->addBtn(btSair);
   manager_btn->addBtn(btLimpar);

   manager_btn->addBtn(btPreto);
   manager_btn->addBtn(btBranco);

   manager_btn->addBtn(btCinzaForte);
   manager_btn->addBtn(btCinza);

   manager_btn->addBtn(btVermelho);
   manager_btn->addBtn(btAmarelo);

   manager_btn->addBtn(btVerde);
   manager_btn->addBtn(btAzul);

   manager_btn->addBtn(btRosa);
   manager_btn->addBtn(btRoxo);
}

// FUNCOES DE CRIACAO DE FIGURAS
void criaCirculo(float x, float y, float raio, Cor* corz, bool fill_, int rot){
   float cord[] = {x, y, raio};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(2, cord, 3, corz, fill_, rot);
   QTD_FIGURAS++;
}
void criaRetangulo(float x1, float y1, float x2, float y2, Cor* corz, bool fill_, int rot){
   float cord[] = {x1, y1, x1, y2, x2, y2, x2, y1};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(1, cord, 8, corz, fill_, rot);
   QTD_FIGURAS++;
}
void criaTriangulo(float x1, float y1, float x2, float y2, float x3, float y3, Cor* corz, bool fill_, int rot){
   float cord[] = {x1, y1, x2, y2, x3, y3};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(3, cord, 6, corz, fill_, rot);
   QTD_FIGURAS++;
}
void criaHexagono(float x, float y, float raio, Cor* corz, bool fill_, int rot){
   float cord[] = {x, y, raio};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(4, cord, 3, corz, fill_, rot);
   QTD_FIGURAS++;
}
///////////////////////////////////

// FUNCAO QUE SALVA AS FIGURAS EM UM ARQUIVO
void salvarArquivo(){
   if(QTD_FIGURAS > 0){
      if((arquivo = fopen("figuras.gr", "w")) == NULL){ /* Abre arquivo binário para escrita */
         printf("Erro na abertura do arquivo");
         exit(1);
      }
      fprintf(arquivo, "%i\n", QTD_FIGURAS);

      for(int i = 0 ; i < QTD_FIGURAS; i++){
         fprintf(arquivo, "%i ", figura[i]->getTipo());
         fprintf(arquivo, "%i ", figura[i]->getQntCoords());

         for(int j = 0; j<(figura[i]->getQntCoords())/2; j++){
            if(figura[i]->getTipo() == 4){
               fprintf(arquivo, "%f ", (figura[i]->getCoord())[0]);
               fprintf(arquivo, "%f ", (figura[i]->getCoord())[1]);
            }else{
               fprintf(arquivo, "%f ", (figura[i]->getXs())[j]);
               fprintf(arquivo, "%f ", (figura[i]->getYs())[j]);
            }
         }

         if(figura[i]->getTipo() == 2 || figura[i]->getTipo() == 4){
            fprintf(arquivo, "%i ", figura[i]->getRaio());
         }

         fprintf(arquivo, "%f ", figura[i]->getCor()->getR());
         fprintf(arquivo, "%f ", figura[i]->getCor()->getG());
         fprintf(arquivo, "%f ", figura[i]->getCor()->getB());

         fprintf(arquivo, "%i ", figura[i]->getFill());
         fprintf(arquivo, "%i\n", figura[i]->getRotacao());
      }

      fclose(arquivo);
   }
}

// FUNCAO QUE INICIALIZA AS IMAGENS UTILIZADAS NO PROGRAMA
void inicializaImagens(){
   fillImg = new Bmp(".\\ThalissonForte_CG_TB\\resources\\fill.bmp");
   fillImg->convertBGRtoRGB();

   rotDirImg = new Bmp(".\\ThalissonForte_CG_TB\\resources\\rot_direita.bmp");
   rotDirImg->convertBGRtoRGB();

   rotEsqImg = new Bmp(".\\ThalissonForte_CG_TB\\resources\\rot_esquerda.bmp");
   rotEsqImg->convertBGRtoRGB();

   trazerFrente = new Bmp(".\\ThalissonForte_CG_TB\\resources\\trazer_frente.bmp");
   trazerFrente->convertBGRtoRGB();

   levarTras = new Bmp(".\\ThalissonForte_CG_TB\\resources\\levar_tras.bmp");
   levarTras->convertBGRtoRGB();
}

void inicializaFiguras(){
   figura = (Figura**)malloc(sizeof(Figura*)); //CRIANDO VETOR
}

// FUNCAO QUE REALIZA A LEITURA DO ARQUIVO
void leituraArquivo(){
   if((arquivo = fopen("figuras.gr", "r")) == NULL){ /* Abre arquivo binário para escrita */
      printf("Erro na abertura ou arquivo inexistente");
   }else{
      int qtd_figuras_salvas = 0;
      int tipo, qtd_coords, fillM, rot;
      float coords[12], r, g, b;
      bool fillB;
      fscanf(arquivo, "%i", &qtd_figuras_salvas);
      printf("Quantidade figuras carregadas: %i\n", qtd_figuras_salvas);

      for(int i = 0 ; i < qtd_figuras_salvas; i++){
         fscanf(arquivo, "%i", &tipo);
         fscanf(arquivo, "%i", &qtd_coords);
         for(int j=0; j<qtd_coords; j++){
            fscanf(arquivo, "%f", &(coords[j]));
         }
         fscanf(arquivo, "%f", &r);
         fscanf(arquivo, "%f", &g);
         fscanf(arquivo, "%f", &b);
         fscanf(arquivo, "%i", &fillM);
         fscanf(arquivo, "%i", &rot);

         if(fillM == 1) fillB = true;
         else fillB = false;

         if(tipo == 1){
            criaRetangulo(coords[0], coords[1], coords[4], coords[5], new Cor(r, g, b), fillB, rot);
         }
         if(tipo == 2){
            criaCirculo(coords[0], coords[1], coords[2], new Cor(r, g, b), fillB, rot);
         }
         if(tipo == 3){
            criaTriangulo(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5], new Cor(r, g, b), fillB, rot);
         }
         if(tipo == 4){
            criaHexagono(coords[0], coords[1], coords[2], new Cor(r, g, b), fillB, rot);
         }

      }
      fclose(arquivo);
   }
}

// MODO DE USO
void modo_uso(){
    system("CLS");
    printf("\n******************* INFORMACOES *******************\n");
   printf("\nUtilize os botoes disponiveis para criar e editar figuras\n");
   printf("(para mais informacoes sobre os botoes consulte o \n\theader do arquivo 'main.cpp')\n\n");


   printf("\nAtalhos importantes: \n");
   printf("\nSETAS  - Movimenta a figura selecionada");
   printf("\nDELETE - Exclui a figura selecionada");
   printf("\nENTER  - Desseleciona a figura");
   printf("\nESC    - Salva a configuracaoo atual e fecha o programa");
   printf("\nF      - Ativa o modo 'fill' (preenchimento de imagem)\n");
   //printf("\n2 - Utilize os numeros para alterar a imagem.");
}

int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // INICIALIZANDO VETOR DE FIGURAS
   inicializaFiguras();

   // LEITURA ARQUIVO
   leituraArquivo();

   // IMAGENS
   inicializaImagens();

   // MODO DE USO
   modo_uso();

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Manipulacao de Figuras");

   runCanvas();
}


