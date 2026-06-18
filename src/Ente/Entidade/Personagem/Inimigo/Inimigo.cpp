

#include "Inimigo.h"

#include <cmath>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Sistema/Fisica/Visitor_Colisao.h"

std::vector<Personagens::Jogador*> Personagens::Inimigo::listaJogadores;

namespace Personagens {

    Inimigo::Inimigo() :
        Personagem(),
        ataque(45),
        alcancePerseguicao(700.0f),
        elite(false),
        direcaoPatrulha(1.f),
        deslocamentoPatrulha(0.f),
        limiteDeslocamento(120.f),
        interagindo(false)
    {
        setNome("Inimigo");
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
        mover();
    }

    void Inimigo::mover() {
        direcaoPatrulha *= -1.f;
        deslocamentoPatrulha = 0.f;

        float escalaX = std::abs(getSprite().getScale().x);
        float escalaY = getSprite().getScale().y;

        // direcaoPatrulha > 0 = indo para direita = inverte X
        if (direcaoPatrulha > 0.f)
            getSprite().setScale(escalaX, escalaY);
        else
            getSprite().setScale(-escalaX, escalaY);
    }

    void Inimigo::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
}
