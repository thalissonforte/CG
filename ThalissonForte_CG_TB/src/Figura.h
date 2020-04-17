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
            int x = (int) Xs[0];
            int y = (int) Ys[0];

            if(modo_fill) circleFill(x, y, raio, DIV_DEFAULT);
            else circle(x, y, raio, DIV_DEFAULT);

        }else if(tipo == 1){

            float xs[4];
            float ys[4];

            for(int i=0; i<qnt_coord_x; i++){
                xs[i] = Xs[i] * cos(rotacao*PI/4) - Ys[i] * sin(rotacao*PI/4);
                ys[i] = Xs[i] * sin(rotacao*PI/4) + Ys[i] * cos(rotacao*PI/4);

                printf("\nX:%f Y:%f", xs[i], ys[i]);
            }
            printf("\n\n");
            if(modo_fill) polygonFill(xs, ys, 4);
            else polygon(xs, ys, 4);
        }
    }

    bool Colidiu(int x, int y){
        if(tipo == 1){
            if( (x >= Xs[0]) && (x <= Xs[2]) && (y >= Ys[0]) && (y <= Ys[2]) ){
                return true;
            }
        }

        if(tipo == 2){
            int _x = Xs[0], _y = Ys[0];
            // (xA – a)^2 + (yA – b)^2 < r^2
            if( (pow((x - _x), 2) + pow((y - _y), 2)) < pow(raio, 2) ){
                return true;
            }
        }

        return false;
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

    void atualizaCoords(){
        printf("\n\n");

        float ponto_medio_x, ponto_medio_y;
        int raio;
        if(tipo == 1){
            ponto_medio_x = (Xs[0] + Xs[2]) / 2;
            ponto_medio_y = (Ys[0] + Ys[2]) / 2;
            raio = abs(ponto_medio_x);
        }
        if(tipo == 2){
            ponto_medio_x = Xs[0];
            ponto_medio_y = Ys[0];
        }



        for(int i=0; i<qnt_coord_x; i++){
            Xs[i] = Xs[i] * cos(PI/4) - Ys[i] * sin(PI/4);
            Ys[i] = Xs[i] * sin(PI/4) + Ys[i] * cos(PI/4);

            printf("%f\t", Xs[i]);
            printf("%f\t", Ys[i]);
        }
    }

    void addRotacao(int i){
        rotacao += i;
        printf("\nRotacao: %i", rotacao);
        //atualizaCoords();
    }

};

#endif
