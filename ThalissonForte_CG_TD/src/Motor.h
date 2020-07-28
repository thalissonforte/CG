#ifndef MOTOR_H
#define MOTOR_H

#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>


class Motor{
private:

public:
    float x, y, z;
    float raio, largura;
    float angulo;

    float distancia_pistao;
    float distancia_biela;

    bool motor_ativo;

    float angulo_x;
    float angulo_y;

    std::vector<Objeto*> Pontos;

    Biela *biela;
    Pistao *pistao;
    Cilindro *cilindro_1;

    Motor(float origem_x, float origem_y, float origem_z, float r, float larg){

        // CENTRO DO OBJETO
        x = origem_x;
        y = origem_y;
        z = origem_z;

        // TAMANHOS
        raio = r;
        largura = larg;

        // ANGULOS
        angulo_x = 0;
        angulo_y = 0;

        // INICIAR
        motor_ativo = true;

        // DEFINICOES P AUXILIAR DESENHO
        distancia_origem_x = x;
        distancia_origem_y = y;
        distancia_origem_z = z;

        // raio / 4 ate a origem - deitado direita
        Pontos.push_back(new Objeto(x - raio/4, y - largura/2, z-10));
        Pontos.push_back(new Objeto(x + 3*raio/4, y - largura/4, z-10));
        Pontos.push_back(new Objeto(x + 3*raio/4, y + largura/4, z-10));
        Pontos.push_back(new Objeto(x - raio/4, y + largura/2, z-10));

        Pontos.push_back(new Objeto(x - raio/4, y - largura/2, z+10));
        Pontos.push_back(new Objeto(x + 3*raio/4, y - largura/4, z+10));
        Pontos.push_back(new Objeto(x + 3*raio/4, y + largura/4, z+10));
        Pontos.push_back(new Objeto(x - raio/4, y + largura/2, z+10));

        // CRIACAO DO PISTAO
        pistao = new Pistao(x, y + raio*2, z, largura/2 + 10, largura + 20);

        // CRIACAO DA BIELA
        biela = new Biela(x + 2*raio/4, y, z + 20, raio*2, largura, pistao);

        // CRIACAO DO CILINDRO 1 (junta virabrequim-biela)
        cilindro_1 = new Cilindro(x + 2*raio/4, y, z+10, 10, 50);

    }

    void desenhar(){

        // RECALCULA ROTACAO
        float rotacaoX[3][3] = {
            {1, 0, 0},
            {0, cos(angulo_x), -sin(angulo_x)},
            {0, sin(angulo_x), cos(angulo_x)}
        };

        float rotacaoY[3][3] = {
            {cos(angulo_y), 0, -sin(angulo_y)},
            {0, 1, 0},
            {sin(angulo_y), 0, cos(angulo_y)}
        };

        float rotacaoZ[3][3] = {
            {cos(angulo), -sin(angulo), 0},
            {sin(angulo), cos(angulo), 0},
            {0, 0, 1}
        };



         //printf("\n%f", angle);
        std::vector<Objeto*> pontos_alterados;

        // DESENHA OS PONTOS
        for(int it = 0; it < Pontos.size(); it++){
            Objeto *p_alterado;
            // MATMUL ROTACAOEIXO E PONTO[IT]
                // PRIMEIRA ROTACAO EM Z
            if(motor_ativo) p_alterado = mulmat(rotacaoZ, Pontos[it], 3);
            p_alterado = mulmat(rotacaoX, p_alterado, 3);
            p_alterado = mulmat(rotacaoY, p_alterado, 3);


            //p_alterado = mulmat(rotacaoZ, p_alterado, 3);
            // MATMUL PROJECAO E RESULTADO
            //p_alterado = mulmat(projecao, p_alterado, 2);
            p_alterado = mulmat(projecao, p_alterado, 2);
            //POINT RESULTADO
            point(p_alterado->x, p_alterado->y);
            pontos_alterados.push_back(p_alterado);
        }

        // DESENHA AS LINHAS
        int i;
        for(i = 0; i < 4; i++){
            line(pontos_alterados[i]->x, pontos_alterados[i]->y, pontos_alterados[(i+1)%4]->x, pontos_alterados[(i+1)%4]->y);
            line(pontos_alterados[i+4]->x, pontos_alterados[i+4]->y, pontos_alterados[(i+1)%4 +4]->x, pontos_alterados[(i+1)%4 + 4]->y);
            line(pontos_alterados[i]->x, pontos_alterados[i]->y, pontos_alterados[i+4]->x, pontos_alterados[i+4]->y);
        }


        // DESENHAR DEMAIS PECAS
        biela->desenhar();
        pistao->desenhar();
        cilindro_1->desenhar();

        //printf("\n%i", pontos_alterados.capacity());

        // LIMPEZA DO VECTOR E DESALOCAR OBJETOS
        limpa_vector(pontos_alterados);
        //printf("\t%i", limpa_vector(pontos_alterados));
        //return vec.capacity();
        //delete pontos_alterados;
    }

    void setAngulo(float angle){
        angulo = angle;

        // SETAR ANGULO DAS DEMAIS PECAS
            // BIELA
        biela->setAngulo(angulo);
        distancia_biela = (2*raio/4);
        float x_biela = x + distancia_biela * cos(angulo);
        float y_biela = y + largura * sin(angulo);
        // ainda nao atualiza

            // PISTAO
        pistao->setAngulo(angulo);
        float dist_origem_biela = raio/2;
        float dist_biela_pistao = raio*3/2;
        float y_pistao = dist_origem_biela * sin(angulo) + sqrt(pow(dist_biela_pistao + y, 2) - pow(dist_origem_biela, 2)*pow(cos(angulo), 2));
        //distancia_pistao = y_pistao;
        pistao->atualiza_pontos(x, y_pistao);

        cilindro_1->setAngulo(angulo);
        cilindro_1->atualiza_pontos(x_biela, y_biela);

        // CALCULO DA ANGULACAO
        //float angulos[2];
        //printf("\n%f \t%f \t%f \t%f\t%f", x_biela, y_biela, x, y_pistao, largura);
        //gerar_angulo_entre_pontos(x_biela, y_biela, x, y_pistao, largura, angulos);

        //float angulo_entre_bielapistao = gerar_angulo_entre_pontos_2(x_biela, y_biela, x, y_pistao);

        //line(x, y, pistao->x, pistao->y);
        //line(x_biela, y_biela, pistao->x, y_pistao);

        // ATUALIZAR BIELA COM ANGULACAO CERTA
        biela->atualiza_pontos(x_biela, y_biela);

    }

    void rotacionar_x(float quantidade_rotacao){
        angulo_x += quantidade_rotacao;

        // ROTACIONAR DEMAIS PECAS
        biela->rotacionar_x(quantidade_rotacao);
        pistao->rotacionar_x(quantidade_rotacao);
        cilindro_1->rotacionar_x(quantidade_rotacao);

    }

    void rotacionar_y(float quantidade_rotacao){
        angulo_y += quantidade_rotacao;

        // ROTACIONAR DEMAIS PECAS
        biela->rotacionar_y(quantidade_rotacao);
        pistao->rotacionar_y(quantidade_rotacao);
        cilindro_1->rotacionar_y(quantidade_rotacao);
    }

    void rotacionar_z(float quantidade_rotacao){
        //angulo += quantidade_rotacao;
    }



};
#endif
