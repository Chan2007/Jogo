//
// Created by Henrique on 05/05/2026.
//

#include "Personagem.h"

namespace Personagens {

    Personagem::Personagem():
        Entidades::Entidade("Personagem", 0),
        velocidade(0.0f, 0.0f),
        aceleracao(0.0f, 0.0f),

        caminhoArquivoSprite(""),
        caminhoArquivoSpritePulo(""),
        tempoPorFrame(0.12f),
        frameAcumulado(0.0f),
        indexFrameAtual(0),
        totalFramesAnimacao(8),
        colunasSpritesheet(1),
        frameWidth(32),
        frameHeight(32),

        vida(100),
        vidaMaxima(100),
        forca(12),
        alcanceAtaque(125),
        chanceCritica(0),
        regeneracaoVida(2.0f),
        estado(static_cast<int>(ESTADO_OCIOSO))
    {
        setTipo(Entidades::ENTIDADE_GENERICA);
    }

    Personagem::~Personagem() {}

    sf::FloatRect Personagem::getTamanho() const {

        sf::FloatRect caixaImagem = getCorpo().getGlobalBounds();

        float larguraHitbox = 70.f;
        float alturaHitbox = 90.f;

        float deslocamentoX = 12.0f;
        float deslocamentoY = -17.5f;

        return sf::FloatRect(
            caixaImagem.left + (caixaImagem.width / 2.0f) - (larguraHitbox / 2.0f) + deslocamentoX,
            caixaImagem.top + caixaImagem.height - alturaHitbox + deslocamentoY,
            larguraHitbox,
            alturaHitbox
        );
    }


    void Personagem::desenhar(sf::RenderWindow& window) {
        getCorpo().setPosition(getPosicao());
        window.draw(getCorpo());
    }

    void Personagem::salvarDataBuffer() {
        Entidades::Entidade::salvarDataBuffer();
    }

    float Personagem::getVidaPercentual() const {
        if (vidaMaxima <= 0)
            return 0.0f;
        return (static_cast<float>(vida) / static_cast<float>(vidaMaxima)) * 100.0f;
    }

    void Personagem::setVidaMaxima(int valor) {
        if (valor <= 0)
            return;
        vidaMaxima = valor;
        if (vida > vidaMaxima)
            vida = vidaMaxima;
    }

    void Personagem::setVida(int valor) {
        if (valor < 0)
            vida = 0;
        else if (valor > vidaMaxima)
            vida = vidaMaxima;
        else
            vida = valor;
        if (vida <= 0)
            estado = static_cast<int>(ESTADO_MORTO);
    }

    int Personagem::receberDano(int dano) {
        if (dano <= 0 || !estaVivo())
            return 0;
        setVida(vida - dano);
        return dano;
    }


    int Personagem::causarDanoBasico() const {
        return (rand()%101 >= chanceCritica) ? (forca * (1 + chanceCritica)) : forca;

    }

    void Personagem::curar(int valor) {
        if (valor > 0)
            setVida(vida + valor);
    }

    void Personagem::regenerarVida(float deltaTempo) {
        if (deltaTempo <= 0.0f || !estaVivo())
            return;
        curar(static_cast<int>(regeneracaoVida * deltaTempo));    }

    void Personagem::moverHorizontal(float direcao) {
        velocidade.x = direcao * velocidade.x;
        estado = (direcao == 0.0f) ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MOVIMENTO);
    }

    void Personagem::resetarCombate() {
        velocidade = sf::Vector2f(0.0f, 0.0f);
        aceleracao = sf::Vector2f(0.0f, 0.0f);
        estado = estaVivo() ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MORTO);
    }
}