//
// Created by Henrique on 05/05/2026.
//

#include "Projetil.h"

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include <iostream>

#include "Sistema/Fisica/Visitor_Colisao.h"

namespace Entidades {
    Projetil::Projetil() :
        Entidade("Projetil"),
        dano(35),
        velocidade(140.f, 140.f),
        doJogador(false)
    {
        setVelocidade(velocidade);
        setPosicao(sf::Vector2f(0.f, 1080.f));

        std::string arquivosprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/projetilinimigo2.png");
        if (!arquivosprite.empty()) {
            if (getTextura().loadFromFile(arquivosprite)) {
                getSprite().setTexture(getTextura());
                getSprite().setTextureRect(sf::IntRect(0, 0, 96, 91));
            }
            else {
                std::cerr << "Erro: não foi possível carregar a imagem do projétil em: " << arquivosprite << std::endl;
            }
        }
        getSprite().setOrigin(96.f / 2.f, 91.f / 2.f);
        getSprite().setScale(0.15f, 0.15f);
    }

    Projetil::~Projetil() {}

    void Projetil::mover() {
        if (getVigente()) {
            const float dt = 0.016f;
            sf::Vector2f posicao = getPosicao();
            posicao.x += velocidade.x * (dt + 0.02);
            posicao.y += velocidade.y * dt;
            setPosicao(posicao);
        }
    }

    void Projetil::atualizar() {
        mover();
    }

    void Projetil::salvar() {
        salvarDataBuffer();
    }

    void Projetil::executar() {
        atualizar();
    }

    sf::FloatRect Projetil::getTamanho() const {
        sf::FloatRect caixaImagem = getSprite().getGlobalBounds();
        float largura = 90.f;
        float altura = 70.f;
        return sf::FloatRect(
            caixaImagem.left + (caixaImagem.width / 2.f) - (largura / 2.f),
            caixaImagem.top + (caixaImagem.height / 2.f) - (altura / 2.f),
            largura,
            altura
        );
    }
    void Projetil::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
}
