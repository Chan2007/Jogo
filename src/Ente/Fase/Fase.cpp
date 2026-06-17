

#include "Fase.h"

#include <iostream>

#include "jogo.h"
#include "Listas/ListaEntidades.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Facil/Inimigo_Facil.h"
#include "Ente/Entidade/Personagem/Inimigo/Chefe/Chefe.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"


namespace Fases {
    Fase::Fase() : gerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade::getGerenciador()),
                   gerenciadorColisao(&Gerenciadores::Gerenciador_Colisao::getGerenciador()),
                   gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
                   gerenciadorInput(Gerenciadores::Gerenciador_Input::getGerenciador()) {
        jogo = Jogo::getJogo();
        tamanhoJanela = gerenciadorGrafico->getJanela().getSize();
        criarInimFaceis();
        criarPlataformas();
        criarJogadores();
    }

    void Fase::criarPlataformas() {
        Obstaculos::Plataforma* chao = new Obstaculos::Plataforma(Obstaculos::Plataforma::CHAO);
        if (chao) {
            chao->setPosicao(sf::Vector2f(static_cast<float>(tamanhoJanela.x), static_cast<float>(tamanhoJanela.y) - (chao->getTamanho().height)/2.f));

            LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(chao));
            gerenciadorColisao->incluirEntidade(chao);
        }
        else {std::cerr << "Falha ao criar chão" << std::endl;}

        Obstaculos::Plataforma* novaPlat;
        sementear();
        const int fator = rand() % 3 + 3;
        for (int i = 0; i < fator; i++) {
            double media = 3;
            double desvio_padrao = 2;
            if (std::fabs(gerar_num_norm(media, desvio_padrao) - media) < desvio_padrao)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL1);
            else if (std::fabs(gerar_num_norm(media, desvio_padrao) - media) > 1.5 * desvio_padrao)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL2);
            else
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL3);

            if (!novaPlat) { std::cerr << "Falha ao criar nova plataforma." << std::endl; }

            bool posicaoValida = false;
            int tentativas = 0;
            while (!posicaoValida && tentativas < 500) {
                const int sizex = tamanhoJanela.x - novaPlat->getTamanho().width;
                const int sizey = tamanhoJanela.y - novaPlat->getTamanho().height - chao->getTamanho().height / 2 - 200;
                novaPlat->getSprite().setPosition(
                    static_cast<int>(gerar_num_binom(0, tamanhoJanela.x)) % sizex + novaPlat->getTamanho().width / 2,
                    static_cast<int>(gerar_num_binom(0, tamanhoJanela.x)) % sizey + novaPlat->getTamanho().height / 2 + 200
                );

                sf::FloatRect hitboxExpandida = novaPlat->getSprite().getGlobalBounds();
                hitboxExpandida.left -= 20.f;
                hitboxExpandida.top -= 20.f;
                hitboxExpandida.width += 40.f;
                hitboxExpandida.height += 40.f;
                if (gerenciadorColisao->verificarPosicaoLivre(hitboxExpandida))
                    posicaoValida = true;
                tentativas++;
            }
            if (posicaoValida) {
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(novaPlat));
                gerenciadorColisao->incluirEntidade(novaPlat);
            }
            else delete novaPlat;
        }
        novaPlat = NULL;
    }

    Fase::~Fase() {
        // Esvazia e deleta os ponteiros de entidades acumulados na fase
        // LEntidades.limparLista();
        // Limpa as referências de ponteiros nos gerenciadores para o próximo estado
        gerenciadorColisao->limpar();
        gerenciadorGravidade.limpar();
        gerenciadorInput.desinscrever(jogo->getJogador1());
        if (jogo->getJogador2())
            gerenciadorInput.desinscrever(jogo->getJogador2());
        LEntidades.limparLista();

    }

    bool Fase::trocarMusica(const int fase) const {
        if (diretorio_Audio.empty()) {
            std::cerr << "Sem música disponível! " << std::endl;
            return false;
        }
        gerenciadorAudio.stop();
        std::string nomeArquivo = "";
        if (fase == 1)
            nomeArquivo = "Aurora_s-Theme.ogg";
        else if (fase == 2)
            nomeArquivo = "Down-to-a-Dusty-Plain.ogg";

        const std::string caminho_musica = Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, nomeArquivo);

        // Carrega e configura através do gerenciador de áudio
        gerenciadorAudio.loadMusic(caminho_musica);

        // O próprio méthodo play() do Gerenciador já deve checar internamente se a música está ligada
        gerenciadorAudio.play();
        return true;
    }
    void Fase::criarInimFaceis(){

        Inimigo_Facil* minion = NULL;
        Entidades::Projetil* tiroInim1 = NULL;
        sementear();
        const int fator = rand() % 3 + 3;
        for (int i = 0; i < fator; i++) {
            minion = new Inimigo_Facil();
            if (minion) {
                minion->setPosicao(sf::Vector2f((rand() % (tamanhoJanela.x - 300)) + 300, rand() % tamanhoJanela.y));
                gerenciadorColisao->incluirEntidade(minion);
                gerenciadorGravidade.aplicarGravidade(minion, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(minion));
                tiroInim1 = new Entidades::Projetil();
                if (tiroInim1) {
                    tiroInim1->setDoJogador(false);
                    tiroInim1->setVigente(false);
                    minion->setProjetil(tiroInim1);
                    gerenciadorColisao->incluirEntidade(tiroInim1);
                    gerenciadorGravidade.aplicarGravidade(tiroInim1, true);
                    LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(tiroInim1));
                }
            }
        }
        minion = NULL;

    }
    bool Fase::verificarLimitesJanela(Entidades::Entidade* entidade) {
        if (!entidade) return false;

        const sf::Vector2f posicaoAtual = entidade->getPosicao();
        const sf::FloatRect tamanho = entidade->getTamanho();

        float limiteLargura = static_cast<float>(tamanhoJanela.x);
        float limiteAltura = static_cast<float>(tamanhoJanela.y);

        sf::Vector2f novaPosicao = posicaoAtual;
        bool colidiuBorda = false;

        float metadeLargura = tamanho.width / 2.0f;
        float metadeAltura = tamanho.height / 2.0f;

        // BORDA ESQUERDA/DIREITA
        if (novaPosicao.x - metadeLargura < 0.0f) {
            novaPosicao.x = metadeLargura;
            colidiuBorda = true;
        }
        else if (novaPosicao.x + metadeLargura > limiteLargura) {
            novaPosicao.x = limiteLargura - metadeLargura;
            colidiuBorda = true;
        }

        // BORDA SUPERIOR/INFERIOR
        if (novaPosicao.y - metadeAltura < 0.0f) {
            novaPosicao.y = metadeAltura;
            colidiuBorda = true;
        }
        else if (novaPosicao.y + metadeAltura > limiteAltura) {
            novaPosicao.y = limiteAltura - metadeAltura;
            colidiuBorda = true;
            gerenciadorGravidade.aoTocarChao(entidade, sf::Vector2f(0.0f, -1.0f));

        }

        if (colidiuBorda) {
            entidade->setPosicao(novaPosicao);
            entidade->setColisao(true);
        }
        return colidiuBorda;
    }
    void Fase::definirLimitesJanela() {
        Elemento<Entidades::Entidade>* itEntidades = LEntidades.getPrimeiro();
        while (itEntidades != NULL) {
            verificarLimitesJanela(itEntidades->getInfo());
            itEntidades = itEntidades->getProximo();
        }
    }
    void Fase::criarJogadores() {
        if (jogo->getJogador1()) {
            jogo->getJogador1()->setCampeao(Personagens::CAMPEAO_NAAFIRI);
            jogo->getJogador1()->setPosicao(sf::Vector2f(
                (jogo->getJogador1()->getTamanho().width) / 2,
                tamanhoJanela.y - (jogo->getJogador1()->getTamanho().height) / 2
                ));
            std::cout << "Jogador criado: " << jogo->getJogador1()->getNome() << std::endl;

            jogo->getJogador1()->setIdJogador(1); // Player 1
            gerenciadorInput.inscrever(jogo->getJogador1());
            gerenciadorColisao->incluirEntidade(jogo->getJogador1());
            gerenciadorGravidade.aplicarGravidade(jogo->getJogador1(), true);
            LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(jogo->getJogador1()));
            Personagens::Inimigo::incluirJogador(jogo->getJogador1());
        }
        else std::cout << "Erro: Jogador 1 não alocado!" << std::endl;
        if (jogo->getJogador2() && jogo->getJogador2Ativo()) {
            jogo->getJogador2()->setCampeao(Personagens::CAMPEAO_NAAFIRI);
            jogo->getJogador2()->setPosicao(sf::Vector2f(
                (jogo->getJogador2()->getTamanho().width) * 2,
                tamanhoJanela.y - (jogo->getJogador2()->getTamanho().height) / 2
                ));
            std::cout << "Jogador criado: " << jogo->getJogador2()->getNome() << std::endl;

            jogo->getJogador2()->setIdJogador(2); // Player 2
            gerenciadorInput.inscrever(jogo->getJogador2());
            gerenciadorColisao->incluirEntidade(jogo->getJogador2());
            gerenciadorGravidade.aplicarGravidade(jogo->getJogador2(), true);
            LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(jogo->getJogador2()));
            Personagens::Inimigo::incluirJogador(jogo->getJogador2());
        }
    }
} // Fases