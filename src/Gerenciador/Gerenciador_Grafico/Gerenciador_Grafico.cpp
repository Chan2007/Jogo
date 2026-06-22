//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Grafico.h"

namespace Gerenciadores {
    Gerenciador_Grafico::Gerenciador_Grafico():
        sprite(), tamanho(),
        window(), gerenciadorTextura(new Gerenciador_Textura()),
        animador(new Animador())
    {
        window.setFramerateLimit(60);
    }
    Gerenciador_Grafico& Gerenciador_Grafico::getGerenciador() {
        static Gerenciador_Grafico instancia;
        return instancia;
    }
    Gerenciador_Grafico::~Gerenciador_Grafico() {
        window.close();
        delete gerenciadorTextura;
        gerenciadorTextura = NULL;
        delete animador;
        animador = NULL;
    }

    void Gerenciador_Grafico::setSize(const sf::VideoMode size) {
        tamanho.width = size.width;
        tamanho.height = size.height;
    }
    void Gerenciador_Grafico::setPosition(const sf::Vector2f pos) {
        position = pos;
    }
    sf::Vector2i Gerenciador_Grafico::getMousePosition() const {
        return sf::Mouse::getPosition(window);
    }
    void Gerenciador_Grafico::draw() {
        window.draw(sprite);
    }
    void Gerenciador_Grafico::draw(const sf::Drawable& drawable) {
        window.draw(drawable);
    }
    bool Gerenciador_Grafico::isOpen() const {
        return window.isOpen();
    }
    void Gerenciador_Grafico::show() {
        window.display();
    }
    void Gerenciador_Grafico::clear() {
        window.clear();
    }
    void Gerenciador_Grafico::close() {
        window.close();
    }
    bool Gerenciador_Grafico::pollEvent(sf::Event& evento) {
        return window.pollEvent(evento);
    }
    sf::VideoMode Gerenciador_Grafico::getSize() {
        return tamanho;
    }
    void Gerenciador_Grafico::loadAnimation(const std::string &prefix, const std::string &name,
                                            const int numFrames, const int frameStep,
                                            const unsigned int cols, const unsigned int rows) {
        if (!window.isOpen()) return;
        animador->setSheetTargetSize(sf::Vector2u(tamanho.width, tamanho.height));
        animador->setSheetPosition(position);
        animador->loadFrames(prefix, name, numFrames, frameStep, cols, rows);
    }

    void Gerenciador_Grafico::updateAnimation() {
        if (animador) animador->update();
    }

    void Gerenciador_Grafico::drawAnimation() {
        if (animador) animador->draw(window);
    }
    void Gerenciador_Grafico::create(sf::VideoMode mode, std::string title, sf::Uint32 style) {
        window.create(mode, title, style);
    }
    void Gerenciador_Grafico::setFramerateLimit(int framerateLimit) {
        window.setFramerateLimit(framerateLimit);
    }
    void Gerenciador_Grafico::setView(const sf::View& visao) {
        window.setView(visao);
    }
    sf::View Gerenciador_Grafico::getView() const {
        return window.getView();
    }
    sf::View Gerenciador_Grafico::getDefaultView() const {
        return window.getDefaultView();
    }
    sf::Texture* Gerenciador_Grafico::loadTexture(const std::string& caminho) {
        if (!gerenciadorTextura) return NULL;

        // Padrão Facade
        gerenciadorTextura->carregarTextura(caminho);
        return gerenciadorTextura->buscarTextura(caminho);
    }

    void Gerenciador_Grafico::updateAnimationSprite(sf::Sprite& Sprite, sf::IntRect& rectAtual,
                                              int numFrames, float dt,
                                              float& tempoAcumulado, int& indexFrameAtual,
                                              unsigned int cols, unsigned int rows, float tempoPorFrame)
    {
        // Facade delegando para o Animador
        animador->atualizarSpriteEntidade(Sprite, rectAtual, numFrames, dt, tempoAcumulado, indexFrameAtual, cols, rows, tempoPorFrame);
    }

} // Gerenciador