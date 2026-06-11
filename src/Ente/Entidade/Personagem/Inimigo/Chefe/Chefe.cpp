//
// Created by Henrique on 05/05/2026.
//

#include "Chefe.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

Chefe::Chefe() :
    Inimigo(),
    forca(2)
{
    sementear();

    velocidadeMax = 220.f;
    nivelMaldade = 200;
    poder = 130;
    setVida(1200);
    alcancePerseguicao = 600;
    alcanceAtaque = 200;
    elite = rand() % 10 < 1;
    cooldownAtaque = 1.8f;
    tempoUltimoAtaque = 0.0f;
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/eldersheet2.png");

    if (rand() % 10 < 5) { poder *= forca; }

    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {

            getSprite().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.08f;
            frameWidth = 128;
            frameHeight = 128;
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getSprite().setTextureRect(rectAtual);
        }
        else {
            std::cerr << "Erro: n�o foi possivel carregar a spritesheet do chefe em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Chefe::~Chefe() {

}

void Chefe::danificar(Personagens::Jogador* J) {
    if (J) {
        J->receberDano(causarDanoBasico());
        std::cout << "Dragao Sabio atacou o jogador! Dano causado : " << causarDanoBasico() << std::endl;
    }
}

sf::FloatRect Chefe::getTamanho() const {
    sf::FloatRect caixaImagem = getSprite().getGlobalBounds();
    // sprite: 128*2.5 = 320x320, origin no centro
    // hitbox menor e centralizada verticalmente no personagem
    float largura = 120.f;
    float altura = 140.f;
    return sf::FloatRect(
        caixaImagem.left + (caixaImagem.width / 2.f) - (largura / 2.f),
        caixaImagem.top + (caixaImagem.height / 2.f) - (altura / 2.f),
        largura,
        altura
    );
}

void Chefe::executar() {

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
    bool interagindo = false;

    if (alvoMaisProximo != NULL) {
        sf::Vector2f posAlvo = alvoMaisProximo->getSprite().getPosition();
        float dx = posAlvo.x - posInimigo.x;

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque()) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            interagindo = true;

            if (tempoUltimoAtaque >= cooldownAtaque) {
                danificar(alvoMaisProximo);
                tempoUltimoAtaque = 0.0f;
            }
        }
        // Comportamento de Perseguir
        else if (menorDistancia <= alcancePerseguicao) {
            interagindo = true;

            if (dx > 0) {
                moverHorizontal(1.0f);
            }
            else {
                moverHorizontal(-1.0f);
            }
        }
    }

    if (!interagindo) { moverHorizontal(0.f); }
}

void Chefe::salvar() {
}
