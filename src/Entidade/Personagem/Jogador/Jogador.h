//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include <iostream>
#include "Sistema/Input/Observador_Input.h"
#include "Entidade/Personagem/Personagem.h"

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

    class Jogador: public Personagem {
    private:
        Gerenciadores::Observador_Input* observer_jogador;
        Gerenciadores::Gerenciador_Gravidade* pGravidade;
    protected:
        std::string caminhoArquivoSprite;
        float sorte;
        float pontos;
        float escudo;
        int ouro;
        int experiencia;
        int nivelInvocador;
        int abates;
        int mortes;
        int assistencias;
        int cargasUltimate;
    public:
        Jogador();
        ~Jogador();
        void setGerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade* g);
        bool colidir(Inimigo* I);
        void atualizar();
        void salvar();
        void mover();
        void setCampeao(EscolhaCampeao campeao);
        void desenhar(sf::RenderWindow& window);
        Gerenciadores::Observador_Input* get_observer_jogador();
        float getSorte() const {return sorte;}
        float getPontos() const {return pontos;}
        float getEscudo() const {return escudo;}
        int getOuro() const {return ouro;}
        int getExperiencia() const {return experiencia;}
        int getNivelInvocador() const {return nivelInvocador;}
        int getAbates() const {return abates;}
        int getMortes() const {return mortes;}
        int getAssistencias() const {return assistencias;}
        int getCargasUltimate() const {return cargasUltimate;}
        void setSorte(float valor) {sorte = (valor < 0.0f) ? 0.0f : valor;}
        void setEscudo(float valor) {escudo = (valor < 0.0f) ? 0.0f : valor;}
        void adicionarPontos(float valor);
        void ganharOuro(int valor);
        void ganharExperiencia(int valor);
        void registrarAbate();
        void registrarMorte();
        void registrarAssistencia();
        void adicionarCargaUltimate();
        bool podeUsarUltimate() const;
        bool consumirCargaUltimate();
        int absorverDano(int dano);
        void aoColidir(Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
} // Personagens

#endif //JOGO_JOGADOR_H