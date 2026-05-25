//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Grafico.h"

namespace Gerenciadores {
        gerenciador_grafico::gerenciador_grafico() {
            const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
            RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Default);
            window.setFramerateLimit(60);
            sprite.setPosition(0, 0);
        }
        gerenciador_grafico& gerenciador_grafico::get_grafico() {
            static gerenciador_grafico instancia;
            return instancia;
        }

        gerenciador_grafico::~gerenciador_grafico() {window.close();}
        RenderWindow& gerenciador_grafico::get_window() {return window;}
        bool gerenciador_grafico::isOpen() const {return window.isOpen();}
        void gerenciador_grafico::draw(const RectangleShape& retangulo) {window.draw(retangulo);}
        void gerenciador_grafico::draw() {window.draw(sprite);}
        void gerenciador_grafico::show() {window.display();}
        void gerenciador_grafico::clear() {window.clear();}
        void gerenciador_grafico::close() {window.close();}

} // Gerenciador