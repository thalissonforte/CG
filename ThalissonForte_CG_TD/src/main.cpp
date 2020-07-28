/*********************************************************************
//  Trabalho final da disciplina de Computação Gráfica
//  Author: Thalisson Forte
//
//      Foram implementados todos os requisitos para a simulação e
//  manipulação do motor de combustão interna utilizando a API
//  Canvas 2D implementado pelo professor da disciplina Cesar Tadeu Pozzer.
//
//  Instruções:
//
//      Os botões com nome de partes do motor habilitam e desabilitam
//      a sua visualização.
//
//      A quantia de rotacoes por minuto pode ser controlada pelos botões
//      ou pelas teclas W (para aumentar o valor) e S (para diminuir).
//
//      Para rotacionar o motor utilize as setas ou clique e arraste
//      utilizando o mouse.
//
//      O botão 'Alternar' alterna a visualização para o formato em duas
//      ou tres dimensões.
//
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
#include "Calculos.h"
#include "Cilindro.h"
#include "Pistao.h"
#include "Biela.h"
#include "Motor.h"

/* Manager */
Manager *manager_btn = NULL;

/* Botões */
Botao *btCamisa = NULL;
Botao *btPistao = NULL;
Botao *btBiela = NULL;
Botao *btVirabrequim = NULL;
Botao *btUpRPM = NULL;
Botao *btDownRPM = NULL;
Botao *btModo = NULL;

/* Frame e velocidade*/
int RPM = 30;
int fps = 60;
Frames* frames;

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 550; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
float rotacoes = 0;

// MOTOR
Motor *motor;

/* Variaveis auxiliares */
bool modoHold = false;
int xHold, yHold;

// EXIBIR RPM NA TELA
void DrawRPM(){
    char str[100];
    sprintf(str, "%i RPM", RPM);
    text(20, screenHeight - 20, str);
}

// FUNCAO DE CONTROLE DO ARRASTAR DO MOUSE
void modo_arrastar(){
   if(modoHold){

      // GUARDO A QUANTIDADE
      int qtd_x = mouseX - xHold;
      int qtd_y = mouseY - yHold;

      // RETORNO PRO ZERO
      xHold += qtd_x;
      yHold += qtd_y;

      motor->rotacionar_x(((float)qtd_y / (float)screenWidth) * PI_2);
      motor->rotacionar_y(((float)qtd_x / (float)screenHeight) * PI_2);
   }
}

// FUNCAO PARA CONTROLAR FPS E ANGULACAO
void controle_fps(){
   fps = frames->getFrames();
   if(fps < 1) fps = 1;

   // TRANSFORMA O FPS EM ROTACOES
   float rotacoes_segundo = (float) RPM / 60;
   float rotacoes_frame = rotacoes_segundo / (float)fps;
   float incremento = rotacoes_frame;

   // INVERTER SENTIDO DA ROTACAO -= ou +=
   rotacoes -= incremento;
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render(){

   // EXIBIR RPM
   DrawRPM();

   // CONTROLAR FPS E ROTACAO
   controle_fps();
   
   // AJUSTE DO MOTOR E EXIBICAO
   motor->setAngulo(rotacoes*PI_2);
   motor->desenhar();

   clear(0,0,0);

   // GERENCIA HOLD MOUSE
   modo_arrastar();

   // MANAGER DE BOTOES
   manager_btn->renderBtns();

}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key){
   if( key < 200 ){
      opcao = key;
   }

   switch(key){
      case 27: // ESC


         exit(0);
      break;

      case 119: // W
        RPM++;
        printf("\n%i RPM", RPM);
      break;
      case 115: // S
        RPM--;
        if(RPM < 0) RPM = 0;
        printf("\n\t%i RPM", RPM);
      break;

      case 200: // SETA P ESQUERDA
         motor->rotacionar_y(-0.1);
      break;

      case 201: // SETA P CIMA
         motor->rotacionar_x(0.1);
      break;

      case 202: // SETA P DIREITA
         motor->rotacionar_y(0.1);
      break;

      case 203: // SETA P BAIXO
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

         if(btCamisa->Colidiu(x, y)){
            motor->change_camisa_exibir();
         }
         if(btPistao->Colidiu(x, y)){
            motor->change_pistao_exibir();
         }
         if(btBiela->Colidiu(x, y)){
            motor->change_biela_exibir();
         }
         if(btVirabrequim->Colidiu(x, y)){
            motor->change_virabrequim_exibir();
         }

         if(btUpRPM->Colidiu(x, y)){
            RPM++;
            printf("\n%i RPM", RPM);
         }
         if(btDownRPM->Colidiu(x, y)){
            RPM--;
            if(RPM < 0) RPM = 0;
            printf("\n\t%i RPM", RPM);
         }

         if(btModo->Colidiu(x, y)){
            // ALTERNAR MODO 2D/3D
            motor->change_modo();
            if(motor->get_modo()) printf("\n\t\tModo 2D ativo");
            else printf("\n\t\tModo 3D ativo");
         }


      }
   }

   if(state == 1){ // SOLTOU
      if(modoHold){
         modoHold = false;
      }
   }
}

// FUNCAO RESPONSAVEL POR INICIALIZAR BOTOES E MANAGER
void inicializaBotoes(){

   int offset_x_btn1 = 10, offset_y_btn1 = 10, larg_btn = 120, div = 10;

   manager_btn = new Manager();
   btCamisa = new Botao(offset_x_btn1, offset_y_btn1, larg_btn, 30, "Camisa", roxo);
   btPistao = new Botao(offset_x_btn1 + div + larg_btn, offset_y_btn1, larg_btn, 30, "Pistao", roxo);
   btBiela = new Botao(offset_x_btn1 + 2*(larg_btn+div), offset_y_btn1, larg_btn, 30, "Biela", roxo);
   btVirabrequim = new Botao(offset_x_btn1 + div + 3*(larg_btn) + 2*div, offset_y_btn1, larg_btn, 30, "Virabrequim", roxo);
   btUpRPM = new Botao(screenWidth - 180, offset_y_btn1 + 7, 80, 20, "+ RPM", azul_fraco);
   btDownRPM = new Botao(screenWidth - 90, offset_y_btn1 + 7, 80, 20, "- RPM", azul_fraco);
   btModo = new Botao(screenWidth - 155, screenHeight - 35, 150, 30, "Alternar 2D/3D", cinza);

   manager_btn->addBtn(btCamisa);
   manager_btn->addBtn(btPistao);
   manager_btn->addBtn(btBiela);
   manager_btn->addBtn(btVirabrequim);
   manager_btn->addBtn(btUpRPM);
   manager_btn->addBtn(btDownRPM);
   manager_btn->addBtn(btModo);

}

// MODO DE USO
void modo_uso(){
   printf("\n\n1 - Utilize as setas ou clique e arraste para rotacionar o motor.");
   printf("\n2 - Utilize W para aumentar o RPM ou S para diminui-lo.");
   printf("\n\n\tPara mais informacoes leia o header do arquivo 'main.cpp'");
}


int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // INICILIZANDO FRAMES
   frames = new Frames();

   // INICIALIZANDO MOTOR
   motor = new Motor(screenWidth/2, 300, 500, 100, 50);

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Motor a combustao interna");
   
   // MODO DE USO
   modo_uso();

   // PRINTANDO RPM
   printf("\n\n\n\t%i RPM", RPM);

   // RODANDO
   runCanvas();
}


