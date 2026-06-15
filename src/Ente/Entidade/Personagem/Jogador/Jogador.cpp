
#include "Jogador.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Sistema/Fisica/Visitor_Colisao.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    Jogador::Jogador() : Personagem(), movendoEsquerda(false), movendoDireita(false), pulando(false),
    atacando(false), usandoHabilidade(false), pontos(0.0f), abates(0)
    {
        setTipo(Entidades::ENTIDADE_JOGADOR);
        velocidadeMax = 300.f;
    }

    Jogador::~Jogador() {}

    void Jogador::setCampeao(EscolhaCampeao campeao) {
        switch (campeao) {
        case CAMPEAO_NAAFIRI:
            setNome("Naafiri");
            setVidaMaxima(620);
            setVida(620);
            setPoder(999);


            totalFramesAnimacao = 8;
            colunasSpritesheet = 4;
            frameWidth = 230;
            frameHeight = 120;
            tempoPorFrame = 0.12f;
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

            if (caminhoReal.empty())
                std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << caminhoArquivoSprite << std::endl;
            else if (getTextura().loadFromFile(caminhoReal)) {
                getSprite().setTexture(getTextura());
                rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
                getSprite().setTextureRect(rectAtual);
            }
            else
                std::cerr << "Erro: A textura falhou ao carregar: " << caminhoReal << std::endl;
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
        getSprite().setOrigin(static_cast<float>(frameWidth) / 2.0f, static_cast<float>(frameHeight) / 2.0f);
    }

    void Jogador::executar() {

        if (tempoDano > 0.f) {
            tempoDano -= clockDano.restart().asSeconds();
            if (tempoDano <= 0.f) {
                tempoDano = 0.f;
                getSprite().setColor(sf::Color(255, 255, 255, 255)); // restaura cor
            }
        }

        regenerarVida(1.0f);
        mover();

        if (estado == static_cast<int>(ESTADO_MOVIMENTO)) {
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
            // Se estiver PARADO (ESTADO_OCIOSO), reseta para o frame inicial
            indexFrameAtual = 0;
            rectAtual.left = 0;
            rectAtual.top = 0;
            getSprite().setTextureRect(rectAtual);

            clockAnimacao.restart();
            frameAcumulado = 0.0f;
        }
    }

    void Jogador::salvar() {
        salvarDataBuffer();
    }

    void Jogador::mover() {
        float direcaoHorizontal = 0.0f;
        sf::Vector2f posicao = getPosicao();
        sf::Vector2f vel = getVelocidade();

        if (movendoDireita)
            direcaoHorizontal = 1.0f;
        else if (movendoEsquerda)
            direcaoHorizontal = -1.0f;

        // TODO -> Remover depois
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            setInvulneravel(true);
        else
            setInvulneravel(false);

        // Aplica a força de movimento horizontal
        moverHorizontal(direcaoHorizontal);

        posicao.x += vel.x * 0.016f;

        if (vel.x > 0.0f)
            getSprite().setScale(-1.f, 1.f); // Olha para a direita

        else if (vel.x < 0.0f)
            getSprite().setScale(1.f, 1.f);  // Olha para a esquerda

        setPosicao(posicao);
    }

    void Jogador::adicionarPontos(float valor) {
        if (valor > 0.0f)
            pontos += valor;
    }

    void Jogador::registrarAbate() {
        ++abates;
        adicionarPontos(150.0f);
    }
    void Jogador::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->visitar(this);
    }
    void Jogador::aoApertarTecla(const Gerenciadores::Tecla& evento) {
        std::string prefixo = "j" + std::to_string(idJogador) + "_";

        if (evento.acao.rfind(prefixo, 0) != 0) return;

        std::string acao = evento.acao.substr(prefixo.length());

        if (acao == "mover_esquerda") {
            movendoEsquerda = evento.pressionada;
        }
        else if (acao == "mover_direita") {
            movendoDireita = evento.pressionada;
        }
        else if (acao == "pular") {
            if (evento.pressionada)
                gerenciadorGravidade.pular(this);
        }
        else if (acao == "atacar")
            atacando = evento.pressionada;
        else if (acao == "habilidade")
            usandoHabilidade = evento.pressionada;
        else if (acao == "acelerar") {
            // Botão RB
        }
        else if (acao == "desacelerar") {
            // Botão LB
        }
    }
}


