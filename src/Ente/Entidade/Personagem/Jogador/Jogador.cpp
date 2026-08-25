
#include "Jogador.h"

#include <iostream>

#include "jogo.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Gerenciador/Gerenciador_Estado/Gerenciador_Estado.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    Jogador::Jogador(sf::Vector2f posicao, sf::Color corBarra) :
    Personagem(), movendoEsquerda(false), movendoDireita(false),
    pulando(false), atacando(false), usandoHabilidade(false),
    correndo(false), frameHeightAtaque(), frameWidthAtaque(),
    frameHeightMovimento(), frameWidthMovimento(), frameHeightPulo(),
    frameWidthPulo(), frameHeightOcioso(), frameWidthOcioso(),
    pontos(0.0f), abates(0), fundoVida(), barraVida(), idJogador(-1)
    {
        velocidadeMax = 300.f;

        fundoVida.setSize(sf::Vector2f(750.0f, 70.0f));
        fundoVida.setPosition(posicao.x - 20.f, posicao.y - 10.0f);
        fundoVida.setFillColor(sf::Color(100, 100, 100));

        barraVida.setSize(sf::Vector2f(700.0f, 50.0f));
        barraVida.setPosition(posicao);
        barraVida.setFillColor(corBarra);

        desenharBarra();
    }

    Jogador::~Jogador() {
        std::cout << "ALERTA: JOGADOR FOI DELETADO!" << std::endl;
    }

    void Jogador::setCampeao(Campeao c) {
        switch (c) {
            case NAAFIRI:
                vidaMaxima = 620;
                vida = 620;
                poder = 1999;


                totalFramesAnimacao = 8;
                colunasSpritesheet = 4;
                frameWidth = 230;
                frameHeight = 120;
                tempoPorFrame = 0.12f;
                // -------------------------------------------------------------------------
                // ATRIBUIÇÃO DE ATIVOS (SPRITES DA NAAFIRI)
                // Imagens, ícones e sprites obtidos via League of Legends Wiki (Fandom).
                // Propriedade original: © Riot Games, Inc.
                // Uso estritamente acadêmico, educacional e não comercial.
                // -------------------------------------------------------------------------

                caminhoArquivoSpriteOcioso = "assets/sprites/spritesheets/Naafiri/Naafiri_ToS_Basic_Attack_Sprite_Sheet1.png";
                caminhoArquivoSpritePulo = "assets/sprites/spritesheets/Naafiri/Naafiri_ToS_Basic_Attack_Sprite_Sheet1.png";
                break;
            default:
                nome = "Campeão";
                vidaMaxima = 500;
                vida = 500;
                totalFramesAnimacao = 1;
                colunasSpritesheet = 1;
                frameWidth = 32;
                frameHeight = 32;
                tempoPorFrame = 0.1f;
                break;
        }
        if (!caminhoArquivoSpriteOcioso.empty()) {

            std::string caminhoReal = Encontrar_Caminho::acharDiretorio_Arquivo(caminhoArquivoSpriteOcioso);

            if (caminhoReal.empty())
                std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << caminhoArquivoSpriteOcioso << std::endl;
            try {
                if (!getTextura().loadFromFile(caminhoReal))
                    throw std::runtime_error("Erro: A textura falhou ao carregar: " + caminhoReal);

                getSprite().setTexture(getTextura());
                rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
                getSprite().setTextureRect(rectAtual);
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        if (!caminhoArquivoSpritePulo.empty()) {

            std::string caminhoRealPulo = Encontrar_Caminho::acharDiretorio_Arquivo(caminhoArquivoSpritePulo);
            if (caminhoRealPulo.empty())
                std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << caminhoArquivoSpritePulo << std::endl;
            try {
                if (!getTextura().loadFromFile(caminhoRealPulo))
                    throw std::runtime_error("Erro: A textura falhou ao carregar: " + caminhoRealPulo);
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        getSprite().setOrigin(static_cast<float>(frameWidth) / 2.0f, static_cast<float>(frameHeight) / 2.0f);
    }

    void Jogador::executar() {

        atualizarBarra();
        if (tempoDano > 0.f) {
            tempoDano -= clockDano.restart().asSeconds();
            if (tempoDano <= 0.f) {
                tempoDano = 0.f;
                getSprite().setColor(sf::Color(255, 255, 255, 255)); // restaura cor
            }
        }

        regenerarVida(0.1f);
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
        else if (estado == static_cast<int>(ESTADO_MORTO)) {
            Gerenciadores::Gerenciador_Estado::getGerenciador().notificar(Gerenciadores::EVENTO_JOGADOR_MORREU);
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
        if (buffer) {
            (*buffer) << "JOGADOR ";

            salvarDataBuffer();
            salvarPersonagem();

            (*buffer) << pontos << ' '
                << abates << ' '
                << idJogador << '\n';
        }
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            setInvulneravel(true);
        else
            setInvulneravel(false);

        // Aplica a força de movimento horizontal
        moverHorizontal(direcaoHorizontal);

        if (correndo) vel.x *= 1.5f;
        posicao.x += vel.x * 0.016f;

        if (vel.x > 0.0f)
            getSprite().setScale(-1.f, 1.f); // Olha para a direita

        else if (vel.x < 0.0f)
            getSprite().setScale(1.f, 1.f);  // Olha para a esquerda

        setPosicao(posicao);
    }

    void Jogador::mudarPontos(float valor) {
        pontos += valor;
        //std::cout << getNome() << " recebeu " << valor << " pontos. Pontuacao atual: " << pontos << std::endl;
    }

    void Jogador::registrarAbate() {
        ++abates;
        mudarPontos(150.0f);
    }
    /*
    void Jogador::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
    */
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
        else if (acao == "correr")
            correndo = evento.pressionada;
        else if (acao == "acelerar") {
            // Botão RB
        }
        else if (acao == "desacelerar") {
            // Botão LB
        }

    }
    void Jogador::desenharBarra() {
        gerenciadorGrafico->draw(fundoVida);
        gerenciadorGrafico->draw(barraVida);
    }
    void Jogador::atualizarBarra() {
        float proporcaoVida = static_cast<float>(vida) / static_cast<float>(vidaMaxima);
        barraVida.setSize(sf::Vector2f(700.0f * proporcaoVida, 50.0f));
    }
    Gerenciadores::Memento* Jogador::salvarMemento() const {
        return new JogadorMemento(*this);
    }

    void Jogador::restaurarMemento(const Gerenciadores::Memento* memento) {
        if (!memento) return;
        Personagem::restaurarMemento(memento);

        const JogadorMemento* pMemento = dynamic_cast<const JogadorMemento*>(memento);
        if (pMemento) {
            movendoEsquerda = pMemento->movendoEsquerdaMemento;
            movendoDireita  = pMemento->movendoDireitaMemento;
            pulando = pMemento->pulandoMemento;
            atacando = pMemento->atacandoMemento;
            correndo = pMemento->correndoMemento;
            usandoHabilidade = pMemento->usandoHabilidadeMemento;
            pontos = pMemento->pontosMemento;
            abates = pMemento->abatesMemento;
            idJogador = pMemento->idJogadorMemento;
            fundoVida = pMemento->fundoVidaMemento;
            barraVida = pMemento->barraVidaMemento;
            frameHeightAtaque = pMemento->frameHeightAtaqueMemento;
            frameHeightMovimento = pMemento->frameHeightMovimentoMemento;
            frameHeightOcioso = pMemento->frameHeightOciosoMemento;
            frameHeightPulo = pMemento->frameHeightPuloMemento;
            frameWidthAtaque = pMemento->frameWidthAtaqueMemento;
            frameWidthMovimento = pMemento->frameWidthMovimentoMemento;
            frameWidthOcioso = pMemento->frameWidthOciosoMemento;
            frameWidthPulo = pMemento->frameWidthPuloMemento;
        }
    }
}