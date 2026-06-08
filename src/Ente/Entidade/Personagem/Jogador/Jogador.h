//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include "Sistema/Input/Observador_Input.h"
#include "Ente/Entidade/Personagem/Personagem.h"

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
        CAMPEAO_NAAFIRI,
        CAMPEAO_JHIN,
        CAMPEAO_LUX,
        CAMPEAO_EVELYNN,
        CAMPEAO_GWEN,
        CAMPEAO_PYKE,
        CAMPEAO_SETT,
        CAMPEAO_SHACO,
        CAMPEAO_VIEGO
    };

    class Jogador: public Personagem {
        private:
            Gerenciadores::Observador_Input* ObserverInput;
            void atualizar(float dt);
        protected:
            float pontos;
            int abates;
        public:
            Jogador();
            ~Jogador();

            void executar(float dt);
            void salvar();
            void mover(float dt);

            void setCampeao(EscolhaCampeao campeao);
            Gerenciadores::Observador_Input* getObserver();
            float getPontos() const {return pontos;}
            int getAbates() const {return abates;}
            void adicionarPontos(float valor);
            void registrarAbate();
            void aoColidir(Entidade* E) {
                E->interagir_Colisao(this);
            }
            void interagir_Colisao(Inimigo* I);
            void interagir_Colisao(Obstaculos::Obstaculo* O);
            void interagir_Colisao(Entidades::Projetil* P);
            void interagir_Colisao(Jogador* J);
        };
} // Personagens

#endif //JOGO_JOGADOR_H