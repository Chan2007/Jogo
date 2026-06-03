//
// Created by Henrique on 05/05/2026.
//

#include "Inimigo_Facil.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

Inimigo_Facil::Inimigo_Facil() :
    Inimigo(),
    raio(200.f),
    tamanho(32),
    forca(15)
    {
    Ente::sementear();
    setVelocidade(sf::Vector2f(0.02f, 0.02f));
    nivelMaldade = 32;
    setVida(170);
    alcancePerseguicao = 200;
    alcanceAtaque = 90;
    elite = rand() % 10 < 3;
    cooldownAtaque = 1.5f;
    tempoUltimoAtaque = 0.0f;
    caminhoArquivoSprite = "assets/sprites/spritesheets/Inimigos/minionrangedsheet.png";
    Encontrar_Caminho buscador;
    caminhoArquivoSprite = buscador.acharDiretorio_Arquivo(caminhoArquivoSprite);
    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {
            getCorpo().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.8f;
            frameWidth = 1262;
            frameHeight = 1028;
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getCorpo().setTextureRect(rectAtual);
        }
        else {
            std::cerr << "Erro: não foi possivel carregar a spritesheet do inimigo facil em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getCorpo().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Inimigo_Facil::~Inimigo_Facil() {

}

void Inimigo_Facil::danificar(Personagens::Jogador* J) {
    if (J) {
        J->receberDano(causarDanoBasico());
        std::cout << "Minion atacou o jogador! Dano causado: " << causarDanoBasico() << std::endl;
    }
}

void Inimigo_Facil::atualizar(Personagens::Jogador* jogador) {
    float dt = 0.016f;

    tempoUltimoAtaque += clockAnimacao.restart().asSeconds();

    if (jogador && jogador->estaVivo()) {

        sf::Vector2f posInimigo = getCorpo().getPosition();
        sf::Vector2f posJogador = (jogador->getCorpo()).getPosition();

        float dx = posJogador.x - posInimigo.x;
        float dy = posJogador.y - posInimigo.y;
        float distancia = std::sqrt(dx * dx + dy * dy);

        if (distancia <= getAlcanceAtaque()) {

            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));

            if (tempoUltimoAtaque >= cooldownAtaque) {
                danificar(jogador);
                tempoUltimoAtaque = 0.0f;

            }
        }
        else if (distancia <= alcancePerseguicao) {

            if (dx > 0) {
                moverHorizontal(1.0f);
            }
            else {
                moverHorizontal(-1.0f);
            }
        }
    }
}

void Inimigo_Facil::executar() {

    frameAcumulado += clockAnimacao.restart().asSeconds();

    if (frameAcumulado >= tempoPorFrame) {
        frameAcumulado = 0.0f;
        indexFrameAtual = (indexFrameAtual + 1) % totalFramesAnimacao;

        int coluna = indexFrameAtual % colunasSpritesheet;
        int linha = indexFrameAtual / colunasSpritesheet;

        getCorpo().setTextureRect(sf::IntRect(coluna * frameWidth, linha * frameHeight, frameWidth, frameHeight));
    }
}

void Inimigo_Facil::salvar() {
}