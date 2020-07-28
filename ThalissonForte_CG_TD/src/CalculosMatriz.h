#include <math.h>
#include <vector>

int distancia_origem_x = 0;
int distancia_origem_y = 0;
int distancia_origem_z = 0;

class PontosOrientados{
public:
    float *xs;
    float *ys;

    PontosOrientados(float *x, float *y){
        xs = x;
        ys = y;
    }
};


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

// GERACAO DO AJUSTE DE ANGULO ENTRE 2 PONTOS
float *gerar_angulo_entre_pontos(float x1, float y1, float x2, float y2, float larg, float *vec_result){

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

    //printf("\nAngulo %f", angulo * 180 / PI);
    //printf("\nAngulo %f", angulo * 180 / PI);
    float ajuste_angulo_y = div*sin(1.5708 + angulo);
    float ajuste_angulo_x = div*cos(1.5708 + angulo);

    if(quadrante == 2){
        ajuste_angulo_y = ajuste_angulo_x;
        ajuste_angulo_x = div*sin(1.5708 + angulo);
    }else if(quadrante == 4){
        ajuste_angulo_y = ajuste_angulo_x;
        ajuste_angulo_x = div*sin(1.5708 + angulo);;
    }

    float xs[] = {x1 - ajuste_angulo_x, x1 + ajuste_angulo_x, x2 + ajuste_angulo_x, x2 - ajuste_angulo_x};
    float ys[] = {y1 - ajuste_angulo_y, y1 + ajuste_angulo_y, y2 + ajuste_angulo_y, y2 - ajuste_angulo_y};

    float resultados[] = {ajuste_angulo_x, ajuste_angulo_y};


    (vec_result)[0] = ajuste_angulo_x;
    (vec_result)[1] = ajuste_angulo_y;

    //polygon(xs, ys, 4);
    //PontosOrientados *p = new PontosOrientados(xs, ys);
    //return p;
    //printf("\n%f\t%f\t%f", resultados[0], resultados[1], angulo* 180 / PI);
    return resultados;
}

// NOVA FUNCAO PARA GERAR ANGULO ENTRE 2 PONTOS (MAIS SIMPLIFICADA)
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
