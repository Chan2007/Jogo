//
// Created by Henrique on 05/05/2026.
//

#include "Projetil.h"

#include "Jogo_Principal/Entidade/Obstaculo/Obstaculo.h"
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Jogo_Principal/Entidade/Personagem/Jogador/Jogador.h"

namespace Entidades {
    Projetil::Projetil():
        Entidade("Projetil", 0),
        dano(80),
        velocidade(0.0f, 0.0f),
        alcanceMaximo(900.0f),
        distanciaPercorrida(0.0f),
        tempoVida(3.0f),
        perfurante(false) {
        setTipo(ENTIDADE_PROJETIL);
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

    void Projetil::desenhar(sf::RenderWindow& window) {
        window.draw(getCorpo());
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
        I->absorverDano(dano);
        if (!perfurante)
            setAtivo(false);
    }

    void Projetil::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O)
            setAtivo(false);
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
        J->absorverDano(dano);
        if (!perfurante)
            setAtivo(false);
    }
}
