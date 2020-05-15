/***************************************************************************************************
//  Trabalho 3 da disciplina de Computação Gráfica
//  Author: Thalisson Forte
//
//      Foram implementados todos os requisitos (básicos e alguns extras)
//  para criação e modificação de curvas utilizando a API Canvas 2D
//  implementada pelo professor da disciplina Cesar Tadeu Pozzer.
//
//  Instruções:
//
//  Na parte inferior encontram-se:
//          - Botão de adicionar novo ponto
//          - Botão de remover ponto
//          - Botão de iniciar ou parar animação
//
//  Na lateral direita encontra-se:
//          - Gráfico das blending functions
//
//    Atalhos importantes:
//          DELETE - Remove último ponto
//          ENTER  - Inicia ou para a animação
//          ESC    - Fecha o programa
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

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 470; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
float tempo = 0; // TEMPO
float inc_tempo = 0.001; // INCREMENTO DE TEMPO
float fps = 100; // FRAME INICIAL

//------- MANAGERS ----------//
#include "Frames.h"
#include "Ponto.h"
#include "ManagerPontos.h"
ManagerPontos *manager_pontos = NULL;
Manager *manager_btn = NULL;
Frames *frames;
/* ------------------------ */

// FUNCOES NOVAS
void grafico_blend();
void gerenciaMoverPonto();
void atualizarFrames();

// DESENHO DA TELA
void desenhaTela(){
   // FUNDO
   clear(0.85, 0.85, 0.85);

   // PARTE PRINCIPAL (MEIO)
      // PARTE DESENHAVEL
   color(0.92, 0.92, 0.92);
   rectFill(30,60, screenWidth-230, screenHeight-30);

}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
void render(){

   // DESENHO DA TELA
   desenhaTela();

   // SETANDO O FPS ATUAL
   manager_pontos->setFPS(fps);

   // DESENHO DOS PONTOS
   manager_pontos->Render();

   // MOVIMENTO DE ALGUM PONTO
   gerenciaMoverPonto();

   // RENDER DOS BOTOES
   manager_btn->renderBtns();

   // GRAFICO
   grafico_blend();

   // ATUALIZACAO DO TEMPO E FRAME
   atualizarFrames();

}

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO TECLADO
void keyboard(int key){

   if(key < 200){
      opcao = key;
   }

   switch(key){
      case 27: // ESC - SAIR
         exit(0);
      break;

      case 13: // ENTER - INICIA ANIMACAO
         modo_animacao = !modo_animacao;
      break;

      case 127: // DELETE - EXCLUSAO DO ULTIMO PONTO
         manager_pontos->removePonto();
      break;

   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   //printf("\nLiberou: %d" , key);
}

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO MOUSE
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x;
   mouseY = y;
   //printf("\nMouse: \tX = %i\tY = %i", mouseX, mouseY);
   if (state == 0){ //clicou

      if(manager_pontos->Colidiu(x, y) != -1){
         ponto_selecionado = manager_pontos->Colidiu(x, y);
         //printf("\nID: %i", ponto_selecionado);
      }

      modoHold = true;
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;

      if(btAddPonto->Colidiu(x, y)){
         printf("\tCriando Ponto\n");
         manager_pontos->addNovoPonto();
      }

      if(btRemovePonto->Colidiu(x, y)){
         printf("\tRemovendo Ponto\n");
         manager_pontos->removePonto();
      }

      if(btAnimacao->Colidiu(x, y)){
         modo_animacao = !modo_animacao;
      }
   }

   if(state == 1){
      // SOLTOU
      modoHold = false;
      ponto_selecionado = -1;
   }
}

// FUNCAO QUE INICIALIZA OS BOTOES UTILIZADOS
void inicializaBotoes(){
   int alt_btn = 30, larg_btn = 100;

   // MANAGER
   manager_btn = new Manager();

   // BOTOES CRIACAO
   btAddPonto = new Botao(30, 10, larg_btn, alt_btn, "Adicionar", cinzaForte);
   btRemovePonto = new Botao(150, 10, larg_btn, alt_btn, "Remover", cinzaForte);
   btAnimacao = new Botao(270, 10, larg_btn*2.5, alt_btn, "Iniciar/Parar animacao", cinzaForte);

   // ADICIONANDO NO MANAGER
   manager_btn->addBtn(btAddPonto);
   manager_btn->addBtn(btRemovePonto);
   manager_btn->addBtn(btAnimacao);
}

// MODO DE USO
void modo_uso(){
   system("CLS");
   printf("\n******************* INFORMACOES *******************\n");
   printf("\nUtilize os botoes disponiveis para adicionar ou remover pontos\n\te para iniciar e parar a animacao.\n");
   printf("\n(para mais informacoes sobre o programa consulte o header do arquivo 'main.cpp')\n\n");

   printf("\nAtalhos importantes: \n");
   printf("\nDELETE - Exclui o ultimo ponto");
   printf("\nENTER  - Cria um novo ponto");
   printf("\nESC    - Finaliza o programa\n\n");
}

// INICIALIZACAO DO MANAGER E DE ALGUNS PONTOS INICIAIS
void inicializaPontos(){
   manager_pontos = new ManagerPontos();
   ponto_selecionado = -1;

   // PONTOS INICIAIS
   manager_pontos->addPonto(80,80);
   manager_pontos->addPonto(80,400);
   manager_pontos->addPonto(500,400);
   manager_pontos->addPonto(500,80);
}

// GRAFICO DAS BLENDING FUNCTIONS
void grafico_blend(){

   char str[100];
   color(0,0,0);
   int x_grafico = screenWidth-180, y_grafico = screenHeight-370;

   sprintf(str, "Blending Functions:");
   text(x_grafico-15, y_grafico+120, str);

   line(x_grafico, y_grafico, x_grafico+150, y_grafico);
   line(x_grafico, y_grafico, x_grafico, y_grafico+100);
   manager_pontos->blendFunct(x_grafico, y_grafico);
}

// FUNCAO QUE FAZ A MOVIMENTACAO DO PONTO PELO USER
void gerenciaMoverPonto(){
   if(modoHold && ponto_selecionado != -1){
      manager_pontos->moverPonto(ponto_selecionado, mouseX - inicioHoldX, mouseY - inicioHoldY);
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;
   }
}

// FUNCAO PARA GERENCIAR TEMPO E FRAMES E EXIBIR
void atualizarFrames(){
   // MODIFICACAO DO TEMPO
   if(modo_animacao){
      tempo += inc_tempo;
      if(tempo > 1) tempo = 0;
   }else{
      tempo = 1;
   }

   // NOVO GET DO FPS
   fps = frames->getFrames();
   //printf("\nFPS: %f", fps);

   char str[100];
   color(0, 0, 0);
   sprintf(str, "%.0f FPS", fps);
   text(screenWidth - 90, screenHeight - 460, str);
}

// INICIANDO AS CORES UTILIZADAS PELAS LINHAS
void inicializaCores(){
   cores = (Cor**)malloc(4*sizeof(Cor*));
   cores[3] = verde;
   cores[2] = azul;
   cores[1] = rosa;
   cores[0] = amarelo;
}

int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // MODO DE USO
   modo_uso();

   // INICIALIZANDO PONTOS E MANAGER
   inicializaPontos();

   // INICILIZA CORES
   inicializaCores();

   // CONTROLE FPS
   frames = new Frames();

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Manipulacao de Curvas de Bezier");

   runCanvas();
}


