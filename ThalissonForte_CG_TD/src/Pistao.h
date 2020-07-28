#ifndef PISTAO_H
#define PISTAO_H

class Pistao{
public:
    float x, y, z;
    float largura, altura;

    float angulo, angulo_x, angulo_y;

    std::vector<Objeto*> Pontos;

    Pistao(float origem_x, float origem_y, float origem_z, float larg, float alt){
        // POSICAO ORIGINAL
        x = origem_x;
        y = origem_y;
        z = origem_z;

        // DIMENSOES
        largura = larg;
        altura = alt;

        // ANGULO INICIAL
        angulo = 0;
        angulo_x = 0;
        angulo_y = 0;

        atualiza_pontos(x, y);

    }

    void desenhar(){
        // RECALCULA ROTACAO
        //rotacionar_biela();

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
            //p_alterado = mulmat(rotacaoZ, Pontos[it], 3);
            p_alterado = mulmat(rotacaoX, Pontos[it], 3);
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


        limpa_vector(pontos_alterados);
    }

    void setAngulo(float angle){
        angulo = angle;
    }

    void atualiza_pontos(float _x, float _y){
        
        x = _x;
        y = _y;

        for(int i = 0; i < Pontos.size(); i++) free(Pontos[i]);

        if(Pontos.size() > 0) Pontos.erase(Pontos.begin() + 0, Pontos.begin() + 8);

        Pontos.push_back(new Objeto(_x - largura/2, _y - altura/2, z-10));
        Pontos.push_back(new Objeto(_x + largura/2, _y - altura/2, z-10));
        Pontos.push_back(new Objeto(_x + largura/2, _y + altura/2, z-10));
        Pontos.push_back(new Objeto(_x - largura/2, _y + altura/2, z-10));

        Pontos.push_back(new Objeto(_x - largura/2, _y - altura/2, z+10));
        Pontos.push_back(new Objeto(_x + largura/2, _y - altura/2, z+10));
        Pontos.push_back(new Objeto(_x + largura/2, _y + altura/2, z+10));
        Pontos.push_back(new Objeto(_x - largura/2, _y + altura/2, z+10));
    }

    void rotacionar_x(float quantidade_rotacao){
        angulo_x += quantidade_rotacao;
    }

    void rotacionar_y(float quantidade_rotacao){
        angulo_y += quantidade_rotacao;
    }

};
#endif
