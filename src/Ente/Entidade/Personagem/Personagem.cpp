
#include <iostream>

#include "Personagem.h"

namespace Personagens {

    Personagem::Personagem() :
        Entidades::Entidade("Personagem"),
        velocidade(0.0f, 0.0f),
        aceleracao(0.0f, 0.0f),

        caminhoArquivoSprite(""),
        caminhoArquivoSpritePulo(""),
        tempoPorFrame(0.06f),
        frameAcumulado(0.0f),
        indexFrameAtual(0),
        totalFramesAnimacao(0),
        colunasSpritesheet(0),
        frameWidth(0),
        frameHeight(0),

        velocidadeMax(0.f),
        vida(100),
        vidaMaxima(100),
        poder(12),
        alcanceAtaque(125),
        chanceCritica(0),
        regeneracaoVida(2.0f),
        estado(static_cast<int>(ESTADO_OCIOSO)),
        invulneravel(false),
        tempoDano(0)
    {
    }

    Personagem::~Personagem() {}

    sf::FloatRect Personagem::getTamanho() const {

        sf::FloatRect caixaImagem = getSprite().getGlobalBounds();

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
        if (dano <= 0 || !estaVivo()) return 0;
        setVida(vida - dano);
        std::cout << getNome() << " recebeu " << dano << " de dano. Vida: " << vida << std::endl;
        getSprite().setColor(sf::Color(255, 80, 80, 255));
        tempoDano = 0.5f;
        clockDano.restart();
        return dano;
    }


    int Personagem::causarDanoBasico() const {
        return rand() % 101 < chanceCritica ? poder * (1 + chanceCritica) : poder;
    }

    void Personagem::curar(int valor) {
        if (valor > 0) vida += valor;
    }

    void Personagem::regenerarVida(float dt) {
        if (dt <= 0.0f || !estaVivo())
            return;
        curar(static_cast<int>(regeneracaoVida * dt));
    }

    void Personagem::moverHorizontal(float direcao) {
        sf::Vector2f velAtual = getVelocidade();
        velAtual.x = direcao * velocidadeMax;
        setVelocidade(velAtual);
        estado = (direcao == 0.0f) ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MOVIMENTO);
    }

    void Personagem::resetarCombate() {
        velocidade = sf::Vector2f(0.0f, 0.0f);
        aceleracao = sf::Vector2f(0.0f, 0.0f);
        estado = estaVivo() ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MORTO);
    }
}
