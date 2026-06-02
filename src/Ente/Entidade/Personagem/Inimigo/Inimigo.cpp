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
        ataque(45),
        escudo(0),
        ouroConcedido(120),
        experienciaConcedida(90),
        alcancePerseguicao(700.0f),
        elite(false) {
        setNome("Inimigo");
        setTipo(Entidades::ENTIDADE_INIMIGO);
        setVidaMaxima(450);
        setVida(450);
        setManaMaxima(1);
        setMana(0);
        setForca(45);
        setArmadura(18);
        setResistenciaMagica(18);
        setVelocidadeMovimento(325);
        setAlcanceAtaque(175);
    }

    Inimigo::~Inimigo() {}

    void Inimigo::salvarDataBuffer() {
        Personagem::salvarDataBuffer();
    }

    void Inimigo::desenhar(sf::RenderWindow& window) {
        window.draw(getCorpo());
    }

    bool Inimigo::estaEmAlcance(const sf::Vector2f& alvo) const {
        sf::Vector2f origem = getPosicao();
        float dx = alvo.x - origem.x;
        float dy = alvo.y - origem.y;
        return std::sqrt(dx * dx + dy * dy) <= alcancePerseguicao;
    }

    int Inimigo::absorverDano(int dano) {
        if (dano <= 0)
            return 0;
        if (escudo > 0) {
            int restanteEscudo = escudo - dano;
            if (restanteEscudo >= 0) {
                escudo = restanteEscudo;
                return 0;
            }
            dano = -restanteEscudo;
            escudo = 0;
        }
        return receberDanoFisico(dano);
    }

    void Inimigo::concederRecompensa(Jogador* J) {
        if (!J)
            return;
        J->ganharOuro(ouroConcedido);
        J->ganharExperiencia(experienciaConcedida);
        J->adicionarPontos(static_cast<float>(ouroConcedido + experienciaConcedida));
    }

    void Inimigo::interagir_Colisao(Inimigo* I) {
        if (I && I != this)
            setColisao(true);
    }

    void Inimigo::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O)
            setColisao(true);
    }

    void Inimigo::interagir_Colisao(Entidades::Projetil* P) {
        if (P)
            absorverDano(P->getDano());
    }

    void Inimigo::interagir_Colisao(Jogador* J) {
        if (J)
            J->absorverDano(ataque);
    }
}
