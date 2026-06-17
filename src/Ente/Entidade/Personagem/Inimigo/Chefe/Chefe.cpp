//
// Created by Henrique on 05/05/2026.
//

#include "Chefe.h"
#include <iostream>
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

Chefe::Chefe() :
    Inimigo(),
    forca(2),
    bolaDeFogo(NULL)
{
    sementear();

    setNome("Dragao Sabio"),
    velocidadeMax = 220.f;
    nivelMaldade = 200;
    poder = 50;
    setVidaMaxima(1200);
    setVida(1200);
    alcancePerseguicao = 400;
    alcanceAtaque = 250;
    elite = rand() % 10 < 1;
    cooldownAtaque = 4.f;
    tempoUltimoAtaque = 0.0f;
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/eldersheet2.png");

    if (gerar_num_binom() > 5) { poder *= forca; }

    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {

            getSprite().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.13f;
            frameWidth = 128;
            frameHeight = 128;
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getSprite().setTextureRect(rectAtual);
            getSprite().setScale(3.f, 3.f);
        }
        else {
            std::cerr << "Erro: n�o foi possivel carregar a spritesheet do Chefe em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Chefe::~Chefe() {

}

void Chefe::danificar(Personagens::Jogador* J) {
    if (J) {
        J->receberDano(causarDanoBasico());
        std::cout << getNome() << " atacou o jogador! Dano causado : " << causarDanoBasico() << std::endl;
    }
}

sf::FloatRect Chefe::getTamanho() const {
    sf::FloatRect caixaImagem = getSprite().getGlobalBounds();
    float largura = 200.f;
    float altura = 180.f;
    return sf::FloatRect(
        caixaImagem.left + (caixaImagem.width / 2.f) - (largura / 2.f),
        caixaImagem.top+40.f + (caixaImagem.height / 2.f) - (altura / 2.f),
        largura,
        altura
    );
}

void Chefe::executar() {

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


    float dt = 0.016f;
    tempoUltimoAtaque += dt;

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
        float dy = posAlvo.y - posInimigo.y;

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque() && !alvoMaisProximo->getInvulneravel()) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            //setEstado(Personagens::ESTADO_OCIOSO);
            interagindo = true;

            if (tempoUltimoAtaque >= cooldownAtaque) {
                danificar(alvoMaisProximo);
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

            if (bolaDeFogo && tempoUltimoAtaque >= cooldownAtaque) {
                if (dx > 0) { bolaDeFogo->setPosicao(sf::Vector2f(posInimigo.x + getTamanho().width / 2, posInimigo.y - getTamanho().height/2)); }
                else { bolaDeFogo->setPosicao(sf::Vector2f(posInimigo.x - getTamanho().width / 2, posInimigo.y - getTamanho().height/2)); }
                bolaDeFogo->setVigente(true);
                bolaDeFogo->setDoJogador(false);
                bolaDeFogo->setDano(poder);
                Gerenciadores::Gerenciador_Colisao::getGerenciador().incluirEntidade(bolaDeFogo);
                bolaDeFogo->getSprite().setScale(1.f, 1.f);

                float dirX = dx / menorDistancia;
                float dirY = dy / menorDistancia;

                const float velocidadeTiro = 300.f;
                bolaDeFogo->setVelocidade(sf::Vector2f(dirX * velocidadeTiro, dirY * velocidadeTiro));
                tempoUltimoAtaque = 0.0f;
            }
        }
    }

    if (!interagindo) { moverHorizontal(0.f); }

    sf::Vector2f pos = getPosicao();
    pos.x += getVelocidade().x * dt;
    setPosicao(pos);
}

void Chefe::salvar() {
}
