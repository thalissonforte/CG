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

float tempo = 0;
float inc_tempo = 0.00025;
bool modo_animacao = false;
Cor **cores = NULL;

// variavel global para selecao do que sera exibido na canvas
int opcao  = 50;
int screenWidth = 780, screenHeight = 530; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

#include "Ponto.h"
#include "ManagerPontos.h"

/* Manager */
Manager *manager_btn = NULL;
ManagerPontos *manager_pontos = NULL;
int ponto_selecionado;
/**/


/* Figuras e cores*/
int QTD_FIGURAS = 0;
Figura **figura = NULL;
Cor *corSelecionada = preto;
FILE *arquivo;



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
/*
void exibeCorSelecionada(){
    char str[100];
    int offset_x_ = 400;
    sprintf(str, "Cor selecionada");
    text(screenWidth-offset_x_,45, str);
}
*/

// DESENHO DA TELA
void desenhaTela(){

   // FUNDO
   clear(0.85, 0.85, 0.85);

   // PARTE PRINCIPAL (MEIO)
      // PARTE DESENHAVEL
   color(0.92, 0.92, 0.92);
   rectFill(30,60, screenWidth-30, screenHeight-30);

   //color(0.54,0.54,0.54);
   //rect(80,60, screenWidth-30, screenHeight-30);

   // COR SELECIONADA
   //corSelecionada->Aplica();
   //rectFill(screenWidth-380, 5, screenWidth-270, 35);
}

// DESENHO DOS BOTOES DE CRIACAO
/*
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
*/

// FUNCAO QUE DESENHA SOBRE O OBJETO SELECIONADO
/*
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
*/

// FUNCAO QUE PRINTA A IMAGEM
/*
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
*/

// FUNCAO DE CONTROLE DE EXIBICAO DE IMAGENS
/*
void exibirImagens(){
   exibeImg(inicio_botoes_cor_x+18, inicio_botoes_cor_y + 67, fillImg);
   exibeImg(inicio_botoes_cor_x + 6*30 + 7, inicio_botoes_cor_y + 7, rotDirImg);
   exibeImg(inicio_botoes_cor_x + 7*30 + 17, inicio_botoes_cor_y + 7, rotEsqImg);
   exibeImg(inicio_botoes_cor_x + 9*30 + 7, inicio_botoes_cor_y + 7, trazerFrente);
   exibeImg(inicio_botoes_cor_x + 10*30 + 17, inicio_botoes_cor_y + 7, levarTras);
}
*/

// FUNCAO QUE REMOVE A FIGURA
/*
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
*/

// FUNCAO QUE MODIFICA A FIGURA (POSICAO E TAMANHO)
/*
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
*/

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
void render(){

   // DESENHO DA TELA
   desenhaTela();

   // DESENHO DOS PONTOS
   manager_pontos->Render();

   // TEMPO
   if(modo_animacao){
      tempo += inc_tempo;
      if(tempo > 1) tempo = 0;
   }else{
      tempo = 1;
   }

   if(modoHold && ponto_selecionado != -1){
      //printf("Mover %i em x: %i\t em y: %i", ponto_selecionado, mouseX - inicioHoldX, mouseY - inicioHoldY);
      manager_pontos->moverPonto(ponto_selecionado, mouseX - inicioHoldX, mouseY - inicioHoldY);

      inicioHoldX = mouseX;
      inicioHoldY = mouseY;
   }

   // RENDER DOS BOTOES
   manager_btn->renderBtns();

   // BOTÕES
   //desenhaBotoes();

   // IMAGENS
   //exibirImagens();

   // FIGURAS
   /*for(int i=0; i<QTD_FIGURAS; i++){
      figura[i]->desenhaFigura();
   }*/

   // MODO FILL
   //if(fillSelect) exibeImg(mouseX-8, mouseY-5, fillImg);

   // DESENHO SELECAO
   //desenhaSelecao();

   // ALTERACAO NA FIGURA
   //movimentacaoHold();

}

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO TECLADO
void keyboard(int key){

   if(key < 200){
      opcao = key;
   }

   switch(key){
      case 27: // ESC
         //salvarArquivo();
         exit(0);
      break;

      case 13: // ENTER
         //manager_pontos->iniciaPontosAnimados();
         modo_animacao = !modo_animacao;
      break;

      case 127: // DELETE
         //if(figura_selecionada != -1) removeFigura(figura_selecionada);
      break;

      case 102: // LETRA F
         //fillSelect = !fillSelect;
      break;

      case 200: // ESQUERDA
         /*if(figura_selecionada != -1){
            figura[figura_selecionada]->addX(-10);
         }*/
      break;

      case 201: // CIMA
         /*if(figura_selecionada != -1){
            figura[figura_selecionada]->addY(10);
         }*/
      break;

      case 202:  // DIREITA
         /*if(figura_selecionada != -1){
            figura[figura_selecionada]->addX(10);
         }*/
      break;

      case 203:  // BAIXO
         /*if(figura_selecionada != -1){
            figura[figura_selecionada]->addY(-10);
         }*/
      break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   //printf("\nLiberou: %d" , key);
}

/*
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
*/

// FUNCAO LIMPAR
/*
void limparFiguras(){
    int quantia = QTD_FIGURAS;
    for(int i=0; i<quantia; i++){
        removeFigura(0);
    }
}
*/

// FUNCAO RESPONSAVEL POR TRATAR A UTILIZACAO DO MOUSE
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x;
   mouseY = y;
   printf("\nMouse: \tX = %i\tY = %i", mouseX, mouseY);


   if (state == 0){ //clicou


      if(manager_pontos->Colidiu(x, y) != -1){
         ponto_selecionado = manager_pontos->Colidiu(x, y);
         printf("\nID: %i", ponto_selecionado);
      }

      modoHold = true;
      inicioHoldX = mouseX;
      inicioHoldY = mouseY;

      if(btAddPonto->Colidiu(x, y)){
         printf("\nCriando Ponto\n");
         manager_pontos->addNovoPonto();
         //criaRetangulo(200, 300, 400, 400, corSelecionada, false, 0);
      }

      if(btRemovePonto->Colidiu(x, y)){
         printf("\tRemovendo Ponto\n");
         manager_pontos->removePonto();
         //criaCirculo(200, 200, 50, corSelecionada, false, 0);
      }

   }
   if(state == 1){
      // SOLTOU
      modoHold = false;
      ponto_selecionado = -1;
      //redimensionar = -1;
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

   // ADICIONANDO NO MANAGER
   manager_btn->addBtn(btAddPonto);
   manager_btn->addBtn(btRemovePonto);
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

void inicializaPontos(){
   manager_pontos = new ManagerPontos();
   ponto_selecionado = -1;
}

int main(void){

   // INICIALIZANDO BOTOES E MANAGER
   inicializaBotoes();

   // MODO DE USO
   //modo_uso();

   // INICIALIZANDO PONTOS E MANAGER
   inicializaPontos();

    manager_pontos->addPonto(80,80);
    manager_pontos->addPonto(80,500);
    manager_pontos->addPonto(500,500);
    manager_pontos->addPonto(500,80);
    manager_pontos->addPonto(120,80);
    manager_pontos->addPonto(120,450);
    manager_pontos->addPonto(450,450);
    manager_pontos->addPonto(450,300);

    cores = (Cor**)malloc(4*sizeof(Cor*));
    cores[3] = verde;
    cores[2] = azul;
    cores[1] = rosa;
    cores[0] = amarelo;

   // INICIANDO CANVAS
   initCanvas(&screenWidth, &screenHeight, "Manipulacao de Curvas de Bezier");

   runCanvas();
}


