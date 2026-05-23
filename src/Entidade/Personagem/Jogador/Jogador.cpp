//
// Created by Henrique on 05/05/2026.
//

#include "Jogador.h"

#include "Entidade/Obstaculo/Obstaculo.h"
#include "Entidade/Personagem/Inimigo/Inimigo.h"
#include "Entidade/Projetil/Projetil.h"

namespace Personagens {
    Jogador::Jogador():
        Personagem(),
        observer_jogador(0),
        sorte(0.0f),
        pontos(0.0f),
        escudo(0.0f),
        ouro(500),
        experiencia(0),
        nivelInvocador(1),
        abates(0),
        mortes(0),
        assistencias(0),
        cargasUltimate(0) {
        setNome("Campeao");
        setTipo(Entidades::ENTIDADE_JOGADOR);
        setVidaMaxima(620);
        setVida(620);
        setManaMaxima(300);
        setMana(300);
        setForca(60);
        setArmadura(30);
        setResistenciaMagica(30);
        setVelocidadeMovimento(330);
        setAlcanceAtaque(550);
    }

    Jogador::~Jogador() {}

    bool Jogador::colidir(Inimigo* I) {
        return (I != 0);
    }

    void Jogador::atualizar() {
        regenerarAtributos(1.0f);
        mover();
    }

    void Jogador::salvar() {
        salvarDataBuffer();
    }

    void Jogador::mover() {
        sf::Vector2f posicao = getPosicao();
        sf::Vector2f vel = getVelocidade();
        posicao.x += vel.x;
        posicao.y += vel.y;
        setPosicao(posicao);
    }

    void Jogador::desenhar(sf::RenderWindow& window) {
        window.draw(getCorpo());
    }

    Gerenciadores::Observador_Input* Jogador::get_observer_jogador() {
        return observer_jogador;
    }

    void Jogador::adicionarPontos(float valor) {
        if (valor > 0.0f)
            pontos += valor;
    }

    void Jogador::ganharOuro(int valor) {
        if (valor > 0)
            ouro += valor;
    }

    void Jogador::ganharExperiencia(int valor) {
        if (valor <= 0)
            return;
        experiencia += valor;
        while (experiencia >= 1000) {
            experiencia -= 1000;
            ++nivelInvocador;
            subirNivel();
            setVidaMaxima(getVidaMaxima() + 90);
            setManaMaxima(getManaMaxima() + 40);
            setForca(getForca() + 5);
            setVida(getVidaMaxima());
            setMana(getManaMaxima());
        }
    }

    void Jogador::registrarAbate() {
        ++abates;
        ganharOuro(300);
        adicionarPontos(150.0f);
        adicionarCargaUltimate();
    }

    void Jogador::registrarMorte() {
        ++mortes;
        setEstado(ESTADO_MORTO);
    }

    void Jogador::registrarAssistencia() {
        ++assistencias;
        ganharOuro(150);
        adicionarPontos(75.0f);
    }

    void Jogador::adicionarCargaUltimate() {
        if (cargasUltimate < 3)
            ++cargasUltimate;
    }

    bool Jogador::podeUsarUltimate() const {
        return cargasUltimate > 0 && getNivelInvocador() >= 6 && getMana() >= 100;
    }

    bool Jogador::consumirCargaUltimate() {
        if (!podeUsarUltimate())
            return false;
        --cargasUltimate;
        return gastarMana(100);
    }

    int Jogador::absorverDano(int dano) {
        if (dano <= 0)
            return 0;
        int mitigado = dano;
        if (escudo > 0.0f) {
            float restanteEscudo = escudo - static_cast<float>(dano);
            if (restanteEscudo >= 0.0f) {
                escudo = restanteEscudo;
                return 0;
            }
            mitigado = static_cast<int>(-restanteEscudo);
            escudo = 0.0f;
        }
        return receberDanoFisico(mitigado);
    }

    void Jogador::interagir_Colisao(Inimigo* I) {
        if (I)
            absorverDano(I->causarDanoBasico());
    }

    void Jogador::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O)
            setColisao(true);
    }

    void Jogador::interagir_Colisao(Entidades::Projetil* P) {
        if (P)
            absorverDano(P->getDano());
    }

    void Jogador::interagir_Colisao(Jogador* J) {
        if (J && J != this)
            setColisao(true);
    }
}
