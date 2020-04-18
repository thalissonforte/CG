/*
  FUNCAO QUE GERENCIA AS FIGURAS

    Por padrão as coordenadas das figuras são sempre para que seja possível
    a utilização da função polygon (menos figuras circulares), portanto até
    mesmo retângulos precisam conter as coordenadas de todas as pontas.

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
    4 = ...
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
    }

public:
    Figura(int t, float c[], int e, Cor *corSelect, bool l){

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
        rotacao = 0;
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

        }else if(tipo == 1){

            float *xs = getXsRot();
            float *ys = getYsRot();

            if(modo_fill) polygonFill(xs, ys, 4);
            else polygon(xs, ys, 4);
        }
    }

    bool Colidiu(int x, int y){

        float *xs = getXsRot();
        float *ys = getYsRot();

        if(tipo == 1){
            // COLIDIU FIGURA
            float x_maior, x_menor, y_maior, y_menor;
            x_maior = xs[0];
            x_menor = xs[0];
            y_maior = ys[0];
            y_menor = ys[0];

            for(int i=0; i<qnt_coord_x; i++){
                if(x_maior < xs[i]) x_maior = xs[i];
                if(x_menor > xs[i]) x_menor = xs[i];
                if(y_maior < ys[i]) y_maior = ys[i];
                if(y_menor > ys[i]) y_menor = ys[i];
            }

            if( (x >= x_menor) && (x <= x_maior) && (y >= y_menor) && (y <= y_maior) ){
                return true;
            }
        }

        if(tipo == 2){
            int _x = xs[0], _y = ys[0];
            // (xA – a)^2 + (yA – b)^2 < r^2
            if( (pow((x - _x), 2) + pow((y - _y), 2)) < pow(raio, 2) ){
                return true;
            }
        }

        return false;
    }

    int ColidiuHold(int x, int y){

        float *xs = getXsRot();
        float *ys = getYsRot();
        float x_medio, y_medio;
        int tam = 4;

        // COLIDIU HOLD
        for(int i=0; i<qnt_coord_x; i++){
            if(tipo == 1){
                x_medio = (xs[i%4] + xs[(i+1)%4]) / 2;
                y_medio = (ys[i%4] + ys[(i+1)%4]) / 2;
            }

            if(tipo == 2){
                x_medio = xs[0] + sin(i*PI/2)*raio;
                y_medio = ys[0] + cos(i*PI/2)*raio;
            }

            if(abs(x_medio - x) < tam && abs(y_medio - y) < tam){
                return i + 1;
            }
        }

        return -1;

    }

    void addXResize(int qtd, int lado){
        //printf("\nLado %i: %i", lado, qtd);
        if(tipo == 1){
            for(int i = 0; i < 2; i++){
                if(lado == 1 || lado == 3){
                    Xs[(i + lado - 1)%4] += qtd;
                    //Xs[(i + lado - 1)%4] += qtd;
                }
            }
        }
        if(tipo == 2){
            raio += qtd;
        }
    }

    void addYResize(int qtd, int lado){
        if(tipo == 1){
            for(int i = 0; i < 2; i++){
                if(lado == 2 || lado == 4){
                    Ys[(i + lado - 1)%4] += qtd;
                }
            }
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
        for(int i=0; i < qnt_coord_x; i++){
            xs[i] = (Xs[i]-x_medio) * cos(rotacao*PI/4) - (Ys[i]-y_medio) * sin(rotacao*PI/4) + x_medio;
        }
        return xs;
    }
    float* getYsRot(){
        float *ys = (float*)malloc(qnt_coord_y * sizeof(float));
        float x_medio = getXMedio();
        float y_medio = getYMedio();
        for(int i=0; i < qnt_coord_y; i++){
            ys[i] = (Xs[i]-x_medio) * sin(rotacao*PI/4) + (Ys[i]-y_medio) * cos(rotacao*PI/4) + y_medio;
        }
        return ys;
    }

    void addX(int qtd_x){
        for(int i=0; i<qnt_coord_x; i++){
            Xs[i] += qtd_x;
        }
    }

    void addY(int qtd_y){
        for(int i=0; i<qnt_coord_y; i++){
            Ys[i] += qtd_y;
        }
    }

    void addRotacao(int i){
        rotacao += i;
        printf("\nRotacao: %i", rotacao);
        //atualizaCoords();
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

};

#endif
