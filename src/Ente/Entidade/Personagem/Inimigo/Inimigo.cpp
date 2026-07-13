

#include "Inimigo.h"

#include <cmath>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

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
        nome = "Inimigo";
        alcanceAtaque = 175;
        vidaMaxima = 450;
        vida = 450;
        poder = 45;
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

        float escalaX = std::abs(sprite.getScale().x);
        float escalaY = sprite.getScale().y;

        // direcaoPatrulha > 0 = indo para direita = inverte X
        if (direcaoPatrulha > 0.f)
            sprite.setScale(escalaX, escalaY);
        else
            sprite.setScale(-escalaX, escalaY);
    }

    /*
    void Inimigo::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
    */

    void Inimigo::salvarInimigo() {
        if (buffer) {
            (*buffer) << poder << ' '
                << tempoUltimoAtaque << ' '
                << direcaoPatrulha << ' '
                << deslocamentoPatrulha << ' '
                << elite << ' '
                << interagindo << ' ';
        }
    }
    Gerenciadores::Memento* Inimigo::salvarMemento() const  {
        return new InimigoMemento(*this);
    }

    void Inimigo::restaurarMemento(const Gerenciadores::Memento* memento) {
        if (!memento) return;
        Personagem::restaurarMemento(memento);

        const InimigoMemento* pMemento = dynamic_cast<const InimigoMemento*>(memento);
        if (pMemento) {
            nivelMaldade = pMemento->nivelMaldadeMemento;
            ataque = pMemento->ataqueMemento;
            tempoUltimoAtaque = pMemento->tempoUltimoAtaqueMemento;
            cooldownAtaque = pMemento->cooldownAtaqueMemento;
            alcancePerseguicao = pMemento->alcancePerseguicaoMemento;
            direcaoPatrulha = pMemento->direcaoPatrulhaMemento;
            deslocamentoPatrulha = pMemento->deslocamentoPatrulhaMemento;
            limiteDeslocamento = pMemento->limiteDeslocamentoMemento;
            elite = pMemento->eliteMemento;
            interagindo = pMemento->interagindoMemento;
        }
    }
}
