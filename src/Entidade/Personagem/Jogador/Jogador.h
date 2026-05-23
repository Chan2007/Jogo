//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include "Sistema/Input/Observador_Input.h"
#include "Entidade/Personagem/Personagem.h"

namespace Obstaculos {
    class Plataforma;
}

namespace Personagens {
    class Inimigo;

    class Jogador: public Personagem {
    private:
        Gerenciadores::Observador_Input* observer_jogador;
    protected:
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
        bool colidir(Inimigo* I);
        void atualizar();
        void salvar();
        void mover();
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
        void aoColidir(Entidades::Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
} // Personagens

#endif //JOGO_JOGADOR_H
