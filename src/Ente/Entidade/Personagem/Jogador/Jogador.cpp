//
// Created by Henrique on 05/05/2026.
//

#include "Jogador.h"

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

namespace Personagens {
    Jogador::Jogador():
        Personagem(),
        pGravidade(NULL),
        ObserverJogador(0),
        pontos(0.0f),
        abates(0)
    {
        setTipo(Entidades::ENTIDADE_JOGADOR);
    }

    Jogador::~Jogador() {}

    void Jogador::setGerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade* g) {
        pGravidade = g;
    }

    Gerenciadores::Gerenciador_Gravidade* Jogador::getGerenciadorGravidade() {
        return pGravidade;
    }

    void Jogador::setCampeao(EscolhaCampeao campeao) {
        switch (campeao) {
        case CAMPEAO_NAAFIRI:
            setNome("Naafiri");
            setVidaMaxima(620);
            setVida(620);
            setForca(65);

            totalFramesAnimacao = 8;
            colunasSpritesheet = 4;
            frameWidth = 230;
            frameHeight = 120;
            tempoPorFrame = 0.08f;
            caminhoArquivoSprite = "assets/sprites/spritesheets/Naafiri/Naafiri_ToS_Basic_Attack_Sprite_Sheet1.png";
            caminhoArquivoSpritePulo = "assets/sprites/spritesheets/Naafiri/Naafiri_Jump_Sprite_Sheet1.png";
            break;
        default:
            setNome("Campeao Generico");
            setVidaMaxima(500);
            setVida(500);
            totalFramesAnimacao = 1;
            colunasSpritesheet = 1;
            frameWidth = 32;
            frameHeight = 32;
            tempoPorFrame = 0.1f;
            break;
        }
        if (!caminhoArquivoSprite.empty()) {

            Encontrar_Caminho buscador;

            std::string caminhoReal = buscador.acharDiretorio_Arquivo(caminhoArquivoSprite);

            if (caminhoReal.empty()) {
                std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << caminhoArquivoSprite << std::endl;
            }
            else if (getTextura().loadFromFile(caminhoReal)) {
                getCorpo().setTexture(getTextura());
                rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
                getCorpo().setTextureRect(rectAtual);
            }
            else {
                std::cerr << "Erro: A textura falhou ao carregar: " << caminhoReal << std::endl;
            }
        }
        if (!caminhoArquivoSpritePulo.empty()) {
            Encontrar_Caminho buscador;
            std::string caminhoRealPulo = buscador.acharDiretorio_Arquivo(caminhoArquivoSpritePulo);
            if (!caminhoRealPulo.empty()) {
                texturaPulo.loadFromFile(caminhoRealPulo);
            }
        }
        else {
            std::cerr << "Erro: Nao foi possivel carregar a textura de: " << getNome() << std::endl;
        }
        getCorpo().setOrigin(static_cast<float>(frameWidth) / 2.0f, static_cast<float>(frameHeight) / 2.0f);
    }

    void Jogador::desenhar(sf::RenderWindow& window) {
        window.draw(getCorpo());
    }

    bool Jogador::colidir(Inimigo* I) {
        return (I != 0);
    }

    void Jogador::atualizar() {
        regenerarVida(1.0f);
        mover();

        if (estado == static_cast<int>(ESTADO_MOVIMENTO)) {
            frameAcumulado += clockAnimacao.restart().asSeconds();

            if (frameAcumulado >= tempoPorFrame) {
                // Avança o frame de 0 a 7
                indexFrameAtual = (indexFrameAtual + 1) % totalFramesAnimacao;

                // Descobre em qual coluna e linha da imagem o frame atual está
                int coluna = indexFrameAtual % colunasSpritesheet;
                int linha = indexFrameAtual / colunasSpritesheet;

                // Calcula a posição exata do corte (X e Y) na folha
                rectAtual.left = coluna * frameWidth;
                rectAtual.top = linha * frameHeight;

                getCorpo().setTextureRect(rectAtual);

                frameAcumulado -= tempoPorFrame;
            }
        }
        else {
            // Se estiver PARADO (ESTADO_OCIOSO), reseta para o frame inicial
            indexFrameAtual = 0;
            rectAtual.left = 0;
            rectAtual.top = 0;
            getCorpo().setTextureRect(rectAtual);

            clockAnimacao.restart();
            frameAcumulado = 0.0f;
        }
    }

    void Jogador::salvar() {
        salvarDataBuffer();
    }

    void Jogador::mover() {
        float direcaoHorizontal = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direcaoHorizontal = 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            direcaoHorizontal = -1.0f;
        }
        moverHorizontal(direcaoHorizontal);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (pGravidade != NULL) {
                pGravidade->pular(this);
            }
        }

        sf::Vector2f posicao = getPosicao();
        sf::Vector2f vel = getVelocidade();

        posicao.x += vel.x * 0.01f;

        if (vel.x > 0.0f) {
            getCorpo().setScale(-1.f, 1.f); // Inverte para olhar para direita
        }
        else if (vel.x < 0.0f) {
            getCorpo().setScale(1.f, 1.f);  // Volta para olhar para esquerda
        }

        setPosicao(posicao);
    }

    Gerenciadores::Observador_Input* Jogador::getObserver() {
        return ObserverJogador;
    }

    void Jogador::adicionarPontos(float valor) {
        if (valor > 0.0f)
            pontos += valor;
    }

    void Jogador::registrarAbate() {
        ++abates;
        adicionarPontos(150.0f);
    }

    int Jogador::absorverDano(int dano) {
        if (dano <= 0)
            return 0;
        return receberDano(dano);
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
