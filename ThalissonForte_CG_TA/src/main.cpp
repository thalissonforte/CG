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
#include "Bmp.h"
#include "Manager.h"

/* Manager */
Manager *manager_btn = NULL;

/* Botões */
Botao   *btRed = NULL;
Botao   *btGreen = NULL;
Botao   *btBlue = NULL;
Botao   *btNormal = NULL;
Botao   *btMono = NULL;
Botao   *btZoomIn = NULL;
Botao   *btZoomOut = NULL;

/* Cores atuais */
int exibe_red = 1;
int exibe_green = 1;
int exibe_blue = 1;
int exibe_mono = 0;
long int maximo = 0;

/* Imagem */
Bmp *img1;

void color_Transform(int i, int j, int zoom);
void exibeImg(int offset_X, int offset_Y, int rotacao, int zoom);
void imageTransform(int i, int j, int offset_X, int offset_Y, int rotacao, int zoom);
void desenhaHistograma();
long int max_cor();
void carregamento_imagem(int opt);

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 530; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

void DrawMouseScreenCoords(){
    char str[100];
    int offset_x_ = 80;
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    text(offset_x_,10, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    text(offset_x_+190,10, str);
    sprintf(str, "Utilize as setas para rotacionar a imagem");
    text(50,30,str);
    sprintf(str, "Utilize os numeros para alterar a imagem");
    text(50,50,str);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render(){
   DrawMouseScreenCoords();

   // FUNDO
   clear(0.8,1,1);

   manager_btn->renderBtns();

   exibeImg(122, 230, img1->getRotacao(), img1->getZoom());
   desenhaHistograma();

   preto->Aplica();
}

void exibeImg(int offset_X, int offset_Y, int rotacao, int zoom){

   int alt, lar, inc;

   alt = img1->getHeight() * pow(2, zoom);
   lar = img1->getWidth() * pow(2, zoom);
   inc = pow(2, abs(zoom));

   for(int i=0; i < alt; i+=inc){
      for(int j = 0; j < lar; j+=inc){
         color_Transform(i, j, zoom);
         imageTransform(i, j, offset_X, offset_Y, rotacao, zoom);
      }
   }
}

void imageTransform(int i, int j, int offset_X, int offset_Y, int rotacao, int zoom){
   int x, y;

   // TRATAR ROTACAO PRIMEIRO - ROTACAO: 0 = BAIXO, 1 = DIREITA, 2 = CIMA, 3 = ESQUERDA
   if(rotacao == 0){ // CIMA
      x = j + offset_X;
      y= i + offset_Y;
   }else if(rotacao == 1){ // DIREITA
      x = offset_X - i + pow(2,zoom)*img1->getHeight();
      y = j + offset_Y;
   }else if(rotacao == 2){ // BAIXO
      x = j + offset_X;
      y = offset_Y - i + pow(2,zoom)*img1->getHeight();
   }else if(rotacao == 3){ // ESQUERDA
      x = i + offset_X;
      y = j + offset_Y;
   }

   // TRATAR ZOOM
   if(zoom!=0){
      if(zoom<0) zoom = -zoom;
      for(int m=0; m<=pow(2,zoom); m++){
         for(int n=0; n<=pow(2,zoom); n++){
            point(x+m, y+n);
         }
      }
   }else{
      point(x, y);
   }

}

void color_Transform(int i, int j, int zoom){

   if(zoom > 0){
      i /= pow(2, zoom);
      j /= pow(2, zoom);
   }

   float _r = img1->pixel[i][j]->getR();
   float _g = img1->pixel[i][j]->getG();
   float _b = img1->pixel[i][j]->getB();

   if(zoom < 0){
      int _zoom = -zoom;
      float z_r = 0;
      float z_g = 0;
      float z_b = 0;

      i *= pow(2, _zoom);
      j *= pow(2, _zoom);

      for(int k = i; k < i + pow(2, _zoom); k++){
         for(int l = j; l < j + pow(2, _zoom); l++){

            z_r += img1->pixel[k][l]->getR();
            z_g += img1->pixel[k][l]->getG();
            z_b += img1->pixel[k][l]->getB();

         }
      }

      _r = z_r / pow(4, _zoom);
      _g = z_g / pow(4, _zoom);
      _b = z_b / pow(4, _zoom);

   }

   if(!exibe_red) _r = 0;
   if(!exibe_green) _g = 0;
   if(!exibe_blue) _b = 0;

   if(exibe_mono){
      _r *= 0.299;
      _g *= 0.587;
      _b *= 0.114;
      float sum = _r + _g + _b;
      _r = _g = _b = sum;
   }

   color(_r, _g, _b);
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
         carregamento_imagem(1);
      break;
      case 50: // 2
         carregamento_imagem(2);
      break;
      case 51: // 3
         carregamento_imagem(3);
      break;
      case 52: // 4
         carregamento_imagem(4);
      break;

      case 200: // SETA P ESQUERDA
         img1->setRotacao(3);
      break;

      case 201: // SETA P CIMA
         img1->setRotacao(2);
      break;

      case 202: // SETA P DIREITA
         img1->setRotacao(1);
      break;

      case 203: // SETA P BAIXO
         img1->setRotacao(0);
      break;


   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   //printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
       if( btNormal->Colidiu(x, y)){
         //printf("\nClicou no botao normal\n");
         exibe_green = exibe_blue = exibe_red = 1;
         exibe_mono = 0;
       }
       if( btMono->Colidiu(x, y)){
         //printf("\nClicou no botao monocromatico\n");
         exibe_green = exibe_blue = exibe_red = 1;
         exibe_mono = 1;
       }
       if( btRed->Colidiu(x, y)){
         //printf("\nClicou no botao vermelho\n");
         exibe_red = 1;
         exibe_green = exibe_blue = exibe_mono = 0;
       }
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
       }
   }
}

