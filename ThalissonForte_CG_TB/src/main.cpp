/*********************************************************************
//  Trabalho 1 da disciplina de Computação Gráfica
//  Author: Thalisson Forte
//
//      Foram implementados todos os requisitos (básicos e alguns
//  extras) para a modificação das cores e tamanho de imagens BMP
//  utilizando a API Canvas 2D implementado pelo professor da
//  disciplina Cesar Tadeu Pozzer.
//
//  Instruções:

//      Todos os botões visíveis no programa realizam alterações na
//  imagem em questão, cada um com sua ação sobre o BMP carregado.
//
//      Para alterar a imagem utilize os números (1, 2, 3 ou 4)
//
//      Para rotacionar a imagem utilize as setas
// *********************************************************************/

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

/* Figuras */
int QTD_FIGURAS = 0;
Figura **figura = NULL;

/* Botões */
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

Botao *btFill = NULL;
Botao *btRotEsquerda = NULL;
Botao *btRotDireita = NULL;

int figura_selecionada = -1;

Cor *corSelecionada = preto;

Bmp *fillImg = NULL;
bool fillSelect = false;
bool modoHold = false;
int inicioHoldX = -1;
int inicioHoldY = -1;
/* Cores atuais
int exibe_red = 1;
int exibe_green = 1;
int exibe_blue = 1;
int exibe_mono = 0;
long int maximo = 0;
*/

void criaRetangulo(float x1, float y1, float x2, float y2, Cor* corz, bool fill_);
void criaCirculo(float x, float y, float raio, Cor* corz, bool fill_);
void exibeImg(int offset_X, int offset_Y, Bmp *img);

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 530; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int inicio_botoes_x = 5;
int inicio_botoes_y = screenHeight - 65;
int inicio_botoes_cor_x = 15;
int inicio_botoes_cor_y = 5;

