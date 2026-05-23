//
// Created by Henrique on 05/05/2026.
//

#include "Jogador.h"

#include "Jogo_Principal/Entidade/Obstaculo/Obstaculo.h"
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Jogo_Principal/Entidade/Projetil/Projetil.h"
#include "Jogo_Principal/Diretorio/Encontrar_Diretorio.h"

namespace Personagens {
    Jogador::Jogador():
        Personagem(),
        pGravidade(NULL),
        caminhoArquivoSprite(""),
        observer_jogador(0),
        sorte(0.0f),
        pontos(0.0f),
        escudo(0.0f),
        ouro(500),
        experiencia(0),
        nivelInvocador(1),
        abates(0),
        mortes(0),
        assistencias(0),
        cargasUltimate(0) 
    {
        setTipo(Entidades::ENTIDADE_JOGADOR);
    }

    Jogador::~Jogador() {}

    void Jogador::setGerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade* g) {
        pGravidade = g;
    }

    void Jogador::setCampeao(EscolhaCampeao campeao) {
        switch (campeao) {
        case CAMPEAO_NAAFIRI:
            setNome("Naafiri");
            setVidaMaxima(620);
            setVida(620);
            setForca(65);
            setVelocidadeMovimento(340);

            totalFramesAnimacao = 8;
            colunasSpritesheet = 4;
            frameWidth = 230;
            frameHeight = 120;
            tempoPorFrame = 0.08f;
            caminhoArquivoSprite = "primeiro jogo/assets/sprites/spritesheets/Naafiri/Naafiri_ToS_Basic_Attack_Sprite_Sheet1.png";
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

            Encontrar_Diretorio buscador;

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
        regenerarAtributos(1.0f);
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
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

    Gerenciadores::Observador_Input* Jogador::get_observer_jogador() {
        return observer_jogador;
    }

    void Jogador::adicionarPontos(float valor) {
        if (valor > 0.0f)
            pontos += valor;
    }

    void Jogador::ganharOuro(int valor) {
        if (valor > 0)
            ouro += valor;
    }

    void Jogador::ganharExperiencia(int valor) {
        if (valor <= 0)
            return;
        experiencia += valor;
        while (experiencia >= 1000) {
            experiencia -= 1000;
            ++nivelInvocador;
            subirNivel();
            setVidaMaxima(getVidaMaxima() + 90);
            setManaMaxima(getManaMaxima() + 40);
            setForca(getForca() + 5);
            setVida(getVidaMaxima());
            setMana(getManaMaxima());
        }
    }

    void Jogador::registrarAbate() {
        ++abates;
        ganharOuro(300);
        adicionarPontos(150.0f);
        adicionarCargaUltimate();
    }

    void Jogador::registrarMorte() {
        ++mortes;
        setEstado(ESTADO_MORTO);
    }

    void Jogador::registrarAssistencia() {
        ++assistencias;
        ganharOuro(150);
        adicionarPontos(75.0f);
    }

    void Jogador::adicionarCargaUltimate() {
        if (cargasUltimate < 3)
            ++cargasUltimate;
    }

    bool Jogador::podeUsarUltimate() const {
        return cargasUltimate > 0 && getNivelInvocador() >= 6 && getMana() >= 100;
    }

    bool Jogador::consumirCargaUltimate() {
        if (!podeUsarUltimate())
            return false;
        --cargasUltimate;
        return gastarMana(100);
    }

    int Jogador::absorverDano(int dano) {
        if (dano <= 0)
            return 0;
        int mitigado = dano;
        if (escudo > 0.0f) {
            float restanteEscudo = escudo - static_cast<float>(dano);
            if (restanteEscudo >= 0.0f) {
                escudo = restanteEscudo;
                return 0;
            }
            mitigado = static_cast<int>(-restanteEscudo);
            escudo = 0.0f;
        }
        return receberDanoFisico(mitigado);
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
