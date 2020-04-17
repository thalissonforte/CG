#ifndef __CANVAS_2D__H__
#define __CANVAS_2D__H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#define PI_2 6.28318530717958

#define Y_CANVAS_CRESCE_PARA_CIMA 1

//funcoes para desenho de ponto e linha
void point(int x, int y); //coordeandas do ponto
void line( int x1, int y1, int x2, int y2 ); //coordenadas da linha x1, y1, x2, y2

//desenha um retangulo alinhado nos eixos x e y
void rect( int x1, int y1, int x2, int y2 ); //coordenadas do retangulo x1, y1, x2, y2
void rectFill( int x1, int y1, int x2, int y2 ); //coordenadas do retangulo x1, y1, x2, y2

//desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
void polygon(float vx[], float vy[], int n_elems);
void polygonFill(float vx[], float vy[], int n_elems);

//centro e raio do circulo
void circle( int x, int y, int raio, int div );
void circleFill( int x, int y, int raio, int div );

//especifica a cor de desenho e de limpeza de tela
void color(float r, float g, float b);
void clear(float r, float g, float b);

//desenha texto na coordenada (x,y)
void text(int x, int y, const char *t);

//funcoes de CALLBACK da biblioteca ()
void keyboard(int key);
void keyboardUp(int key);
void specialUp(int key);
void mouse(int bt, int st, int wheel, int direction, int x, int y);
void render();
void mouseWheelCB(int wheel, int direction, int x, int y);
void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y);


//funcao de inicializacao da Canvas2D. Recebe a largura e altura
void initCanvas(int *w, int *h, const char *title);

//funcao para executar a Canvas2D
void runCanvas();


#endif