void DrawMouseScreenCoords(){
    char str[100];
    int offset_x_ = 400;
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    text(screenWidth-180,10, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    text(screenWidth-190,30, str);

    sprintf(str, "Cor selecionada");
    text(screenWidth-400,45, str);
    //sprintf(str, "Utilize as setas para rotacionar a imagem");
    //text(50,30,str);
    //sprintf(str, "Utilize os numeros para alterar a imagem");
    //text(50,50,str);
}

float distanciaPontos(float x1, float y1, float x2, float y2){
   return (sqrt(pow((x2-x1),2) + pow((y2-y1),2)));
}

void desenhaTela(){

   // FUNDO
   clear(0.85, 0.85, 0.85);

   // PARTE PRINCIPAL (MEIO)
      // PARTE DESENHAVEL
   color(1,1,1);
   rectFill(80,60, screenWidth-30, screenHeight-30);


   color(0.54,0.54,0.54);
   rect(80,60, screenWidth-30, screenHeight-30);

   // COR SELECIONADA
   corSelecionada->Aplica();
   rectFill(screenWidth-380, 5, screenWidth-270, 35);
}

void desenhaBotoes(){
   // BOTAO RETANGULO
   color(0,0,0);
   rect(inicio_botoes_x+6, inicio_botoes_y+6, inicio_botoes_x+24, inicio_botoes_y+24);

   //BOTAO CIRCULO
   circle(inicio_botoes_x+55, inicio_botoes_y+15, 10, 50);
}

void desenhaSelecao(){
   if(figura_selecionada == -1) return;
   Figura *fig = figura[figura_selecionada];
   int t = fig->getTipo();

   float *xs = fig->getXs();
   float *ys = fig->getYs();
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

}

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

   printf("\nFigura %i removida", id);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render(){

   //if(screenWidth != 780) screenWidth = 780;
   //if(screenHeight != 530) screenHeight = 530;

   desenhaTela();

   preto->Aplica();
   DrawMouseScreenCoords();

   manager_btn->renderBtns();

   // BOTÕES
   desenhaBotoes();
   exibeImg(13, 432, fillImg);
   
   for(int i=0; i<QTD_FIGURAS; i++){
      figura[i]->desenhaFigura();
   }

   if(fillSelect) exibeImg(mouseX-8, mouseY-5, fillImg);

   // DESENHO SELECAO
   desenhaSelecao();
   
   if(modoHold && figura_selecionada != -1){
      figura[figura_selecionada]->addX(mouseX - inicioHoldX);
      figura[figura_selecionada]->addY(mouseY - inicioHoldY);
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;
   }

}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key){
   //printf("\nTecla: %d" , key);
   if( key < 200 ){
      opcao = key;
   }

   switch(key){
      case 27: // ESC

         exit(0);
      break;

      case 49: // 1
         //carregamento_imagem(1);
      break;
      case 50: // 2
         //carregamento_imagem(2);
      break;
      case 51: // 3
         //carregamento_imagem(3);
      break;
      case 52: // 4
         //carregamento_imagem(4);
      break;

      case 200: // SETA P ESQUERDA
         //img1->setRotacao(3);
      break;

      case 201: // SETA P CIMA
         //img1->setRotacao(2);
      break;

      case 202: // SETA P DIREITA
         //img1->setRotacao(1);
      break;

      case 203: // SETA P BAIXO
         //img1->setRotacao(0);
      break;

      case 127: // DELETE
         if(figura_selecionada != -1) removeFigura(figura_selecionada);
      break;


   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   printf("\nLiberou: %d" , key);
}

bool colidiuFiguras(int x, int y){

   for(int i=QTD_FIGURAS-1; i>=0; i--){
      if(figura[i]->Colidiu(x, y)){
         printf("\nColidiu figura[%i]", i);
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

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if (state == 0){ //clicou
      
      modoHold = true;
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;

       if(btAddRetangulo->Colidiu(x, y)){
         printf("\nCriando Retangulo\n");
         criaRetangulo(300, 300, 500, 400, corSelecionada, false);
       }

       if(btAddCirculo->Colidiu(x, y)){
         printf("\nCriando Circulo\n");
         criaCirculo(200, 200, 50, corSelecionada, false);
       }

       if(btRotEsquerda->Colidiu(x, y)){
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addRotacao(1);
         }
       }

       if(btRotDireita->Colidiu(x, y)){
         if(figura_selecionada != -1){
            figura[figura_selecionada]->addRotacao(-1);
         }
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
          //figura_selecionada = -1;
       }

      if(btFill->Colidiu(x, y)){
         fillSelect = !fillSelect;
       }
       /*
       if(btGreen->Colidiu(x, y)){
          //printf("\nClicou no botao verde\n");
          exibe_green = 1;
          exibe_red = exibe_blue = exibe_mono = 0;
       }

       if(btBlue->Colidiu(x, y)){
         //printf("\nClicou no botao azul\n");
         exibe_blue = 1;
         exibe_green = exibe_red = exibe_mono = 0;
       }
       if( btZoomIn->Colidiu(x, y)){
         //printf("\nClicou no botao zoom in\n");
         img1->setZoom(img1->getZoom()+1);
       }
       if( btZoomOut->Colidiu(x, y)){
         //printf("\nClicou no botao zoom out\n");
         img1->setZoom(img1->getZoom()-1);
       }*/
   }
   if(state == 1){
      // SOLTOU
      modoHold = false;
   }
}

void inicializaBotoes(){
   int espaco_entre = 10, larg_btn = 30, larg_btn_cor = 20;
   int offset_cor = larg_btn_cor + espaco_entre;

   // MANAGER
   manager_btn = new Manager();

   // BOTOES ACAO
   btAddRetangulo = new Botao(inicio_botoes_x, inicio_botoes_y, larg_btn, larg_btn, "", cor_zooms);
   btAddCirculo = new Botao(inicio_botoes_x+larg_btn+espaco_entre, inicio_botoes_y, larg_btn, larg_btn, "", cor_zooms);

   btFill = new Botao(inicio_botoes_x, inicio_botoes_y-larg_btn-espaco_entre, larg_btn, larg_btn, "", cor_zooms);

   btRotDireita = new Botao(inicio_botoes_x, inicio_botoes_y-5*larg_btn-espaco_entre, larg_btn, larg_btn, "", cor_zooms);
   btRotEsquerda = new Botao(inicio_botoes_x+larg_btn+espaco_entre, inicio_botoes_y-5*larg_btn-espaco_entre, larg_btn, larg_btn, "", cor_zooms);

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

   manager_btn->addBtn(btFill);
   manager_btn->addBtn(btRotDireita);
   manager_btn->addBtn(btRotEsquerda);

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



// MODO DE USO
void modo_uso(){
   //printf("\n\n1 - Utilize as setas para rotacionar a imagem.");
   //printf("\n2 - Utilize os numeros para alterar a imagem.");
}

void criaCirculo(float x, float y, float raio, Cor* corz, bool fill_){
   float cord[] = {x, y, raio};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(2, cord, 3, corz, fill_);
   QTD_FIGURAS++;
}

void criaRetangulo(float x1, float y1, float x2, float y2, Cor* corz, bool fill_){
   float cord[] = {x1, y1, x1, y2, x2, y2, x2, y1};
   figura = (Figura**)realloc(figura, (QTD_FIGURAS+1)*sizeof(Figura*));
   figura[QTD_FIGURAS] = new Figura(1, cord, 8, corz, fill_);
   QTD_FIGURAS++;
}

void inicializaFiguras(){
   figura = (Figura**)malloc(sizeof(Figura*)); //CRIANDO VETOR
}

int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // INICIALIZANDO VETOR DE FIGURAS
   inicializaFiguras();

   // IMAGENS
   fillImg = new Bmp(".\\ThalissonForte_CG_TB\\resources\\fill.bmp");
   fillImg->convertBGRtoRGB();

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Manipulacao de Figuras");

   runCanvas();
}