void inicializaBotoes(){

   int offset_x_btn1 = 80, offset_y_btn1 = 80, larg_btn = 100;

   manager_btn = new Manager();

   btZoomIn = new Botao(offset_x_btn1-40, offset_y_btn1 + 2.65*larg_btn, 50, 50, "+", cor_zooms);
   btZoomOut = new Botao(offset_x_btn1-40, offset_y_btn1 + 2*larg_btn, 50, 50, "-", cor_zooms);
   btNormal = new Botao(offset_x_btn1 + 0.75*larg_btn, offset_y_btn1 + 0.65*larg_btn, larg_btn, 50, "Normal", cinza);
   btMono = new Botao(offset_x_btn1 + 2.25*larg_btn, offset_y_btn1 + 0.65*larg_btn, larg_btn, 50, "Mono", preto);
   btRed = new Botao(offset_x_btn1, offset_y_btn1, larg_btn, 50, "Vermelho", verm_fraco);
   btGreen = new Botao(offset_x_btn1 + 1.5*larg_btn, offset_y_btn1, larg_btn, 50, "Verde", verde_fraco);
   btBlue = new Botao(offset_x_btn1 + 3*larg_btn, offset_y_btn1, larg_btn, 50, "Azul", azul_fraco);

   manager_btn->addBtn(btZoomIn);
   manager_btn->addBtn(btZoomOut);
   manager_btn->addBtn(btNormal);
   manager_btn->addBtn(btMono);
   manager_btn->addBtn(btRed);
   manager_btn->addBtn(btGreen);
   manager_btn->addBtn(btBlue);

}

void desenhaHistograma(){
   int offset_x_histo = screenWidth + img1->getWidth() - 550, offset_y_histo = 5;
   int height = 220, width = 350;

   if(offset_x_histo < 500) offset_x_histo = 500;

   for(int x = 0; x < 256; x++){
      for(int y = 0; y < height; y++){
         color(1,1,1);
         float pixels = (float) height / maximo;
         //pixels *= 2;
         if(exibe_red) if(y < (img1->rgb[0][x] * pixels)) color(255,0,0);
         if(exibe_green) if(y < (img1->rgb[1][x] * pixels)) color(0,255,0);
         if(exibe_blue) if(y < (img1->rgb[2][x] * pixels)) color(0,0,255);
         point(offset_x_histo + x, offset_y_histo + y);
      }
   }

}

// RETORNA MAIOR VALOR R,G,B
long int max_cor(){
   long int r_max = -1, g_max = -1, b_max = -1;
   for(int i=0; i<256; i++){
      if(img1->rgb[0][i] > r_max) r_max = img1->rgb[0][i];
   }
   for(int i=0; i<256; i++){
      if(img1->rgb[1][i] > g_max) g_max = img1->rgb[1][i];
   }
   for(int i=0; i<256; i++){
      if(img1->rgb[2][i] > b_max) b_max = img1->rgb[2][i];
   }

   if(r_max >= g_max && r_max >= b_max) return r_max;
   if(g_max >= r_max && g_max >= b_max) return g_max;
   if(b_max >= g_max && b_max >= r_max) return b_max;
}

// MODO DE USO
void modo_uso(){
   printf("\n\n1 - Utilize as setas para rotacionar a imagem.");
   printf("\n2 - Utilize os numeros para alterar a imagem.");
}

// CARREGAMENTO IMAGEM
void carregamento_imagem(int opt){
   system("CLS");
   // CARREGAMENTO IMAGEM
   if(opt == 1) img1 = new Bmp(".\\ThalissonForte_CG_TA\\resources\\normal_1.bmp"); // 49
   if(opt == 2) img1 = new Bmp(".\\ThalissonForte_CG_TA\\resources\\marbles.bmp"); // 50
   if(opt == 3) img1 = new Bmp(".\\ThalissonForte_CG_TA\\resources\\ray_res.bmp"); // 51
   if(opt == 4) img1 = new Bmp(".\\ThalissonForte_CG_TA\\resources\\aviao.bmp"); // 52

   printf("\n\nImagem %i carregada.", opt);

   modo_uso();

   // CONFIGURACOES DA IMAGEM E AUXILIAR
   img1->convertBGRtoRGB();
   maximo = max_cor();

}

int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // CARREGAMENTO IMAGEM
   carregamento_imagem(1);

   // UTILIZAR O TAMANHO DA IMAGEM PARA ADAPTAR A CANVAS
   int image_size_h = img1->getHeight() - 100;
   int image_size_w = img1->getWidth() - 100;
   if(image_size_h > 0) screenHeight += image_size_h;

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Manipulacao de Imagens");

   runCanvas();
}


