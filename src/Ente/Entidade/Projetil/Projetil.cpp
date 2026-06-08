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
        velocidade(20.f, 20.f),
        alcanceMaximo(900.0f),
        distanciaPercorrida(0.0f),
        tempoVida(3.0f),
        perfurante(false),
        doJogador(true)
    {
        setTipo(ENTIDADE_PROJETIL);

        std::string arquivosprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/projetilinimigo.png");
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
    }

    Projetil::~Projetil() {}

    void Projetil::mover() {
        sf::Vector2f posicao = getPosicao();
        posicao.x += velocidade.x;
        posicao.y += velocidade.y;
        setPosicao(posicao);
        registrarDeslocamento((velocidade.x < 0.0f ? -velocidade.x : velocidade.x) +
            (velocidade.y < 0.0f ? -velocidade.y : velocidade.y));
    }

    void Projetil::atualizar() {
        mover();
        if (tempoVida > 0.0f)
            tempoVida -= 1.0f / 60.0f;
        if (expirou())
            setAtivo(false);
    }

    void Projetil::salvar() {
        salvarDataBuffer();
    }

    void Projetil::executar() {
    }

    bool Projetil::expirou() const {
        return tempoVida <= 0.0f || distanciaPercorrida >= alcanceMaximo;
    }

    void Projetil::registrarDeslocamento(float delta) {
        if (delta > 0.0f)
            distanciaPercorrida += delta;
    }

    void Projetil::interagir_Colisao(Personagens::Inimigo* I) {
        if (!I)
            return;
        if (doJogador) {
            I->receberDano(dano);
            if (!perfurante)
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
        if (!J)
            return;
        if (!doJogador) {
            J->receberDano(dano);
            if (!perfurante) { setAtivo(false); }
        }
    }
}
