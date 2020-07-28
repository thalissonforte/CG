/*
    FUNCAO QUE TRATA DOS CALCULOS E OUTRAS ACOES NECESSARIAS
*/
#include <math.h>
#include <vector>

int distancia_origem_x = 0;
int distancia_origem_y = 0;
int distancia_origem_z = 0;

float projecao[2][3] = {
        {1, 0, 0},
        {0, 1, 0}
};

// MULTIPLICACAO DE MATRIZ
Objeto *mulmat(float a[][3], Objeto *b, int linhas_a){

    float b_vet[3] = {b->x - distancia_origem_x, b->y - distancia_origem_y, b->z - distancia_origem_z};

    Objeto *resultado = new Objeto(0, 0, 0);

    for (int i = 0; i < linhas_a; i++) { // LINHAS
        float soma = 0;
        for (int k = 0; k < 3; k++) { // COLUNAS
            soma += a[i][k] * b_vet[k];
        }
        if(i == 0) resultado->x = soma;
        if(i == 1) resultado->y = soma;
        if(i == 2) resultado->z = soma;
    }

    resultado->x += distancia_origem_x;
    resultado->y += distancia_origem_y;
    resultado->z += distancia_origem_z;

    return resultado;
}

// FUNCAO PARA GERAR ANGULO ENTRE 2 PONTOS ( UTILIZANDO COEFICIENTE ANGULAR E ARCTG )
float gerar_angulo_entre_pontos_2(float x1, float y1, float x2, float y2){

    // CALCULO A PARTIR DO COEFICIENTE ANGULAR
    float coeficiente_angular;
    float angulo;

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
    //if(angulo < PI/2) angulo *= -2;

    return angulo;
}

// LIMPAR VECTORS
int limpa_vector(std::vector<Objeto*>& vec){
    for(int i = 0; i < vec.size(); i++){
        delete vec[i];
    }
    //delete vec;
    //std::vector<Objeto*>().swap(vec);
    return vec.capacity();
}

// DISTANCIA ENTRE PONTOS
float distancia_entre_pontos(float x1, float y1, float x2, float y2){
   return ( sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) ) );
}
