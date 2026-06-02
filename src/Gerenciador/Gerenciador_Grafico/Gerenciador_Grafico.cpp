//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Grafico.h"

namespace Gerenciadores {
        Gerenciador_Grafico::Gerenciador_Grafico() {
            const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
            RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Default);
            window.setFramerateLimit(60);
            sprite.setPosition(0, 0);
        }
        Gerenciador_Grafico& Gerenciador_Grafico::get_grafico() {
            static Gerenciador_Grafico instancia;
            return instancia;
        }

        Gerenciador_Grafico::~Gerenciador_Grafico() {window.close();}
        RenderWindow& Gerenciador_Grafico::get_window() {return window;}
        bool Gerenciador_Grafico::isOpen() const {return window.isOpen();}
        void Gerenciador_Grafico::draw(const RectangleShape& retangulo) {window.draw(retangulo);}
        void Gerenciador_Grafico::draw() {window.draw(sprite);}
        void Gerenciador_Grafico::show() {window.display();}
        void Gerenciador_Grafico::clear() {window.clear();}
        void Gerenciador_Grafico::close() {window.close();}

} // Gerenciador