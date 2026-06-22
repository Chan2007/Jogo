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
            Animador* animador;
            Gerenciador_Grafico();

            // Proibir o uso de construtora de cópia e atribuição à cópias
            Gerenciador_Grafico(const Gerenciador_Grafico&);
            Gerenciador_Grafico& operator=(const Gerenciador_Grafico&);
        public:
            static Gerenciador_Grafico& getGerenciador();

            ~Gerenciador_Grafico();

            sf::VideoMode getSize();
            void setSize(sf::VideoMode size);
            void setPosition(sf::Vector2f pos);
            sf::Vector2i getMousePosition() const;

            void draw();
            void draw(const sf::Drawable& drawable);

            void create(sf::VideoMode mode, std::string title, sf::Uint32 style = sf::Style::Default);
            void setFramerateLimit(int framerateLimit);
            bool isOpen() const;
            void show();
            void clear();
            void close();

            bool pollEvent(sf::Event& evento);

            void setView(const sf::View& visao);
            sf::View getView() const;
            sf::View getDefaultView() const;

            // FlyWeight
            sf::Texture* loadTexture(const std::string& caminho);

            // Animações (fundo, ‘sprites’, etc)
            void loadAnimation(const std::string &prefix, const std::string &name, int numFrames,
                               int frameStep, unsigned int cols, unsigned int rows);
            void updateAnimation();
            void drawAnimation();
            void updateAnimationSprite(sf::Sprite& Sprite, sf::IntRect& rectAtual,
                                 int numFrames, float dt,
                                 float& tempoAcumulado, int& indexFrameAtual,
                                 unsigned int cols, unsigned int rows, float tempoPorFrame);

    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_GRAFICO_H