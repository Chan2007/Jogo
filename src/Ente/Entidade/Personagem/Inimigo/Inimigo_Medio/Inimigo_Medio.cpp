//
// Created by Henrique on 05/05/2026.
//

#include "Inimigo_Medio.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

Inimigo_Medio::Inimigo_Medio() :
    Inimigo(),
    raio(250.f),
    tamanho(40)
{
    Ente::sementear();

    velocidadeMax = 8.f;
    nivelMaldade = 64;
    poder = 65;
    setVida(450);
    alcancePerseguicao = 0;
    alcanceAtaque = 60;
    elite = rand() % 10 < 3;
    cooldownAtaque = 2.0f;
    tempoUltimoAtaque = 0.0f;
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/bluesheet.png");

    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {
            getSprite().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.8f;
            frameWidth = 128;
            frameHeight = 128;
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getSprite().setTextureRect(rectAtual);
        }
        else {
            std::cerr << "Erro: não foi possivel carregar a spritesheet do inimigo médio em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Inimigo_Medio::~Inimigo_Medio() {
}

void Inimigo_Medio::danificar(Personagens::Jogador* J) {
    if (J) {
        J->receberDano(causarDanoBasico());
        std::cout << "Azulo atacou o jogador! Dano causado: " << causarDanoBasico() << std::endl;
    }
}

void Inimigo_Medio::atualizar() {
    executar();

    float dt = 0.016f;
    tempoUltimoAtaque += clockAnimacao.restart().asSeconds();

    sf::Vector2f posInimigo = getSprite().getPosition();

    Personagens::Jogador* alvoMaisProximo = NULL;
    float menorDistancia = -1.0f;

    for (size_t i = 0; i < listaJogadores.size(); ++i) {
        Personagens::Jogador* j = listaJogadores[i];

        if (j != NULL && j->estaVivo()) {
            sf::Vector2f posJogador = j->getSprite().getPosition();

            float dx = posJogador.x - posInimigo.x;
            float dy = posJogador.y - posInimigo.y;
            float distancia = std::sqrt(dx * dx + dy * dy);

            if (menorDistancia < 0.0f || distancia < menorDistancia) {
                menorDistancia = distancia;
                alvoMaisProximo = j;
            }
        }
    }
    bool atacando = false;

    if (alvoMaisProximo != NULL) {
        sf::Vector2f posAlvo = alvoMaisProximo->getSprite().getPosition();
        float dx = posAlvo.x - posInimigo.x;

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque()) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            atacando = true;

            if (tempoUltimoAtaque >= cooldownAtaque) {

                if (dx > 0) getSprite().setScale(-1.f, 1.f);
                else getSprite().setScale(1.f, 1.f);

                danificar(alvoMaisProximo);
                tempoUltimoAtaque = 0.0f;
            }
        }
    }

    if (!atacando) {

        float distanciaNesteFrame = std::abs(velocidadeMax * dt);
        deslocamentoPatrulha += distanciaNesteFrame;

        // Se atingiu o limite de passos, vira para o outro lado
        if (deslocamentoPatrulha >= limiteDeslocamento) {
            inverterPatrulha();
        }

        moverHorizontal(direcaoPatrulha);
    }
}

void Inimigo_Medio::executar() {

    if (estado == static_cast<int>(Personagens::ESTADO_MOVIMENTO)) {
        frameAcumulado += clockAnimacao.restart().asSeconds();

        if (frameAcumulado >= tempoPorFrame) {

            indexFrameAtual = (indexFrameAtual + 1) % totalFramesAnimacao;

            int coluna = indexFrameAtual % colunasSpritesheet;
            int linha = indexFrameAtual / colunasSpritesheet;

            rectAtual.left = coluna * frameWidth;
            rectAtual.top = linha * frameHeight;

            getSprite().setTextureRect(rectAtual);

            frameAcumulado -= tempoPorFrame;
        }
    }
    else {

        indexFrameAtual = 0;
        rectAtual.left = 0;
        rectAtual.top = 0;
        getSprite().setTextureRect(rectAtual);

        clockAnimacao.restart();
        frameAcumulado = 0.0f;
    }
}

void Inimigo_Medio::mover() {

}

void Inimigo_Medio::salvar() {
}
