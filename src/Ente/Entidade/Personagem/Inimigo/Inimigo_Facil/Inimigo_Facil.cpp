//
// Created by Henrique on 05/05/2026.
//

#include "Inimigo_Facil.h"

#include <iostream>

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include "Sistema/UI/Animador.h"

Personagens::Inimigo_Facil::Inimigo_Facil() :
    Inimigo(),
    raio(200.f),
    tamanho(32)
{
    sementear();

    setVelocidade(sf::Vector2f(0.02f, 0.02f));
    setAtaque(20);
    setVida(50);
    setAlcanceAtaque(100),
    setAlcancePerseguicao(200),
    setElite(rand() % 10 < 3),
    setCooldownAtaque(1.5f);
    setTempoUltimoAtaque(0.0f);
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/minionrangedsheet.png");

    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {
            getSprite().setTexture(getTextura());

            // Novos parâmetros:
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            linhasSpritesheet = 2;
            tempoPorFrame = 0.06f;

            // O motor descobre a largura e altura do frame dividindo o total pelas colunas/linhas:
            const sf::Vector2u tamanhoTextura = getTextura().getSize();
            const int frameW = tamanhoTextura.x / colunasSpritesheet;
            const int frameH = tamanhoTextura.y / linhasSpritesheet;

            rectAtual = sf::IntRect(0, 0, frameW, frameH);
            getSprite().setTextureRect(rectAtual);
        }
    }
}

Personagens::Inimigo_Facil::~Inimigo_Facil() {

}

void Personagens::Inimigo_Facil::danificar(Jogador* J) {
    if (J) {
        J->receberDano(causarDano());
        std::cout << "Minion atacou o jogador! Dano causado: " << causarDano() << std::endl;
    }
}

void Personagens::Inimigo_Facil::atualizar(const float dt) {

    setTempoUltimoAtaque(getTempoUltimoAtaque() + dt);

    if (jogador && jogador->estaVivo()) {
        if (deveAtacar(jogador->getPosicao())) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            if (getTempoUltimoAtaque() >= getCooldownAtaque()) {
                danificar(jogador);
                setTempoUltimoAtaque(0.0f);
            }
        }
        else if (devePerseguir(jogador->getPosicao())) {
            const float dx = jogador->getPosicao().x - getPosicao().x;
            moverHorizontal(dx > 0 ? 1.0f : -1.0f);
        }
    }
}

void Personagens::Inimigo_Facil::executar(const float dt) {
    atualizar(dt);
    Animador::atualizarSpriteEntidade(
        getSprite(), rectAtual, totalFramesAnimacao, colunasSpritesheet,
        linhasSpritesheet, tempoPorFrame, dt, tempoAcumulado, indexFrameAtual
    );
    getSprite().setTextureRect(rectAtual);
}