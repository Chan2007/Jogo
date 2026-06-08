//
// Created by Henrique on 05/05/2026.
//

#include "Jogador.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    Jogador::Jogador():
        Personagem(),
        ObserverInput(NULL),
        pontos(0.0f),
        abates(0)
    {
        setTipo(Entidades::ENTIDADE_JOGADOR);
    }

    Jogador::~Jogador() {}

    void Jogador::setCampeao(const EscolhaCampeao campeao) {
        switch (campeao) {
        case CAMPEAO_NAAFIRI:
            setNome("Naafiri");
            setVidaMaxima(90);
            setVida(90);
            setAtaque(65);
            setAlcanceAtaque(80);
            setChanceCritica(30);
            setRegeneracao(1.5f);
            setVelocidadeMax(220.0f);
            setEstado(0);

            totalFramesAnimacao = 8;
            colunasSpritesheet = 4;
            linhasSpritesheet = 2;
            tempoPorFrame = 0.08f;
            caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo(
                "assets/sprites/spritesheets/Naafiri/Naafiri_ToS_Basic_Attack_Sprite_Sheet1.png");
            break;
        default:
            break;
        }
        if (!caminhoArquivoSprite.empty()) {
            if (getTextura().loadFromFile(caminhoArquivoSprite)) {
                // Aplica a textura ao Sprite do jogador
                getSprite().setTexture(getTextura());

                // Descobre o tamanho de 1 frame dividindo o tamanho total pelas colunas/linhas
                const sf::Vector2u tamanhoTextura = getTextura().getSize();
                const int frameW = tamanhoTextura.x / colunasSpritesheet;
                const int frameH = tamanhoTextura.y / linhasSpritesheet;

                // Inicializa o retângulo de corte apontando para o primeiro frame (0, 0)
                rectAtual = sf::IntRect(0, 0, frameW, frameH);
                getSprite().setTextureRect(rectAtual);
            }
            else
                std::cerr << "Erro: A textura do boneco falhou ao carregar: " << caminhoArquivoSprite << std::endl;

        }
    }

    void Jogador::atualizar(const float dt) {
        // Apenas lógica de regras do jogo
        regenerarVida(1.0f * dt);
        mover(dt);
    }
    void Jogador::executar(const float dt) {
        atualizar(dt);

        if (getEstado() == static_cast<int>(ANDANDO)) {
            Animador::atualizarSpriteEntidade(
                getSprite(), rectAtual, totalFramesAnimacao, colunasSpritesheet,
                linhasSpritesheet, tempoPorFrame, dt, tempoAcumulado, indexFrameAtual
            );
            getSprite().setTextureRect(rectAtual);
        }
        else {
            // Estado parado
            indexFrameAtual = 0;
            rectAtual.left = 0;
            rectAtual.top = 0;
            getSprite().setTextureRect(rectAtual);

            // O acumulador é zerado para garantir que, ao voltar a andar,
            // a animação comece do frame 0 imediatamente
            tempoAcumulado = 0.0f;
        }
    }
    void Jogador::salvar() {
        // TODO
        salvarDataBuffer();
    }

    void Jogador::mover(const float dt) {
        float direcaoHorizontal = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direcaoHorizontal = 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            direcaoHorizontal = -1.0f;
        }
        moverHorizontal(direcaoHorizontal);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            gerenciadorGravidade.pular(this);
        }

        sf::Vector2f posicao = getPosicao();
        const sf::Vector2f vel = getVelocidade();

        posicao.x += vel.x * dt;

        if (vel.x > 0.0f)
            getSprite().setScale(-1.f, 1.f);

        else if (vel.x < 0.0f)
            getSprite().setScale(1.f, 1.f);


        setPosicao(posicao);
    }

    Gerenciadores::Observador_Input* Jogador::getObserver() {
        return ObserverInput;
    }

    void Jogador::adicionarPontos(float valor) {
        if (valor > 0.0f)
            pontos += valor;
    }

    void Jogador::registrarAbate() {
        ++abates;
        adicionarPontos(150.0f);
    }


    void Jogador::interagir_Colisao(Inimigo* I) {
        if (I)
            receberDano(I->causarDano());
    }

    void Jogador::interagir_Colisao(Obstaculos::Obstaculo* O) {
        if (O)
            setColisao(true);
    }

    void Jogador::interagir_Colisao(Entidades::Projetil* P) {
        if (P)
            receberDano(P->getDano());
    }

    void Jogador::interagir_Colisao(Jogador* J) {
        if (J && J != this)
            setColisao(true);
    }
}
