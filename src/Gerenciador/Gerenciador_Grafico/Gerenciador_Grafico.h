//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_GRAFICO_H
#define JOGO_GERENCIADOR_GRAFICO_H
#include <SFML/Graphics.hpp>

namespace Gerenciadores {
    class Gerenciador_Grafico {
        private:
            sf::Sprite sprite;
            sf::RenderWindow window;
            sf::VideoMode tamanho;
            Gerenciador_Grafico();
            // Proibir o uso de construtora de cópia e atribuição à cópias
            Gerenciador_Grafico(const Gerenciador_Grafico&);
            Gerenciador_Grafico& operator=(const Gerenciador_Grafico&);
        public:
            static Gerenciador_Grafico& get_grafico();

            ~Gerenciador_Grafico() {window.close();}
            sf::RenderWindow& get_window() {return window;}
            bool Gerenciador_Grafico::isOpen() const {return window.isOpen();}
            void Gerenciador_Grafico::setSize(sf::Vector2i size) {tamanho.width = size.x; tamanho.height = size.y;}
            void Gerenciador_Grafico::draw(const sf::RectangleShape& retangulo) {window.draw(retangulo);}
            void Gerenciador_Grafico::draw() {window.draw(sprite);}
            void Gerenciador_Grafico::show() {window.display();}
            void Gerenciador_Grafico::clear() {window.clear();}
            void Gerenciador_Grafico::close() {window.close();}

    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_GRAFICO_H