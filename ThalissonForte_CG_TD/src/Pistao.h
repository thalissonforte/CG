#ifndef PISTAO_H
#define PISTAO_H

class Pistao{
public:
    // DIMENSOES E ANGULOS
    float x, y, z;
    float raio, comprimento;
    float angulo, angulo_x, angulo_y, angulo_x_3d, angulo_y_3d;

    // CONFIGURACOES
    bool modo_2d;
    bool pistao_exibir, camisa_exibir;

    // PARTES
    Cilindro *pistao_real;
    Cilindro *camisa;

    std::vector<Objeto*> Pontos;

    Pistao(float origem_x, float origem_y, float origem_z, float r, float comp){
        // POSICAO ORIGINAL
        x = origem_x;
        y = origem_y;
        z = origem_z;

        // DIMENSOES
        raio = r;
        comprimento = comp;

        // ANGULO INICIAL
        angulo = 0;
        angulo_x = 0;
        angulo_x_3d = 0;
        angulo_y = 0;
        angulo_y_3d = 0;
        modo_2d = false;

        // CRIACAO E EXIBICAO DAS PARTES
        pistao_real = new Cilindro(x, y, z, raio, comprimento, 1, 20, true);
        camisa = new Cilindro(x, y - comprimento*3/4, z, raio+10, comprimento*2.5, 1, 10, false);
        pistao_exibir = true;
        camisa_exibir = true;

    }

    void desenhar(){
        // DESENHAR O CILINDRO DO PISTAO
        if(pistao_exibir) pistao_real->desenhar();
        if(camisa_exibir) camisa->desenhar();
    }

    void setAngulo(float angle){
        angulo = angle;
    }

    void atualiza_pontos(float _x, float _y){
        
        x = _x;
        y = _y;

        pistao_real->atualiza_pontos(x, y);
    }

    void rotacionar_x(float quantidade_rotacao){
        angulo_x_3d += quantidade_rotacao;
        pistao_real->rotacionar_x(quantidade_rotacao);
        camisa->rotacionar_x(quantidade_rotacao);
    }

    void rotacionar_y(float quantidade_rotacao){
        angulo_y_3d += quantidade_rotacao;
        pistao_real->rotacionar_y(quantidade_rotacao);
        camisa->rotacionar_y(quantidade_rotacao);
    }

    void change_pistao_exibir(){
        pistao_exibir = !pistao_exibir;
    }

    void change_camisa_exibir(){
        camisa_exibir = !camisa_exibir;
    }

    void change_modo(){
        modo_2d = !modo_2d;
        pistao_real->change_modo();
        camisa->change_modo();
    }

};
#endif
