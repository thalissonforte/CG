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
#include "Frames.h"
#include "Objeto.h"


//#include "Manivela.h"


#include "CalculosMatriz.h"
#include "Cilindro.h"
#include "Pistao.h"
#include "Biela.h"
#include "Motor.h"


//#define RPM 120

/* Manager */
Manager *manager_btn = NULL;

/* Botões */
//Botao   *btRed = NULL;

/* Frame e velocidade*/
int frame = 0;
int RPM = 1;
int fps = 60;
//float velocidade = 1;
Frames* frames;

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 530; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

int origemX = screenWidth/2;
int origemY = screenHeight/2 - 100;

float rotacoes = 0;

//Manivela *manivela;
Motor *motor;
Motor *motor_;


bool modoHold = false;
int xHold, yHold;

void DrawMouseScreenCoords(){
    char str[100];
    int offset_x_ = 80;
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    text(offset_x_,10, str);
    //sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    //text(offset_x_+190,10, str);
    //sprintf(str, "Utilize as setas para rotacionar a imagem");
    //text(50,30,str);
    //sprintf(str, "Utilize os numeros para alterar a imagem");
    //text(50,50,str);
}




float **gerar_retangulo(int x1, int y1, int x2, int y2, int larg){

   float coeficiente_angular;
   float angulo;
   float div = larg/2;

   // PRIMEIRO PONTO SEMPRE ORIGEM
      // DIVISAO POR ZERO (ANGULO == 0)
   if(x2 == x1){
      angulo = PI/2;
   }else{
      // ENCONTRO O COEFICIENTE ANGULAR ENTRE OS PONTOS
      coeficiente_angular = (float)(y2-y1)/(float)(x2-x1);
      // OBTENHO O VALOR DO ANGULO ATRAVES DO ARCTG(COEFICIENTE ANGULAR)
      angulo = atan(coeficiente_angular);
   }

   // SE ANGULO MENOR QUE ZERO, AJUSTO A POSICAO POIS ESTA DO OUTRO LADO
   if(angulo < 0) angulo = -angulo + PI/2;

   // ANALISE DO QUADRANTE
   int quadrante = 0; // SOBRE ALGUM EIXO
   if(y2 > y1){
      if(x2 > x1) quadrante = 1;       // + +
      else if(x2 < x1) quadrante = 2;  // - +
   }else if(y2 < y1){
      if(x2 > x1) quadrante = 4;       // + -
      else if(x2 < x1) quadrante = 3;  // - -
   }

   //
   float ajuste_angulo_y = div*sin(1.5708 + angulo);
   float ajuste_angulo_x = div*cos(1.5708 + angulo);
   if(quadrante == 1){
      //ajuste_angulo_y = div*sin(1.5708 + angulo);
      //ajuste_angulo_x = div*cos(1.5708 + angulo);
   }else if(quadrante == 2){
      ajuste_angulo_y = ajuste_angulo_x;
      ajuste_angulo_x = div*sin(1.5708 + angulo);
   }else if(quadrante == 3){
      //ajuste_angulo_y = ajuste_angulo_y;
      //ajuste_angulo_x = ajuste_angulo_x;
   }else if(quadrante == 4){
      ajuste_angulo_y = ajuste_angulo_x;
      ajuste_angulo_x = div*sin(1.5708 + angulo);;
   }else if(quadrante == 0){
      //ajuste_angulo_y = div;
      //ajuste_angulo_x = div;
   }

   float xs[] = {x1 - ajuste_angulo_x, x1 + ajuste_angulo_x, x2 + ajuste_angulo_x, x2 - ajuste_angulo_x};
   float ys[] = {y1 - ajuste_angulo_y, y1 + ajuste_angulo_y, y2 + ajuste_angulo_y, y2 - ajuste_angulo_y};


   //polygon(xs, ys, 4);

   float *(vetores[]) = {xs, ys};
   return vetores;
 }

void demodocubo(){

   int ponto_x_central_cubo = screenWidth/2;
   int ponto_y_central_cubo = screenWidth/2;

   int d = 1;



}
void modo_arrastar(){
   if(modoHold){

      // GUARDO A QUANTIDADE
      int qtd_x = mouseX - xHold;
      int qtd_y = mouseY - yHold;

      // RETORNO PRO ZERO
      xHold += qtd_x;
      yHold += qtd_y;

      //motor->rotacionar_y(0.01);
      //motor->rotacionar_x(qtd_x/10);

      /*xHold += qtd_x;
      yHold += qtd_y;

      origemX = xHold;
      origemY = yHold;

      if(origemX < 1) origemX = 1;
      if(origemY < 1) origemY = 1;
      */
   }
}
//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render(){

   DrawMouseScreenCoords();

   fps = frames->getFrames();
   if(fps < 1) fps = 1;

   // TRANSFORMA O FPS EM ROTACOES
   float rotacoes_segundo = (float) RPM / 60;
   float rotacoes_frame = rotacoes_segundo / (float)fps;
   float incremento = rotacoes_frame;

   // INVERTER SENTIDO DA ROTACAO -= ou +=
   rotacoes -= incremento;
   motor->setAngulo(rotacoes*PI_2);
   motor->desenhar();

   //motor_->setAngulo(rotacoes*PI_2);
   //motor_->desenhar();

   clear(0,0,0);

   modo_arrastar();
   //manivela->setAngulo(rotacoes*PI_2);
   //manivela->Draw2D(200);
   //manivela->Draw2D(60);
   //rotacoes = -rotacoes;


