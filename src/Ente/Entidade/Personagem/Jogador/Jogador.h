
#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include <iostream>
#include "Sistema/Input/Observador_Input.h"
#include "Ente/Entidade/Personagem/Personagem.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Gerenciadores {
    class Observador_Input;
    class Gerenciador_Gravidade;
}

namespace Obstaculos {
    class Plataforma;
}
 
namespace Personagens {
    class Inimigo;

    enum EscolhaCampeao {
        CAMPEAO_NAAFIRI = 0,
        CAMPEAO_JHIN,
        CAMPEAO_LUX,
        CAMPEAO_EVELYNN,
        CAMPEAO_GWEN,
        CAMPEAO_PYKE,
        CAMPEAO_SETT,
        CAMPEAO_SHACO,
        CAMPEAO_VIEGO
    };

    class Jogador : public Personagem {
    private:
        Gerenciadores::Observador_Input* ObserverJogador;
        Gerenciadores::Gerenciador_Gravidade* pGravidade;
    protected:
        float pontos;
        int abates;
    public:
        Jogador();
        ~Jogador();

        void setGerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade* g);
        Gerenciadores::Gerenciador_Gravidade* getGerenciadorGravidade();
        Gerenciadores::Observador_Input* getObserver();

        void executar();
        void salvar();
        void mover();
        void setCampeao(EscolhaCampeao campeao);

        float getPontos() const { return pontos; }
        int getAbates() const { return abates; }
        void adicionarPontos(float valor);
        void registrarAbate();

        void aoColidir(Entidade* E) { E->interagir_Colisao(this); }
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
} // Personagens

#endif //JOGO_JOGADOR_H