/*
  FUNCAO QUE IMPLEMENTA O MANAGER DOS PONTOS
*/
#ifndef __MANAGERPONTOS_H__
#define __MANAGERPONTOS_H__

#include "gl_canvas2d.h"

class ManagerPontos{
    // VETORES DE PONTOS IMPORTANTES  
    Ponto **pontos_controle;
    Ponto **pontos_animados_vet;
    Ponto **pontos_curva;

    // INFORMACOES IMPORTANTES
    int quantidade_pontos;
    bool habilitaCurva;
    int fps_pontos;

private:

    // DESENHO DAS LINHAS PRINCIPAIS E FIXAS
    void desenha_linhas(){
        if(quantidade_pontos > 1){
            Ponto *a;
            Ponto *b;
            for(int i=0; i<quantidade_pontos-1; i++){
                a = pontos_controle[i];
                b = pontos_controle[i+1];
                line(a->getX(), a->getY(), b->getX(), b->getY());

            }
        }
    }

    float fatorial(int x){
        float res = 1;
        for(int i=x; i>1; i--){
            res *= i;
        }
        return res;
    }

    // POLINOMIO QUE CALCULA OS PONTOS DA CURVA
    Ponto * polinomio(int idx, float t){
        float res = 0, ni, j, resX = 0, resY = 0;
        int n = quantidade_pontos - 1;

        for(int i=0; i<=n; i++){
            ni = fatorial(n) / (fatorial(i) * fatorial(n - i));
            j = ni * pow(t, i) * pow((1-t), (n-i));

            resX += pontos_controle[i]->getX() * j;
            resY += pontos_controle[i]->getY() * j;
        }

        color(1,0,0);
        Ponto *result_ponto = new Ponto(0, resX, resY);
        return result_ponto;
    }

    // ----------------- DESENHO DOS OBJETOS ANIMADOS (PONTOS E LINHAS) ----------------------------
    void desenha_animacao_recursiva(int pontos_animados_feitos, int offset, int qtd_prox){
        if(qtd_prox < 2) return;
        int atual = qtd_prox;


        float x_a, y_a;

        for(int i=0; i<qtd_prox; i++){

            x_a = (1-tempo)*(pontos_animados_vet[i+offset]->getX()) + tempo*(pontos_animados_vet[i+offset+1]->getX());
            y_a = (1-tempo)*(pontos_animados_vet[i+offset]->getY()) + tempo*(pontos_animados_vet[i+offset+1]->getY());

            // ATUALIZAR POSICOES DOS PONTOS ANIMADOS
            pontos_animados_vet[pontos_animados_feitos + i]->setX(x_a);
            pontos_animados_vet[pontos_animados_feitos + i]->setY(y_a);

            pontos_animados_vet[pontos_animados_feitos + i]->Render();
        }

        for(int i=0; i<qtd_prox-1; i++){
            cores[qtd_prox%4]->Aplica();
            line(pontos_animados_vet[pontos_animados_feitos + i]->getX(), pontos_animados_vet[pontos_animados_feitos + i]->getY(), pontos_animados_vet[pontos_animados_feitos + i +1]->getX(), pontos_animados_vet[pontos_animados_feitos + i +1]->getY());
        }

        desenha_animacao_recursiva(pontos_animados_feitos+atual, pontos_animados_feitos, qtd_prox-1);
    }
    void desenha_animacao(){
        // OBJETOS ANIMADOS
        int pontos_animados = quantidade_pontos-1;
        int linhas_animadas = quantidade_pontos-2;
        float x_a, y_a;

        for(int i=0; i<pontos_animados; i++){
            color(1,0,0);
            x_a = (1-tempo)*(pontos_controle[i]->getX()) + tempo*(pontos_controle[i+1]->getX());
            y_a = (1-tempo)*(pontos_controle[i]->getY()) + tempo*(pontos_controle[i+1]->getY());

            pontos_animados_vet[i]->setX(x_a);
            pontos_animados_vet[i]->setY(y_a);

            if(i<pontos_animados-1)
                line(pontos_animados_vet[i]->getX(), pontos_animados_vet[i]->getY(), pontos_animados_vet[i+1]->getX(), pontos_animados_vet[i+1]->getY());

            // RENDER NOS PONTOS INICIAIS
            pontos_animados_vet[i]->Render();
        }
        // CALCULO DE QUANTOS PONTOS FALTAM
        int pontos_faltantes = 0;
        for(int j = 2; j < pontos_animados; j++){
            pontos_faltantes += j;
        }
        desenha_animacao_recursiva(pontos_animados, 0, pontos_animados-1);
    }
    // -----------------------------------------------------------------------

