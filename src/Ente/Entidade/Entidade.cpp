//
// Created by Henrique on 05/05/2026.
//

#include "Entidade.h"

namespace Entidades {

    void Entidade::salvarDataBuffer() {}
    void Entidade::desenhar(sf::RenderWindow& window) {
        getSprite().setPosition(getPosicao());
        window.draw(getSprite());
    }

    Listas::ListaEntidades* Entidade::listaEntidades = NULL;
    Entidade::Entidade(const std::string &n) : Ente(), sprite(), textura(), colisao(false),
                                               nome(n), ativo(true), tipo(),
                                               gerenciadorColisao(Gerenciadores::Gerenciador_Colisao::getGerenciador()),
                                               gerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade::getGerenciador())
    {
        if (!n.empty())
            nome = n;

    }

    Entidade::~Entidade() {}
}
