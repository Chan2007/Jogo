#include "Minion.h"

#include <iostream>

#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    Minion::Minion() : Inimigo(), raio(200.f) {
        sementear();

        nome = "Minion";
        velocidadeMax = 250.f;
        nivelMaldade = 32;
        vida = 170;
        alcancePerseguicao = 300;
        alcanceAtaque = 150;
        elite = rand() % 10 < 5;
        poder = elite? 60.0 * (getVelocidade().x/velocidadeMax) + nivelMaldade: 60 + nivelMaldade;
        cooldownAtaque = 6.5f;
        tempoUltimoAtaque = 0.0f;
        limiteDeslocamento = 600.f;

        // -------------------------------------------------------------------------
        // ATRIBUIÇÃO DE ATIVOS (MODELVIEWER.LOL / KHADA)
        // Modelos 3D, malhas ou texturas obtidos via ModelViewer.lol.
        // Propriedade original dos personagens e artes: © Riot Games, Inc.
        // Uso estritamente acadêmico, educacional e não comercial.
        // -------------------------------------------------------------------------

        caminhoArquivoSpriteMovimento = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/minionrangedsheet2.png"); // obtido em: https://modelviewer.lol/
        if (!caminhoArquivoSpriteMovimento.empty()) {
            try {
                if (!getTextura().loadFromFile(caminhoArquivoSpriteMovimento))
                    throw std::runtime_error("Erro: não foi possível carregar a spritesheet do minion em: " + caminhoArquivoSpriteMovimento);

                getSprite().setTexture(getTextura());
                totalFramesAnimacao = 18;
                colunasSpritesheet = 9;
                tempoPorFrame = 0.08f;
                frameWidth = 1262;
                frameHeight = 1028;
                rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
                getSprite().setTextureRect(rectAtual);
                getSprite().setScale(0.08f, 0.08f);
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
    }

    Minion::~Minion() {}

    void Minion::danificar(Personagens::Jogador* J) {
        setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
        setEstado(Personagens::ESTADO_OCIOSO);
        interagindo = true;

        if (tempoUltimoAtaque >= cooldownAtaque) {
            if (J && !J->getInvulneravel()) {
                J->receberDano(causarDanoNormal());
                std::cout << getNome() << " atacou o jogador! Dano causado: " << causarDanoNormal() << std::endl;
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

        Jogador* alvoMaisProximo = NULL;
        float menorDistancia = -1.f;

        for (int i = 0; i < listaJogadores.size(); ++i) {
            Jogador* j = listaJogadores[i];

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
            if (menorDistancia <= alcanceAtaque) {

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
    Gerenciadores::Memento* Minion::salvarMemento() const  {
        return new MinionMemento(*this);
    }

    void Minion::restaurarMemento(const Gerenciadores::Memento* memento) {
        if (!memento) return;
        Inimigo::restaurarMemento(memento);

        const MinionMemento* pMemento = dynamic_cast<const MinionMemento*>(memento);
        if (pMemento) {
            raio = pMemento->raioMemento;
        }
    }
}
