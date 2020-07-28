#ifndef MANIVELA_H
#define MANIVELA_H

#define DIM 20
#define screenX 780
#define screenY 530
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>

class Ponto{
    public:
        float x, y, z;

    Ponto(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

/*
class PontosOrientados{
public:
    float *xs;
    float *ys;

    PontosOrientados(float *x, float *y){
        xs = x;
        ys = y;
    }
};
*/


class Manivela{

private:
    //Ponto origem;
    float x, y, z;

    float raio, largura;
    float angulo;

    std::vector<Objeto*> Pontos;

public:

    Manivela(float origem_x, float origem_y, float origem_z, float r, float larg){

        x = origem_x;
        y = origem_y;
        z = origem_z;
        raio = r;
        angulo = 0;

        largura = larg;

        // FUNCAO QUE ATUALIZA OS PONTOS DE ACORDO COM O ANGULO
        atualiza_pontos();

    }

    PontosOrientados *gerar_retangulo(float x1, float y1, float x2, float y2, int larg){

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
       PontosOrientados *p = new PontosOrientados(xs, ys);
       return p;
     }

    void atualiza_pontos(){

        // USANDO FUNCAO GERADORA DE RETANGULOS ( origem ate biela )
        PontosOrientados *p = gerar_retangulo(x, y, x + raio*cos(angulo), y + raio*sin(angulo), largura);

        // ZERAR TODOS OS PONTOS EXISTENTES
        if(Pontos.size()>0) Pontos.erase(Pontos.begin() + 0, Pontos.begin() + 8);
        
        float *xs = p->xs;
        float *ys = p->ys;
        //printf("%f", xs[0]);

        Pontos.push_back(new Objeto(xs[0], ys[0], z+largura/2));
        Pontos.push_back(new Objeto(xs[1], ys[1], z+largura/2));

        Pontos.push_back(new Objeto(xs[2], ys[2], z+largura/2));
        Pontos.push_back(new Objeto(xs[3], ys[3], z+largura/2));

        Pontos.push_back(new Objeto(xs[0], ys[0], z-largura/2));
        Pontos.push_back(new Objeto(xs[1], ys[1], z-largura/2));

        Pontos.push_back(new Objeto(xs[2], ys[2], z-largura/2));
        Pontos.push_back(new Objeto(xs[3], ys[3], z-largura/2));

        // LADO CENTRO
        /*
        Pontos.push_back(new Objeto(x-largura/2, y-largura/2, z+largura/2));
        Pontos.push_back(new Objeto(x-largura/2, y+largura/2, z+largura/2));
        Pontos.push_back(new Objeto(x+largura/2, y-largura/2, z+largura/2));
        Pontos.push_back(new Objeto(x+largura/2, y+largura/2, z+largura/2));

    
        // PONTOS DA ORIGEM-BIELA
        Pontos.push_back(new Objeto(x-largura/2 + raio*cos(angulo), y-largura/2 + raio*sin(angulo), z-largura/2));
        Pontos.push_back(new Objeto(x-largura/2 + raio*cos(angulo), y+largura/2 + raio*sin(angulo), z-largura/2));
        Pontos.push_back(new Objeto(x+largura/2 + raio*cos(angulo), y+largura/2 + raio*sin(angulo), z-largura/2));
        Pontos.push_back(new Objeto(x+largura/2 + raio*cos(angulo), y-largura/2 + raio*sin(angulo), z-largura/2));*/

    }

    void setAngulo(float ang){
        angulo = ang;
        atualiza_pontos();
    }

    void Draw2D(float dist_screen){

        Objeto* origem = new Objeto(this->Pontos[0]->x + largura/2, this->Pontos[0]->y + largura/2, 0);

        std::vector<Objeto*> Pontos2d;
        for(auto i = 0; i<8; i++){
                Pontos2d.push_back(new Objeto(this->Pontos[i]->x*dist_screen/this->Pontos[i]->z, this->Pontos[i]->y*dist_screen/this->Pontos[i]->z, 0));
        }
        for(auto i = 0; i<8; i++){
            Pontos2d[i]->x += origem->x;
            Pontos2d[i]->y += origem->y;

        }

        line(Pontos2d[0]->x, Pontos2d[0]->y, Pontos2d[1]->x, Pontos2d[1]->y);
        line(Pontos2d[1]->x, Pontos2d[1]->y, Pontos2d[2]->x, Pontos2d[2]->y);
        line(Pontos2d[2]->x, Pontos2d[2]->y, Pontos2d[3]->x, Pontos2d[3]->y);
        line(Pontos2d[0]->x, Pontos2d[0]->y, Pontos2d[3]->x, Pontos2d[3]->y);

        line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[5]->x, Pontos2d[5]->y);
        line(Pontos2d[5]->x, Pontos2d[5]->y, Pontos2d[6]->x, Pontos2d[6]->y);
        line(Pontos2d[6]->x, Pontos2d[6]->y, Pontos2d[7]->x, Pontos2d[7]->y);
        line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[7]->x, Pontos2d[7]->y);

        line(Pontos2d[1]->x, Pontos2d[1]->y, Pontos2d[5]->x, Pontos2d[5]->y);
        line(Pontos2d[2]->x, Pontos2d[2]->y, Pontos2d[6]->x, Pontos2d[6]->y);
        line(Pontos2d[3]->x, Pontos2d[3]->y, Pontos2d[7]->x, Pontos2d[7]->y);
        line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[0]->x, Pontos2d[0]->y);

    }

    void MoveX(float dist){

        for(auto i = 0; i<8; i++){
            this->Pontos[i]->x += dist;
        }

    }

    void MoveY(float dist){

        for(auto i = 0; i<8; i++){
            this->Pontos[i]->y += dist;
        }

    }

    void SetX(float variacao){
        float x_new, y_new, z_new;

        for(int i=4; i<8; i++){
            x_new = this->Pontos[i]->x * cos(variacao);
            y_new = this->Pontos[i]->y * sin(variacao);
            z_new = this->Pontos[i]->z;

            this->Pontos[i] = new Objeto(x_new, y_new, z_new);

            //Objeto *Lib = this->Pontos[i];



        }



    }

    void SetY(float coord){
        for(auto i = 0; i<8; i++){
            this->Pontos[i]->y = coord;
        }
    }

   // ROTACAO 3D
    void rotate_x(float qtd){

        float seno = sin(qtd);
        float cosseno = cos(qtd);

        for(int i=0; i<Pontos.size(); i++){
            Objeto *atual = Pontos[i];
            float _y, _z;
            _y = atual->y;
            _z = atual->z;

            atual->y = _y * cosseno - _z * seno;
            atual->z = _z * cosseno + _y * seno;
        }
   }

    void rotate_y(float qtd){

        float seno = sin(qtd);
        float cosseno = cos(qtd);

        for(int i=0; i<Pontos.size(); i++){
            Objeto *atual = Pontos[i];
            float _x, _z;
            _x = atual->x;
            _z = atual->z;

            atual->x = _x * cosseno - _z * seno;
            atual->z = _z * cosseno + _x * seno;
        }
   }


   Objeto *rotacionaX(Objeto p ){
        Objeto *resp;
        int ang;
        ang += 0.00001;
        resp = new Objeto(p.x, cos(ang)*p.y - sin(ang)*p.z, sin(ang)*p.y + cos(ang)*p.z);

        return resp;
   }

   Objeto *rotacionaY(Objeto p ){
        Objeto *resp;
        int ang;
        ang += 0.00001;
        resp = new Objeto(cos(ang)*p.x - sin(ang)*p.z, p.y, sin(ang)*p.x + cos(ang)*p.z);

        return resp;
   }

    /*
   //faz a translacao de um unico ponto
   Ponto translada(Ponto p)
   {
       Ponto resp;

       resp.x = p.x;
       resp.y = p.y - 10 ;
       resp.z = p.z - 12;

       return resp;
   }

   //projecao em perspectiva, assumindo camera na origem olhando para z negativo.
   Ponto projeta( Ponto p )
   {
       float d = -10.0;
       Ponto resp;

       resp.x = (p.x*d) / p.z;
       resp.y = (p.y*d) / p.z;
       resp.z = 0;

       return resp;
   }

   //aplica sequencia de transformacoes na malha para fazer a animacao e visualizacao.
   void transforma()
   {
       Ponto p;
      //Processa cada vertice da superficie individualmente.
      for(int x=0; x<=DIM; x++)
         for(int z=0; z<=DIM; z++)
         {
              //copia os pontos originais
              p = mat[x][z];

              //rotacao no eixo x
              //p = rotacionaX( p );

              //translacao no eixo z
              p = translada( p );

              //projecao em perspectiva
              transf[x][z] = projeta( p );
         }
   }

   void render()
   {
      glPointSize(4);

      //a cada renderizacao aplica uma transformacao na superficie que faz a rotacao e projecao em perspectiva.
      //transforma();

      //desenha os vertices
      /*for(int x=0; x<=DIM; x++)
         for(int z=0; z<=DIM; z++)
         {
             point(transf[x][z].x, transf[x][z].y);
         }

      //desenha as arestas
      for(int x=0; x<DIM; x++)
         for(int z=0; z<DIM; z++)
         {
             line(transf[x][z].x, transf[x][z].y, transf[x+1][z].x, transf[x+1][z].y );
             line(transf[x][z].x, transf[x][z].y, transf[x][z+1].x, transf[x][z+1].y );
         }


   }
   */

};

#endif
