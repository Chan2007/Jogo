//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_OBSERVADOR_JOGADOR_H
#define JOGO_OBSERVADOR_JOGADOR_H
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

class Jogador;
class Observador_Jogador {
    private:
        Jogador* jogador;
    protected:
        bool ativo;

    public:
        Observador_Jogador();
        ~Observador_Jogador();
        void setAtivo(bool a);
        const bool getAtivo() const;
        void setJogador(Jogador* jog);
        void verificaTeclaPressionada(sf::Keyboard::Key tecla);
        void verificaTeclaSolta(sf::Keyboard::Key tecla);
        void verificaMousePressionado(sf::Mouse::Button butao);
        void verificaMouseSolto(sf::Mouse::Button butao);
        void verificaMouseEmCima(sf::Vector2f pos);
};


#endif //JOGO_OBSERVADOR_JOGADOR_H