//
// Created by Henrique on 05/05/2026.
//

#include "DragaoAnciao.h"
#include <iostream>
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    DragaoAnciao::DragaoAnciao() : Inimigo(), forca(2), bolaDeFogo(NULL) {
        sementear();

        setNome("DragaoAnciao"),
        velocidadeMax = 220.f;
        nivelMaldade = 200;
        poder = 80 * log(nivelMaldade * 10);
        setVidaMaxima(1200);
        setVida(1200);
        alcancePerseguicao = 400;
        alcanceAtaque = 250;
        elite = rand() % 10 < 1;
        cooldownAtaque = 4.f;
        tempoUltimoAtaque = 0.0f;

        // -------------------------------------------------------------------------
        // ATRIBUIÇÃO DE ATIVOS (MODELVIEWER.LOL / KHADA)
        // Modelos 3D, malhas ou texturas obtidos via ModelViewer.lol.
        // Propriedade original dos personagens e artes: © Riot Games, Inc.
        // Uso estritamente acadêmico, educacional e não comercial.
        // -------------------------------------------------------------------------

        caminhoArquivoSpriteMovimento = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/eldersheet2.png"); // obtido em: https://modelviewer.lol/

        if (gerar_num_binom() > 5) { poder *= forca; }

        if (!caminhoArquivoSpriteMovimento.empty()) {
            try {
                if (!getTextura().loadFromFile(caminhoArquivoSpriteMovimento))
                    throw std::runtime_error("Erro: não foi possível carregar a spritesheet do DragaoAnciao em: " + caminhoArquivoSpriteMovimento);

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
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
    }

    DragaoAnciao::~DragaoAnciao() {}

    void DragaoAnciao::danificar(Jogador* J) {
        sf::Vector2f posJogador = J->getSprite().getPosition();
        sf::Vector2f posInimigo = getSprite().getPosition();

        float dx = posJogador.x - posInimigo.x;
        float dy = posJogador.y - posInimigo.y;
        float menorDistancia = std::sqrt(dx * dx + dy * dy);

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque() && !J->getInvulneravel()) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            //setEstado(ESTADO_OCIOSO);
            interagindo = true;

            if (tempoUltimoAtaque >= cooldownAtaque) {
                J->receberDano(causarDanoBasico());
                std::cout << getNome() << " atacou o jogador! Dano causado : " << causarDanoBasico() << std::endl;
                tempoUltimoAtaque = 0.0f;
            }
        }
        // Comportamento de Perseguir
        else if (menorDistancia <= alcancePerseguicao) {
            interagindo = true;

            if (dx > 0) {
                moverHorizontal(1.0f);
                getSprite().setScale(-3.f, 3.f);
            }
            else {
                moverHorizontal(-1.0f);
                getSprite().setScale(3.f, 3.f);
            }

            if (bolaDeFogo && tempoUltimoAtaque >= cooldownAtaque) {
                if (dx > 0) { bolaDeFogo->setPosicao(sf::Vector2f(posInimigo.x + getTamanho().width / 2, posInimigo.y - getTamanho().height / 2)); }
                else { bolaDeFogo->setPosicao(sf::Vector2f(posInimigo.x - getTamanho().width / 2, posInimigo.y - getTamanho().height / 2)); }
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

    sf::FloatRect DragaoAnciao::getTamanho() const {
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

    void DragaoAnciao::executar() {

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
        interagindo = false;

        if (alvoMaisProximo != NULL) {
            danificar(alvoMaisProximo);
        }

        if (!interagindo) { moverHorizontal(0.f); }

        sf::Vector2f pos = getPosicao();
        pos.x += getVelocidade().x * dt;
        setPosicao(pos);
    }

    void DragaoAnciao::salvar() {
        if (buffer) {
            (*buffer) << "DRAGAO_ANCIAO ";

            salvarDataBuffer();
            salvarPersonagem();
            salvarInimigo();

            (*buffer) << forca << '\n';
        }
    }
    Memento* DragaoAnciao::salvarMemento() const  {
        return new DragaoAnciaoMemento(*this);
    }

    void DragaoAnciao::restaurarMemento(const Memento* memento) {
        if (!memento) return;
        Inimigo::restaurarMemento(memento);

        const DragaoAnciaoMemento* pMemento = dynamic_cast<const DragaoAnciaoMemento*>(memento);
        if (pMemento) {
            forca = pMemento->forcaMemento;
        }
    }
}