    // --------------- FUNCOES QUE GERENCIAM A CURVA E O DESENHO DELA
    void calculaCurva(){

        if(pontos_curva != NULL) free(pontos_curva);

        float qtd_frames = 1/inc_tempo;
        pontos_curva = (Ponto**)malloc(qtd_frames * sizeof(Ponto*));

        float time = 0;

        for(int i=0; i<qtd_frames; i++){
            pontos_curva[i] = polinomio(0, time);
            time += inc_tempo;
        }
        habilitaCurva = true;
    }
    void desenha_curva(){
        if(habilitaCurva){
            for(int i=0; i < tempo/inc_tempo; i++){
                circleFill(pontos_curva[i]->getX(), pontos_curva[i]->getY(), 2, 20);
            }
        }

    }
// ------------------------------------------------

public:
    ManagerPontos(){
        quantidade_pontos = 0;
        pontos_controle = (Ponto**)malloc(sizeof(Ponto*));
        pontos_animados_vet = (Ponto**)malloc(sizeof(Ponto*));
        pontos_curva = (Ponto**)malloc(sizeof(Ponto*));
        habilitaCurva = false;

    }

    // EXIBIR PONTOS DE CONTROLE
    void Render(){
        for(int i = 0; i<quantidade_pontos; i++){
            pontos_controle[i]->Render();
        }

        // DESENHOS
        desenha_linhas();
        desenha_animacao();
        desenha_curva();
        
    }

  // VERIFICAR CLICK SOBRE PONTO
    int Colidiu(int mx, int my){
        int id = -1;

        for(int i = 0; i<quantidade_pontos; i++){
            if(pontos_controle[i]->Colidiu(mx, my) != -1) id = pontos_controle[i]->Colidiu(mx, my);
        }

        return id;
    }

      

    // FUNCAO PARA SETAR FPS
    void setFPS(int _fps){
        fps_pontos = _fps;
    }

    // ATUALIZACAO DOS DEMAIS PONTOS A CADA MODIFICACAO NA CURVA
    void atualizaPontosAnimados(){
        if(quantidade_pontos > 2){
            int pontos_animados = quantidade_pontos - 1;
            int sum = 0;
            // SOMAR O TOTAL DE PONTOS ANIMADOS QUE VAO EXISTIR (SE 4 PONTOS DE CONTROLE: 3 + 2 PONTOS ANIMADOS)
            for(int i=2; i<=pontos_animados; i++){
                sum += i;
            }

            pontos_animados_vet = (Ponto**)realloc(pontos_animados_vet, (sum)*sizeof(Ponto*));

            for(int i=0; i<sum; i++){
                Ponto *p = new Ponto(1, pontos_controle[i%quantidade_pontos]->getX(), pontos_controle[i%quantidade_pontos]->getY());
                pontos_animados_vet[i] = p;
            }
        }
    }

    // QUANTIDADE DE PONTOS
    int getQuantidadePontos(){
        return quantidade_pontos;
    }

    // MOVIMENTO DO PONTO
    void moverPonto(int _id, int _x, int _y){
        habilitaCurva = false;
        (pontos_controle[_id])->addX(_x);
        (pontos_controle[_id])->addY(_y);
        calculaCurva();
    }

    // ADICIONAR E EXCLUIR PONTO
        // GERENCIAMENTO ANTES DE ADICIONAR
    void addNovoPonto(){
        habilitaCurva = false;
        int _x = pontos_controle[quantidade_pontos-1]->getX();
        int _y = pontos_controle[quantidade_pontos-1]->getY();

        addPonto((_x + 100) % (screenWidth-260) + 30, (_y + 100) % (screenHeight - 90) + 60);
    }
        // FUNCAO QUE REALMENTE ADICIONA PONTO
    void addPonto(int x, int y){
        habilitaCurva = false;
        Ponto *novoPonto = new Ponto(quantidade_pontos, x, y);
        pontos_controle = (Ponto**)realloc(pontos_controle, (quantidade_pontos+1)*sizeof(Ponto*));
        pontos_controle[quantidade_pontos] = novoPonto;
        quantidade_pontos++;

        if(quantidade_pontos > 1){
            desenha_linhas();
        }
        atualizaPontosAnimados();
        calculaCurva();
    }
    void removePonto(){
        habilitaCurva = false;
        if(quantidade_pontos > 1){
            Ponto *p = pontos_controle[quantidade_pontos-1];
            quantidade_pontos -= 1;
            free(p);
        }
        calculaCurva();
    }

    // FUNCAO AUXILIAR DE DESENHO DO GRAFICO
    float blendFunct(int pos_x, int pos_y){

        float x_a, y_a;
        float ni, j;
        int n = quantidade_pontos-1;

        color(1, 0, 0);

        for(int i=0; i <= n; i++){

            for(float t=0; t<1; t += 0.00125){
                ni = fatorial(n) / (fatorial(i) * fatorial(n - i));
                j = ni * pow(t, i) * pow((1-t), (n-i));

                point(pos_x + t*150, pos_y + 80*j);
            }

        }
    }
};
// VARIAVEIS NECESSARIAS PARA CONTROLE
    bool modo_animacao = false;
    bool modoHold = false;
    int inicioHoldX = -1;
    int inicioHoldY = -1;
    int ponto_selecionado;

#endif
