/*
  FUNCAO QUE GERENCIA AS FIGURAS

    Por padrão as coordenadas das figuras são sempre para que seja possível
    a utilização da função polygon (menos figuras circulares), portanto até
    mesmo retângulos precisam conter as coordenadas de todas os pontos.

*/
#ifndef __FIGURA_H__
#define __FIGURA_H__

#define DIV_DEFAULT 200


#include "gl_canvas2d.h"
#include "math.h"

/*
    TIPO
    1 = RETANGULO
    2 = CIRCULO
    3 = TRIANGULO
    4 = HEXAGONO
*/

class Figura{
    int tipo;
    float *coord;

    float *Xs;
    float *Ys;
    int raio;

    int rotacao;

    Cor *cor;
    int qtd_coords;

    int qnt_coord_x;
    int qnt_coord_y;
    bool modo_fill;
    bool selected;

    void set_coordenadas(){
        if(tipo == 1){
            Xs = (float*)malloc(4*sizeof(float));
            Ys = (float*)malloc(4*sizeof(float));

            Xs[0] = coord[0];
            Xs[1] = coord[2];
            Xs[2] = coord[4];
            Xs[3] = coord[6];
            qnt_coord_x = 4;

            Ys[0] = coord[1];
            Ys[1] = coord[3];
            Ys[2] = coord[5];
            Ys[3] = coord[7];
            qnt_coord_y = 4;

        }
        if(tipo == 2){
            Xs = (float*)malloc(sizeof(float));
            Ys = (float*)malloc(sizeof(float));

            Xs[0] = coord[0];
            qnt_coord_x = 1;

            Ys[0] = coord[1];
            qnt_coord_y = 1;

            raio = (int) coord[2];
        }
        if(tipo == 3){
            Xs = (float*)malloc(3*sizeof(float));
            Ys = (float*)malloc(3*sizeof(float));

            Xs[0] = coord[0];
            Xs[1] = coord[2];
            Xs[2] = coord[4];
            qnt_coord_x = 3;

            Ys[0] = coord[1];
            Ys[1] = coord[3];
            Ys[2] = coord[5];
            qnt_coord_y = 3;

        }
        if(tipo == 4){
            Xs = (float*)malloc(6*sizeof(float));
            Ys = (float*)malloc(6*sizeof(float));

            raio = coord[2];
            float xc, yc;
            for(int i=0; i<6; i++){
                xc = coord[0] + sin(i*PI/3)*raio;
                yc = coord[1] + cos(i*PI/3)*raio;
                Xs[i] = xc;
                Ys[i] = yc;

            }

            qnt_coord_x = 6;
            qnt_coord_y = 6;
        }

    }

public:
    Figura(int t, float c[], int e, Cor *corSelect, bool l, int rot){

        tipo = t;

        coord = (float*) malloc(e * sizeof(float));
        for(int i=0; i<e; i++){
            coord[i] = c[i];
        }
        qtd_coords = e;
        cor = corSelect;
        modo_fill = l;
        raio = -1;
        set_coordenadas();
        rotacao = rot;
        selected = false;
    }

    void desenhaFigura(){
        // APLICANDO COR
        cor->Aplica();

        if(tipo == 2){ // CIRCULO
            float x = (getXsRot())[0];
            float y = (getYsRot())[0];

            if(modo_fill) circleFill(x, y, raio, DIV_DEFAULT);
            else circle(x, y, raio, DIV_DEFAULT);

            return;
        }

        float *xs = getXsRot();
        float *ys = getYsRot();
        int vertices = 0;

        if(tipo == 1) vertices = 4;  // RETANGULO
        if(tipo == 3) vertices = 3;  // TRIANGULO
        if(tipo == 4) vertices = 6; // HEXAGONO

        if(modo_fill) polygonFill(xs, ys, vertices);
        else polygon(xs, ys, vertices);

    }

    bool pointInPoly(int qtdVert, float *xs, float *ys, float x, float y){
        int i, j;
        bool colisao = false;

        for (i = 0, j = qtdVert-1; i < qtdVert; j = i++) {
            if (((ys[i]>y) != (ys[j]>y)) && (x < (xs[j]-xs[i]) * (y-ys[i]) / (ys[j]-ys[i]) + xs[i])){
                colisao = !colisao;
            }
        }
        return colisao;
    }

    bool Colidiu(int x, int y){

        float *xs = getXsRot();
        float *ys = getYsRot();

        if(tipo == 2){
            int _x = xs[0], _y = ys[0];
            // (xA – a)^2 + (yA – b)^2 < r^2
            if( (pow((x - _x), 2) + pow((y - _y), 2)) < pow(raio, 2) ){
                return true;
            }
        }else{
            int vertices = 0;
            if(tipo == 1) vertices = 4;
            if(tipo == 3) vertices = 3;
            if(tipo == 4) vertices = 6;

            return pointInPoly(vertices, xs, ys, x, y);
        }

        return false;
    }

