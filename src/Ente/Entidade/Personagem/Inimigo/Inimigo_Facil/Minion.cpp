#include "Minion.h"

#include <iostream>

#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"

Minion::Minion() :
    Inimigo(),
    raio(200.f)
{
    sementear();

    setNome("minion"),
    velocidadeMax = 250.f;
    nivelMaldade = 32;
    setVida(170);
    alcancePerseguicao = 300;
    alcanceAtaque = 150;
    elite = rand() % 10 < 5;
    poder = elite? 60.0 * (getVelocidade().x/velocidadeMax) + nivelMaldade: 60 + nivelMaldade;
    cooldownAtaque = 6.5f;
    tempoUltimoAtaque = 0.0f;
    limiteDeslocamento = 600.f;
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/minionrangedsheet2.png"); // obtido em: https://modelviewer.lol/
    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {
            getSprite().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.08f;
            frameWidth = 1262;
            frameHeight = 1028;
            getSprite().setScale(0.08f, 0.08f);
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getSprite().setTextureRect(rectAtual);
        }
        else {
            std::cerr << "Erro: nÃ£o foi possivel carregar a spritesheet do inimigo facil em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Minion::~Minion() {

}

void Minion::danificar(Personagens::Jogador* J) {
    setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
    setEstado(Personagens::ESTADO_OCIOSO);
    interagindo = true;

    if (tempoUltimoAtaque >= cooldownAtaque) {
        if (J && !J->getInvulneravel()) {
            J->receberDano(causarDanoBasico());
            std::cout << getNome() << " atacou o jogador! Dano causado: " << causarDanoBasico() << std::endl;
        }
        tempoUltimoAtaque = 0.0f;
    }
}

sf::FloatRect Minion::getTamanho() const {
    sf::FloatRect caixaImagem = getSprite().getGlobalBounds();

    float largura = 60.f;
    float altura = 60.f;
    return sf::FloatRect(
        caixaImagem.left + (caixaImagem.width / 2.f) - (largura / 2.f),
        caixaImagem.top + (caixaImagem.height / 2.f) - (altura / 2.f),
        largura,
        altura
    );
}

void Minion::executar() {

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

    const float dt = 0.016f;
    tempoUltimoAtaque += dt;

    sf::Vector2f posInimigo = getPosicao();

    Personagens::Jogador* alvoMaisProximo = NULL;
    float menorDistancia = -1.f;

    for (size_t i = 0; i < listaJogadores.size(); ++i) {
        Personagens::Jogador* j = listaJogadores[i];

        if (j != NULL && j->estaVivo()) {
            sf::Vector2f posJogador = j->getPosicao();

            float dx = posJogador.x - posInimigo.x;
            float dy = posJogador.y - posInimigo.y;
            float distancia = std::sqrt(dx * dx + dy * dy);

            if (menorDistancia < 0.0f || distancia < menorDistancia) {
                menorDistancia = distancia;
                alvoMaisProximo = j;
            }
        }
    }
    interagindo = false;

    if (alvoMaisProximo != NULL) {
        sf::Vector2f posAlvo = alvoMaisProximo->getSprite().getPosition();
        float dx = posAlvo.x - posInimigo.x;
        float dy = posAlvo.y - posInimigo.y;

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque()) {

            danificar(alvoMaisProximo);
            
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

    if (!interagindo) {
        float distanciaNesteFrame = std::abs(velocidadeMax * dt);
        deslocamentoPatrulha += distanciaNesteFrame;

        // Se atingiu o limite de passos, vira para o outro lado
        if (deslocamentoPatrulha >= limiteDeslocamento) {
            inverterPatrulha();
        }

        moverHorizontal(direcaoPatrulha);
    }

    sf::Vector2f pos = getPosicao();
    pos.x += getVelocidade().x * dt;
    setPosicao(pos);
}

void Minion::salvar() {
    if (buffer) {
        (*buffer) << "MINION ";

        salvarDataBuffer();
        salvarPersonagem();
        salvarInimigo();

        (*buffer) << raio << '\n';
    }
}