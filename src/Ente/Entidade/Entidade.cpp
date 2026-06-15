//
// Created by Henrique on 05/05/2026.
//

#include "Entidade.h"

namespace Entidades {

    void Entidade::salvarDataBuffer() {}
    void Entidade::desenhar() {
        getSprite().setPosition(getPosicao());
        gerenciadorGrafico->getJanela().draw(getSprite());

        sf::RectangleShape hitbox;
        hitbox.setSize(sf::Vector2f(getTamanho().width, getTamanho().height));
        hitbox.setPosition(getPosicao());

        hitbox.setFillColor(sf::Color::Transparent); // O interior fica invisível
        hitbox.setOutlineColor(sf::Color::Red);      // A linha de contorno fica vermelha
        hitbox.setOutlineThickness(2.0f);            // Espessura da linha
        hitbox.setOrigin(getTamanho().width / 2.f, getTamanho().height / 2.f);

        gerenciadorGrafico->getJanela().draw(hitbox);
    }

    Listas::ListaEntidades* Entidade::listaEntidades = NULL;
    Entidade::Entidade(const std::string &n) : 
        Ente(), 
        sprite(), 
        textura(), 
        colisao(false),                           
        nome(n), 
        ativo(true), 
        tipo(),                                  
        gerenciadorColisao(&Gerenciadores::Gerenciador_Colisao::getGerenciador()),
        gerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade::getGerenciador())
    {
        if (!n.empty())
            nome = n;

    }

    Entidade::~Entidade() {}
}
