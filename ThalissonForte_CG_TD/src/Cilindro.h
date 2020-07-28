/*
    FUNCAO RESPONSAVEL PELOS CILINDROS UTILIZADOS NO MOTOR
*/
#ifndef CILINDRO_H
#define CILINDRO_H

class Cilindro{
public:
    float x, y, z;
    float raio, comprimento, quantidade_pontos;

    int posicao; // POSICAO = 0 : HORIZONTAL, POSICAO = 1 : VERTICAL

    float angulo, angulo_x, angulo_y;
    bool modo_fill;
    bool modo_2d;
    float angulo_x_3d, angulo_y_3d;

    std::vector<Objeto*> Pontos;

    Cilindro(float origem_x, float origem_y, float origem_z, float r, float comp, int pos, int qtd_pontos, bool modo){
        // POSICAO ORIGINAL
        x = origem_x;
        y = origem_y;
        z = origem_z;

        // DIMENSOES
        raio = r;
        comprimento = comp;
        //quantidade_pontos = 20;
        posicao = pos;
        quantidade_pontos = qtd_pontos;
        modo_fill = modo;
        modo_2d = false;

        // ANGULO INICIAL
        angulo = 0;
        angulo_x = 0;
        angulo_x_3d = 0;
        angulo_y = 0;
        angulo_y_3d = 0;

        atualiza_pontos(x, y);

    }

    void desenhar(){

        angulo_x = angulo_x_3d;
        angulo_y = angulo_y_3d;

        if(modo_2d){
            angulo_x = 0;
            angulo_y = 0;
        }

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


        std::vector<Objeto*> pontos_alterados;

        // DESENHA OS PONTOS
        for(int it = 0; it < Pontos.size(); it++){
            // COPIA DOS PONTOS ORIGINAIS
            Objeto *p_alterado;

            // MATMUL ROTACAOEIXO E PONTO[IT]
            p_alterado = mulmat(rotacaoX, Pontos[it], 3);
            p_alterado = mulmat(rotacaoY, p_alterado, 3);

            // MATMUL PROJECAO E RESULTADO
            p_alterado = mulmat(projecao, p_alterado, 2);
            //POINT RESULTADO
            point(p_alterado->x, p_alterado->y);
            pontos_alterados.push_back(p_alterado);
        }

        // DESENHA AS LINHAS
        int i;
        for(i = 0; i < quantidade_pontos; i++){
            int aux = (i+1) % (int)quantidade_pontos;
            line(pontos_alterados[i]->x, pontos_alterados[i]->y, pontos_alterados[(i+quantidade_pontos)]->x, pontos_alterados[(i+quantidade_pontos)]->y);
            line(pontos_alterados[i]->x, pontos_alterados[i]->y, pontos_alterados[aux]->x, pontos_alterados[aux]->y);
            line(pontos_alterados[i+quantidade_pontos]->x, pontos_alterados[i+quantidade_pontos]->y, pontos_alterados[(aux+quantidade_pontos)]->x, pontos_alterados[(aux+quantidade_pontos)]->y);

            // MODO FILL
            if(modo_fill){
                if(i < quantidade_pontos/2){
                    int aux2 = quantidade_pontos/2;
                    line(pontos_alterados[i]->x, pontos_alterados[i]->y, pontos_alterados[i + aux2]->x, pontos_alterados[i+aux2]->y);
                    line(pontos_alterados[i+quantidade_pontos]->x, pontos_alterados[i+quantidade_pontos]->y, pontos_alterados[quantidade_pontos + aux2 + i]->x, pontos_alterados[quantidade_pontos+aux2+i]->y);
                }
            }

        }

        // LIMPA VECTOR
        limpa_vector(pontos_alterados);
    }

    void setAngulo(float angle){
        angulo = angle;
    }

    void atualiza_pontos(float _x, float _y){

        x = _x;
        y = _y;

        // LIMPAR PONTOS ALOCADOS ANTERIORMENTE
        for(int i = 0; i < Pontos.size(); i++) free(Pontos[i]);
        // ZERAR PONTOS
        while(Pontos.size() > 0){
            Pontos.erase(Pontos.begin() + 0, Pontos.begin() + quantidade_pontos);
        }

        // RECRIAR PONTOS
        float comprimento2 = comprimento/2;
        float rotacao = 0;

        for(int j = 0; j < 2; j++){
            rotacao = 0;
            for(int i=0; i < quantidade_pontos; i++){

                if(j == 0){
                    if(posicao == 0) Pontos.push_back(new Objeto(_x + raio * cos(rotacao), _y + raio * sin(rotacao), z - comprimento2));
                    if(posicao == 1) Pontos.push_back(new Objeto(_x + raio * cos(rotacao), _y - comprimento2, z + raio * sin(rotacao)));
                }

                if(j == 1){
                    if(posicao == 0) Pontos.push_back(new Objeto(_x + raio * cos(rotacao),  _y + raio * sin(rotacao), z + comprimento2));
                    if(posicao == 1) Pontos.push_back(new Objeto(_x + raio * cos(rotacao), _y + comprimento2, z + raio * sin(rotacao)));
                }

                rotacao += PI_2/quantidade_pontos;
            }
        }
    }

    void rotacionar_x(float quantidade_rotacao){
        angulo_x_3d += quantidade_rotacao;
    }

    void rotacionar_y(float quantidade_rotacao){
        angulo_y_3d += quantidade_rotacao;
    }

    void change_modo(){
        modo_2d = !modo_2d;
    }

};
#endif