    int ColidiuHold(int x, int y){

        float *xs = getXsRot();
        float *ys = getYsRot();
        float x_medio, y_medio;
        float tam = 4;

        // COLIDIU HOLD
        if(tipo == 4){
            for(int i=0; i<6; i++){

                x_medio = xs[i];
                y_medio = ys[i];

                if((abs(x_medio - x) < tam) && (abs(y_medio - y) < tam)){
                    return i + 1;
                }
            }
        }else{
            for(int i=0; i<4; i++){
                if(tipo == 1){
                    x_medio = (xs[i%4] + xs[(i+1)%4]) / 2;
                    y_medio = (ys[i%4] + ys[(i+1)%4]) / 2;
                }

                if(tipo == 2){
                    x_medio = xs[0] + cos(i*PI/2)*raio;
                    y_medio = ys[0] + sin(i*PI/2)*raio;
                }

                if(tipo == 3){
                    x_medio = xs[i%3];
                    y_medio = ys[i%3];
                }

                if((abs(x_medio - x) < tam) && (abs(y_medio - y) < tam)){
                    return i + 1;
                }
            }
        }

        return -1;

    }

    void addResize(int qtdX, int qtdY, int lado){

        int dist_x = qtdX;
        int dist_y = qtdY;

        if(tipo == 1){
            for(int i=0; i<2; i++){

                if(abs(rotacao) == 2){
                    dist_x = qtdY;
                    dist_y = -qtdX;
                }

                if(lado == 1 || lado == 3){
                    Xs[(i + lado - 1)%4] += dist_x;
                }

                if(lado == 2 || lado == 4){
                    Ys[(i + lado - 1)%4] += dist_y;
                }
            }
        }

        if(tipo == 2){
            if(lado == 1) raio += dist_x;
            if(lado == 3) raio -= dist_x;

            if(lado == 2) raio += dist_y;
            if(lado == 4) raio -= dist_y;

        }

        if(tipo == 3){
            if(lado == 1 || lado == 3){
                Xs[(lado - 1)%3] += dist_x;
            }
            if(lado == 2){
                Ys[(lado - 1)%3] += dist_y;
            }

        }
        if(tipo == 4){
            if(lado == 1) raio += dist_y;
            if(lado == 2) raio += dist_x;
            if(lado == 3) raio += dist_x;
            if(lado == 4) raio -= dist_y;
            if(lado == 5) raio -= dist_x;
            if(lado == 6) raio -= dist_x;

            atualizaCoords();
        }
    }

    void setFill(Cor *c){
        modo_fill = true;
        cor = c;
    }

    int getTipo(){
        return tipo;
    }
    int getRaio(){
        return raio;
    }

    void setCor(Cor *c){
        cor = c;
    }

    float* getXs(){
        return Xs;
    }
    float* getYs(){
        return Ys;
    }

    float getXMedio(){
        int cont = 0;
        float c = 0;
        for(int i=0; i<qnt_coord_x; i++){
            c += Xs[i];
            cont++;
        }
        return (c/cont);
    }

    float getYMedio(){
        int cont = 0;
        float c = 0;
        for(int i=0; i<qnt_coord_y; i++){
            c += Ys[i];
            cont++;
        }
        return (c/cont);
    }

    float distanciaPontos(float x1, float y1, float x2, float y2){
        return (sqrt(pow((x2-x1),2) + pow((y2-y1),2)));
    }

    float* getXsRot(){
        float *xs = (float*)malloc(qnt_coord_x * sizeof(float));
        float x_medio = getXMedio();
        float y_medio = getYMedio();

        float rot = rotacao%4;

        for(int i=0; i < qnt_coord_x; i++){
            xs[i] = (Xs[i]-x_medio) * cos(rot*PI/4) - (Ys[i]-y_medio) * sin(rot*PI/4) + x_medio;
        }
        return xs;
    }
    float* getYsRot(){
        float *ys = (float*)malloc(qnt_coord_y * sizeof(float));
        float x_medio = getXMedio();
        float y_medio = getYMedio();

        float rot = rotacao%4;

        for(int i=0; i < qnt_coord_y; i++){
            ys[i] = (Xs[i]-x_medio) * sin(rot*PI/4) + (Ys[i]-y_medio) * cos(rot*PI/4) + y_medio;
        }
        return ys;
    }

    void addX(int qtd_x){
        if(tipo == 4){
            coord[0] += qtd_x;
            atualizaCoords();
            return;
        }

        for(int i=0; i<qnt_coord_x; i++){
            Xs[i] += qtd_x;
        }
    }

    void addY(int qtd_y){
        if(tipo == 4){
            coord[1] += qtd_y;
            atualizaCoords();
            return;
        }

        for(int i=0; i<qnt_coord_y; i++){
            Ys[i] += qtd_y;
        }
    }

    void addRotacao(int i){
        rotacao += i;
    }

    int getRotacao(){
        return rotacao;
    }

    float * getCoord(){
        return coord;
    }


    int getQntCoords(){
        return qtd_coords;
    }

    bool getFill(){
        return modo_fill;
    }

    Cor *getCor(){
        return cor;
    }

    void atualizaCoords(){
        float xc, yc;
        float x_medio = coord[0];
        float y_medio = coord[1];
        for(int i=0; i<6; i++){
            xc = x_medio + sin(i*PI/3)*raio;
            yc = y_medio + cos(i*PI/3)*raio;
            Xs[i] = xc;
            Ys[i] = yc;
        }
    }

};

/* Variaveis de gerenciamento das figuras*/
int figura_selecionada = -1;
int redimensionar = -1;
bool fillSelect = false;
bool modoHold = false;
int inicioHoldX = -1;
int inicioHoldY = -1;

#endif
