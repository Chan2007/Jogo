//
// Created by Henrique on 05/05/2026.
//

#include "Personagem.h"

namespace Personagens {

    Personagem::Personagem():
        Entidade("Personagem"),
        velocidade(0.0f, 0.0f),
        aceleracao(0.0f, 0.0f),

        caminhoArquivoSprite(""),
        tempoPorFrame(0.06f),
        tempoAcumulado(0),
        indexFrameAtual(0),
        totalFramesAnimacao(1),
        colunasSpritesheet(1),
        linhasSpritesheet(1),
        tamanhoHitbox(sf::Vector2f(0,0)),
        deslocamentoHitbox(sf::Vector2f(0,0)),
        vida(100),
        vidaMaxima(100),
        ataque(25),
        alcanceAtaque(100),
        chanceCritica(0),
        regeneracaoVida(1.0f),
        estado(0),
        velocidadeMax(200.0f),
        ObserverColisao(NULL)
    {}

    Personagem::~Personagem() {}

    sf::FloatRect Personagem::getSize() const {
        const sf::FloatRect caixaImagem = getSprite().getGlobalBounds();

        return sf::FloatRect(
            caixaImagem.left + (caixaImagem.width / 2.0f) - (tamanhoHitbox.x / 2.0f) + deslocamentoHitbox.x,
            caixaImagem.top + caixaImagem.height - tamanhoHitbox.y + deslocamentoHitbox.y,
            tamanhoHitbox.x,
            tamanhoHitbox.y
        );
    }


    float Personagem::getVidaPercentual() const {
        if (vidaMaxima <= 0)
            return 0.0f;
        return static_cast<float>(vida) / static_cast<float>(vidaMaxima) * 100.0f;
    }

    void Personagem::setVidaMaxima(const int valor) {
        if (valor <= 0)
            return;
        vidaMaxima = valor;
        if (vida > vidaMaxima)
            vida = vidaMaxima;
    }

    void Personagem::setVida(const int valor) {
        if (valor < 0)
            vida = 0;
        else if (valor > vidaMaxima)
            vida = vidaMaxima;
        else
            vida = valor;
    }

    int Personagem::receberDano(const int dano) {
        if (dano <= 0 || !estaVivo())
            return 0;
        setVida(vida - dano);
        return dano;
    }


    int Personagem::causarDano() const {
        return rand() % 101 >= chanceCritica ? ataque * (1 + chanceCritica) : ataque;

    }

    void Personagem::curar(const int valor) {
        if (valor > 0)
            setVida(vida + valor);
    }

    void Personagem::regenerarVida(const float dt) {
        if (dt <= 0.0f || !estaVivo())
            return;
        curar(static_cast<int>(regeneracaoVida * dt));    }

    void Personagem::moverHorizontal(const float direcao) {
        velocidade.x = direcao * velocidadeMax;
        estado = (direcao == 0.0f) ? static_cast<int>(PARADO) : static_cast<int>(ANDANDO);
    }

    void Personagem::parar() {
        velocidade = sf::Vector2f(0.0f, 0.0f);
        aceleracao = sf::Vector2f(0.0f, 0.0f);
    }
}