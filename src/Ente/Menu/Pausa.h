//
// Created by Henrique on 16/06/2026.
//

#ifndef JOGO_PAUSA_H
#define JOGO_PAUSA_H
#define NOMINMAX
#include <string>
#include <vector>

#include "Ente/Ente.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

class Pausa: public Ente {
    private:
        sf::Sprite spriteFundo;
        sf::Texture texturaFundo;

        sf::Font fonte;
        std::vector<sf::Text> botoes;
        std::vector<std::string> textosBotoes;
        int indiceSelecionado;

        void inicializarBotoes();


    public:
        Pausa();
        ~Pausa();

        void ajustarPosicoes();
        void atualizarItemSelecionado(int direcao); // -1 para cima, 1 para baixo
        int cliqueMouse();
        int getIndiceSelecionado() const { return indiceSelecionado; }

        void desenhar();
};

#endif //JOGO_PAUSA_H