//
// Created by Henrique on 05/05/2026.
//

#include "Inimigo.h"

#include <cmath>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Personagens {
    Inimigo::Inimigo():
        Personagem(),
        tempoUltimoAtaque(0.0f),
        cooldownAtaque(),
        pontosConcedidos(),
        alcancePerseguicao(),
        elite(false)
    {
        setNome("Inimigo");
        setTipo(Entidades::ENTIDADE_INIMIGO);
    }

    Inimigo::~Inimigo() {}

    bool Inimigo::devePerseguir(const sf::Vector2f& alvo) const {
        const sf::Vector2f origem = getPosicao();
        const float dx = alvo.x - origem.x;
        const float dy = alvo.y - origem.y;
        return std::sqrt(dx * dx + dy * dy) <= getAlcancePerseguicao();
    }
    bool Inimigo::deveAtacar(const sf::Vector2f& alvo) const {
        const sf::Vector2f origem = getPosicao();
        const float dx = alvo.x - origem.x;
        const float dy = alvo.y - origem.y;
        return std::sqrt(dx * dx + dy * dy) <= getAlcanceAtaque();
    }

    int Inimigo::receberDano(const int dano) {
        if (dano <= 0) return 0;
        return receberDano(dano);
    }

    void Inimigo::concederRecompensa(Jogador* J) const {
        if (!J) return;
        J->adicionarPontos(static_cast<float>(pontosConcedidos));
    }

    void Inimigo::interagir_Colisao(Inimigo* I) {
        if (I && I != this) setColisao(true);
    }

    void Inimigo::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O) setColisao(true);
    }

    void Inimigo::interagir_Colisao(Entidades::Projetil* P) {
        if (P) receberDano(P->getDano());
    }

    void Inimigo::interagir_Colisao(Jogador* J) {
        if (J) J->receberDano(getAtaque());
    }
}
