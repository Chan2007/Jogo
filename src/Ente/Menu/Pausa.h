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
#include "Sistema/Input/Observador_Input.h"

class Pausa: public Ente, public Gerenciadores::Observador_Input {
    private:
        sf::Sprite spriteFundo;
        sf::Texture texturaFundo;

        sf::Font Fonte;
        std::vector<sf::Text> Botoes;
        std::vector<std::string> textosBotoes;
        int index;

        void initBotoes();


    public:
        Pausa();
        ~Pausa();

        void redimensionarTela();
        void atualizarItem(int direcao); // -1 para cima, 1 para baixo
        virtual void executar() {}
        int clicado();
        int getIndex() const { return index; }
        void aoApertarTecla(const Gerenciadores::Tecla& evento);

        void desenhar();
};

#endif //JOGO_PAUSA_H