/*
   // RAIO (TAMANHO ENTRE ORIGEM MANIVELA-ORIGEM BIELA)
   int r = 80;



   // DISTANCIA ENTRE ORIGEM-BIELA E PISTAO
   int dist_bp = 200;
   // FUNDO
   clear(0,0,0);

   modo_arrastar();

   // CIRCULO ORIGEM
   circle(origemX, origemY, 5, 100);

   // SEGMENTO ORIGEM-BIELA
   int centrox_biela = origemX + r*cos(rotacoes * PI_2);
   int centroy_biela = origemY + r*sin(rotacoes * PI_2);
   line(origemX, origemY, centrox_biela, centroy_biela);

   // DESENHO CIRCULO BIELA
   circle(centrox_biela, centroy_biela, 5, 100);

   // POSICAO DO PISTAO
   int x_pistao = origemX;
   int y_pistao = r * sin(rotacoes * PI_2) + sqrt(pow(dist_bp+origemY, 2) - pow(r, 2)*pow(cos(rotacoes * PI_2), 2));

   // SEGMENTO QUE LIGA BIELA AO PISTAO (BIELA)
   line(centrox_biela, centroy_biela, x_pistao, y_pistao);

   // RETANGULO BIELA-PISTAO
   int larg_rod = 20;
   float angulo_rod_y = cos(rotacoes * PI_2);
   float angulo_rod_x = sin(rotacoes * PI_2);


   gerar_retangulo(centrox_biela, centroy_biela, x_pistao, y_pistao, larg_rod);

   gerar_retangulo(origemX, origemY, centrox_biela, centroy_biela, larg_rod-10);
   rect(x_pistao-20, y_pistao-40, x_pistao+20, y_pistao+40);

   */

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
        RPM++;
        printf("\n%i RPM", RPM);
      break;
      case 50: // 2
        RPM--;
        if(RPM < 0) RPM = 0;
        printf("\n\t%i RPM", RPM);
      break;
      case 51: // 3

      break;
      case 52: // 4

      break;

      case 200: // SETA P ESQUERDA
         //manivela->MoveX(-10);
         motor->rotacionar_y(-0.1);
      break;

      case 201: // SETA P CIMA
         //manivela->MoveY(10);
         motor->rotacionar_x(0.1);
      break;

      case 202: // SETA P DIREITA
         //manivela->MoveX(10);
         motor->rotacionar_y(0.1);
      break;

      case 203: // SETA P BAIXO
         //manivela->MoveY(-10);
         motor->rotacionar_x(-0.1);
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

   if(state == 0){//clicou
      if(button == 0){ // ESQUERDO

         modoHold = true;
         xHold = mouseX;
         yHold = mouseY;

         /*if(mouseX > origemX-5 && mouseX < origemX+5 && mouseY > origemY-5 && mouseY < origemY+5){
            modoHold = true;
            xHold = mouseX;
            yHold = mouseY;
         }
         */
      }
   }

   if(state == 1){ // SOLTOU
      if(modoHold){
         modoHold = false;
      }
   }
}

void inicializaBotoes(){

   int offset_x_btn1 = 80, offset_y_btn1 = 80, larg_btn = 100;

   manager_btn = new Manager();

   //btZoomIn = new Botao(offset_x_btn1-40, offset_y_btn1 + 2.65*larg_btn, 50, 50, "+", cor_zooms);


   //manager_btn->addBtn(btZoomIn);

}

// MODO DE USO
void modo_uso(){
   //printf("\n\n1 - Utilize as setas para rotacionar a imagem.");
   //printf("\n2 - Utilize os numeros para alterar a imagem.");
}


int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   //inicializaBotoes();

   // CARREGAMENTO IMAGEM
   //carregamento_imagem(1);

   // UTILIZAR O TAMANHO DA IMAGEM PARA ADAPTAR A CANVAS
   //int image_size_h = img1->getHeight() - 100;
   //int image_size_w = img1->getWidth() - 100;
   //if(image_size_h > 0) screenHeight += image_size_h;

   // INICILIZANDO FRAMES
   frames = new Frames();

   // INICIALIZANDO MANIVELA
   //manivela = new Manivela(100, 100, 100, (float) 100,(float) 50);

   motor = new Motor(200, 200, 500, 100, 50);
   //motor_ = new Motor(0, 0, 0, 0, 0);

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Motor a combustao interna");
   printf("\n%i RPM", RPM);
   runCanvas();
}


