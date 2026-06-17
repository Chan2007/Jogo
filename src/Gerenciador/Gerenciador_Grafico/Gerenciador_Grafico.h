//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_GRAFICO_H
#define JOGO_GERENCIADOR_GRAFICO_H

#include <SFML/Graphics.hpp>
#include "Sistema/UI/Animador.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"

namespace Gerenciadores {
    class Gerenciador_Grafico {
        private:
            sf::Sprite sprite;
            sf::VideoMode tamanho;
            sf::Vector2f position;
            sf::RenderWindow window;

            Gerenciador_Textura* gerenciadorTextura;
            Animador animadorFundo;
            Gerenciador_Grafico();

            // Proibir o uso de construtora de cópia e atribuição à cópias
            Gerenciador_Grafico(const Gerenciador_Grafico&);
            Gerenciador_Grafico& operator=(const Gerenciador_Grafico&);
        public:
            static Gerenciador_Grafico& getGerenciador();

            ~Gerenciador_Grafico() {window.close(); delete gerenciadorTextura;}
            sf::RenderWindow& getJanela() {return window;}
            bool isOpen() const {return window.isOpen();}

            void setSize(const sf::VideoMode size) {tamanho.width = size.width; tamanho.height = size.height;}
            void setPosition(const sf::Vector2f pos) {position = pos;}
            void draw(const sf::RectangleShape& retangulo) {window.draw(retangulo);}
            void draw() {window.draw(sprite);}
            void show() {window.display();}
            void clear() {window.clear();}
            void close() {window.close();}

            // Animações (fundo, ‘sprites’, etc)
            void loadAnimation(const std::string &prefix, const std::string &name, int numFrames,
                               int frameStep, unsigned int cols, unsigned int rows);
            void updateAnimation();
            void drawAnimation();

    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_GRAFICO_H