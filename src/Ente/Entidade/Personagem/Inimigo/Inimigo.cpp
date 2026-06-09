//
// Created by Henrique on 05/05/2026.
//

#include "Inimigo.h"

#include <cmath>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"

std::vector<Personagens::Jogador*> Personagens::Inimigo::listaJogadores;

namespace Personagens {

    Inimigo::Inimigo() :
        Personagem(),
        ataque(45),
        alcancePerseguicao(700.0f),
        elite(false),
        direcaoPatrulha(1.f),
        deslocamentoPatrulha(0.f),
        limiteDeslocamento(120.f)
    {
        setNome("Inimigo");
        setTipo(Entidades::ENTIDADE_INIMIGO);
        setVidaMaxima(450);
        setVida(450);
        setPoder(45);
        setAlcanceAtaque(175);
    }

    Inimigo::~Inimigo() {}

    bool Inimigo::estaEmAlcance(const sf::Vector2f& alvo) const {
        sf::Vector2f origem = getPosicao();
        float dx = alvo.x - origem.x;
        float dy = alvo.y - origem.y;
        return std::sqrt(dx * dx + dy * dy) <= alcancePerseguicao;
    }

    void Inimigo::inverterPatrulha() {
        direcaoPatrulha *= -1.f;
        deslocamentoPatrulha = 0.f;
    }

    void Inimigo::interagir_Colisao(Inimigo* I) {
        if (I && I != this)
            setColisao(true);
    }

    void Inimigo::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (!O) return;
        setColisao(true);

        sf::FloatRect hitboxInimigo = getSprite().getGlobalBounds();
        sf::FloatRect hitboxObs = O->getSprite().getGlobalBounds();

        float centroYInimigo = hitboxInimigo.top + (hitboxInimigo.height / 2.f);
        float centroXInimigo = hitboxInimigo.left + (hitboxInimigo.width / 2.f);

        bool bateuNaParede = (centroYInimigo > hitboxObs.top) && (centroYInimigo < hitboxObs.top + hitboxObs.height);

        if (bateuNaParede) {
            inverterPatrulha();
        }

        else {
            float margem = 5.0f;

            if (direcaoPatrulha > 0.0f) {
                //Checa se o centro passou da borda direita da plataforma
                if (centroXInimigo >= (hitboxObs.left + hitboxObs.width) - margem) {
                    inverterPatrulha();
                }
            }
            else if (direcaoPatrulha < 0.0f) {
                // Checa se o centro passou da borda esquerda da plataforma
                if (centroXInimigo <= hitboxObs.left + margem) {
                    inverterPatrulha();
                }
            }
        }
    }

    void Inimigo::interagir_Colisao(Entidades::Projetil* P) {
    }

    void Inimigo::interagir_Colisao(Jogador* J) {
        if (J)
            J->receberDano(ataque);
    }
}
