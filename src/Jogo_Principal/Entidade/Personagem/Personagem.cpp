//
// Created by Henrique on 05/05/2026.
//

#include "Personagem.h"

namespace Personagens {
    Personagem::Personagem():
        Entidades::Entidade("Personagem", 0),
        velocidade(0.0f, 0.0f),
        aceleracao(0.0f, 0.0f),

        tempoPorFrame(0.12f),
        frameAcumulado(0.0f),
        indexFrameAtual(0),
        totalFramesAnimacao(8),
        frameWidth(32),
        frameHeight(32),

        vida(100),
        vidaMaxima(100),
        mana(100),
        manaMaxima(100),
        forca(12),
        armadura(10),
        resistenciaMagica(10),
        alcanceAtaque(125),
        velocidadeMovimento(0.01f),
        chanceCritica(0),
        vampirismo(0.0f),
        regeneracaoVida(2.0f),
        regeneracaoMana(3.0f),
        estado(static_cast<int>(ESTADO_OCIOSO)) {
        setTipo(Entidades::ENTIDADE_GENERICA);
    }

    Personagem::~Personagem() {}

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

    void Personagem::setManaMaxima(int valor) {
        if (valor <= 0)
            return;
        manaMaxima = valor;
        if (mana > manaMaxima)
            mana = manaMaxima;
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

    void Personagem::setMana(int valor) {
        if (valor < 0)
            mana = 0;
        else if (valor > manaMaxima)
            mana = manaMaxima;
        else
            mana = valor;
    }

    int Personagem::receberDanoFisico(int dano) {
        if (dano <= 0 || !estaVivo())
            return 0;
        int danoFinal = dano - armadura;
        if (danoFinal < 1)
            danoFinal = 1;
        setVida(vida - danoFinal);
        return danoFinal;
    }

    int Personagem::receberDanoMagico(int dano) {
        if (dano <= 0 || !estaVivo())
            return 0;
        int danoFinal = dano - resistenciaMagica;
        if (danoFinal < 1)
            danoFinal = 1;
        setVida(vida - danoFinal);
        return danoFinal;
    }

    int Personagem::causarDanoBasico() const {
        int bonusCritico = (chanceCritica >= 100) ? forca : 0;
        return forca + bonusCritico;
    }

    void Personagem::curar(int valor) {
        if (valor > 0)
            setVida(vida + valor);
    }

    bool Personagem::gastarMana(int custo) {
        if (custo < 0)
            return false;
        if (mana < custo)
            return false;
        mana -= custo;
        return true;
    }

    void Personagem::restaurarMana(int valor) {
        if (valor > 0)
            setMana(mana + valor);
    }

    void Personagem::regenerarAtributos(float deltaTempo) {
        if (deltaTempo <= 0.0f || !estaVivo())
            return;
        curar(static_cast<int>(regeneracaoVida * deltaTempo));
        restaurarMana(static_cast<int>(regeneracaoMana * deltaTempo));
    }

    void Personagem::moverHorizontal(float direcao) {
        velocidade.x = direcao * static_cast<float>(velocidadeMovimento);
        estado = (direcao == 0.0f) ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MOVIMENTO);
    }

    void Personagem::resetarCombate() {
        velocidade = sf::Vector2f(0.0f, 0.0f);
        aceleracao = sf::Vector2f(0.0f, 0.0f);
        estado = estaVivo() ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MORTO);
    }
}
