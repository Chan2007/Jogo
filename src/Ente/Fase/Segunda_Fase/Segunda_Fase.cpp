//
// Created by Henrique on 09/05/2026.
//

#include "Segunda_Fase.h"

#include "Ente/Entidade/Obstaculo/Obstaculo_Dificil/Pinstouro.h"
#include "Ente/Entidade/Personagem/Inimigo/Chefe/DragaoAnciao.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Estado/Gerenciador_Estado.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"



namespace Fases {
    Segunda_Fase::Segunda_Fase() : Fase(), maxChefoes(5) {
        Segunda_Fase::criarCenario();

        if (jogo->getJogador1())
            jogo->getJogador1()->setPosicao(sf::Vector2f(50.0f, 300.0f));

        if (jogo->getJogador2Ativo() && jogo->getJogador2())
            jogo->getJogador2()->setPosicao(sf::Vector2f(100.0f, 300.0f));

        if (!jogo->getCarregandoSave()) {
            Segunda_Fase::criarObstaculos();
            Segunda_Fase::criarInimigos();
            Segunda_Fase::criarProjeteis();
        }
    }

    Segunda_Fase::~Segunda_Fase() {}

    void Segunda_Fase::criarCenario() {
        // -------------------------------------------------------------------------
        // ATRIBUIÇÃO DE ATIVOS (LEAGUE DISPLAYS)
        // Os vídeos utilizados nesta tela são de propriedade da Riot Games, Inc.
        // Uso não comercial permitido para fins educacionais e comunitários.
        // -------------------------------------------------------------------------
        diretorio_Frames_Fase = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase2");
        diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

        if (!diretorio_Frames_Fase.empty())
            gerenciadorGrafico->loadAnimation(diretorio_Frames_Fase,"bg_fase2_",451,2,4,7);

        if (!diretorio_Audio.empty())
            trocarMusica(2);
    }

    // Inputs específicos da Fase 2
    void Segunda_Fase::processarEventos(const sf::Event& evento) {
        gerenciadorInput.notificarObservadores(evento);
    }

    // Cuidar apenas da evolução da física/lógica
    void Segunda_Fase::executar() {
        LEntidades.percorrer();
        LEntidades.percorrer();

        Lista<Entidades::Entidade>::IteratorLista it = LEntidades.getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);
        bool jogador1Vivo = false;
        bool jogador2Vivo = false;
        bool existemInimigos = false;
        while (it != fim) {
            if (it->getVigente()) {
                if (it->getNome() == jogo->getJogador1()->getNome())
                    jogador1Vivo = true;
                else if (jogo->getJogador2Ativo() && it->getNome() == jogo->getJogador2()->getNome())
                    jogador2Vivo = true;
                else if (it->getNome() == "Minion" || it->getNome() == "DragaoAnciao")
                    existemInimigos = true;
            }
            ++it;
        }

        if (!jogador1Vivo && !jogador2Vivo) {
            Gerenciadores::Gerenciador_Estado::getGerenciador().notificar(Gerenciadores::EVENTO_JOGADOR_MORREU);
            return; // Aborta o resto para evitar erros de ponteiro nulo
        }
        if (!existemInimigos) {
            Gerenciadores::Gerenciador_Estado::getGerenciador().notificar(Gerenciadores::EVENTO_FASE2_CONCLUIDA);
        }
        gerenciadorGrafico->updateAnimation();

        // Executa gerenciadores de física usando o delta time recebido do Jogo
        gerenciadorGravidade.executar();
        gerenciadorColisao->executar();
        definirLimitesJanela();
    }

    // Cuidar apenas de mandar os elementos para a janela
    void Segunda_Fase::desenhar() {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas();
        if (jogo->getJogador1()) {
            jogo->getJogador1()->desenharBarra();
            jogo->getJogador1()->atualizarBarra();
        }

        // JOGADOR 2: Fixo no Canto Superior Direito
        if (jogo->getJogador2() && jogo->getJogador2Ativo()) {
            jogo->getJogador2()->desenharBarra();
            jogo->getJogador2()->atualizarBarra();
        }
    }
    void Segunda_Fase::criarChefoes() {
        Personagens::DragaoAnciao* ElderDragon = NULL;
        Entidades::Projetil* tiroInim = NULL;
        const int fator = gerar_num_exp(3, maxChefoes, 0.5);
        for (int i = 0; i < fator; i++) {
            ElderDragon = new Personagens::DragaoAnciao();
            if (ElderDragon) {
                ElderDragon->setPosicao(
                    sf::Vector2f(
                    gerar_num_norm((tamanhoJanela.width - 300)/2.0, tamanhoJanela.width/6.0,0,
                                    static_cast<int>(tamanhoJanela.width - 300)),
                    gerar_num_norm(tamanhoJanela.height/2.0, tamanhoJanela.height/6.0, 0,
                                    static_cast<int>(tamanhoJanela.height))
                    )
                );
                gerenciadorColisao->incluirEntidade(ElderDragon);
                gerenciadorGravidade.aplicarGravidade(ElderDragon, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(ElderDragon));
                tiroInim = new Entidades::Projetil();
                if (tiroInim) {
                    tiroInim->setDoJogador(false);
                    tiroInim->setVigente(false);
                    ElderDragon->setProjetil(tiroInim);
                    gerenciadorColisao->incluirEntidade(tiroInim);
                    gerenciadorGravidade.aplicarGravidade(tiroInim, true);
                    LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(tiroInim));
                }
            }
            ElderDragon = NULL;
        }
    }

    void Segunda_Fase::criarProjeteis() {

    }
    void Segunda_Fase::criarObstDificeis() {
        Obstaculos::Pinstouro* Pinstouro = NULL;
        for (int i = 1; i <= 3; i++) {
            Pinstouro = new Obstaculos::Pinstouro();
            if (Pinstouro) {
                Pinstouro->setPosicao(sf::Vector2f(300 * i, (rand() % tamanhoJanela.height - 300) + 300));
                gerenciadorColisao->incluirEntidade(Pinstouro);
                gerenciadorGravidade.aplicarGravidade(Pinstouro, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(Pinstouro));
            }
            Pinstouro = NULL;
        }
    }
    Memento* Segunda_Fase::salvarMemento() const {
        return new Segunda_FaseMemento(*this);
    }

    void Segunda_Fase::restaurarMemento(const Memento* memento) {
        Fase::restaurarMemento(memento);
        const Segunda_FaseMemento* pMemento = dynamic_cast<const Segunda_FaseMemento*>(memento);
        if (pMemento) {
            maxChefoes = pMemento->maxChefoesMemento;
            diretorio_Frames_Fase = pMemento->diretorio_Frames_FaseMemento;
        }
    }
} // Fases