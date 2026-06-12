//
// Created by Henrique on 05/05/2026.
//

#include "Projetil.h"

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include <iostream>

namespace Entidades {
    Projetil::Projetil() :
        Entidade("Projetil"),
        dano(35),
        velocidade(2000.f, 1400.f),
        doJogador(false)
    {
        setTipo(ENTIDADE_PROJETIL);
        setVelocidade(velocidade);
        setPosicao(sf::Vector2f(0.f, 0.f));

        std::string arquivosprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/projetilinimigo2.png");
        if (!arquivosprite.empty()) {
            if (getTextura().loadFromFile(arquivosprite)) {
                getSprite().setTexture(getTextura());
                getSprite().setTextureRect(sf::IntRect(0, 0, 96, 91));
            }
            else {
                std::cerr << "Erro: não foi possivel carregar a spritesheet do inimigo facil em: " << arquivosprite << std::endl;
            }
        }
        getSprite().setOrigin(96.f / 2.f, 91.f / 2.f);
        getSprite().setScale(0.4f, 0.4f);
    }

    Projetil::~Projetil() {}

    void Projetil::mover() {
        const float dt = 0.016f;
        sf::Vector2f posicao = getPosicao();
        posicao.x += velocidade.x * dt;
        posicao.y += velocidade.y * dt;
        setPosicao(posicao);
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

    void Projetil::interagir_Colisao(Personagens::Inimigo* I) {
        if (!I || !getAtivo()) return;
        if (doJogador) {
            I->receberDano(dano);
            setAtivo(false);
        }
    }

    void Projetil::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O) { setAtivo(false); }
    }

    void Projetil::interagir_Colisao(Projetil* P) {
        if (P && P != this) {
            setAtivo(false);
            P->setAtivo(false);
        }
    }

    void Projetil::interagir_Colisao(Personagens::Jogador* J) {
        if (!J || !getAtivo()) return;

        // Verifica se o tiro é inimigo para causar dano
        if (!doJogador) {
            J->receberDano(dano);
            std::cout << "Jogador foi atingido por um projétil." << std::endl;
            setAtivo(false);
        }
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
}
