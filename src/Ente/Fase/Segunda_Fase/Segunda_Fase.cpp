//
// Created by Henrique on 09/05/2026.
//

#include "Segunda_Fase.h"

#include "Ente/Entidade/Obstaculo/Obstaculo_Dificil/Obstaculo_Dificil.h"
#include "Ente/Entidade/Personagem/Inimigo/Chefe/Chefe.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"



namespace Fases {
    Segunda_Fase::Segunda_Fase() : Fase(), maxChefoes(5) {
        Segunda_Fase::criarCenario();
        Segunda_Fase::criarObstaculos();
        Segunda_Fase::criarInimigos();
        Segunda_Fase::criarProjeteis();
    }

    Segunda_Fase::~Segunda_Fase() {}

    void Segunda_Fase::criarCenario() {
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
        LEntidades.executarTodas();
        gerenciadorGrafico->updateAnimation();

        // Executa gerenciadores de física usando o delta time recebido do Jogo
        gerenciadorGravidade.executar();
        gerenciadorColisao->executar();
        definirLimitesJanela();
    }

    // Cuidar apenas de mandar os elementos para a janela
    void Segunda_Fase::desenhar() {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas(gerenciadorGrafico->getJanela());
    }
    void Segunda_Fase::criarChefoes() {
        Chefe* ElderDragon = NULL;
        Entidades::Projetil* tiroInim = NULL;
        const int fator = gerar_num_exp(3, maxChefoes, 0.5);
        for (int i = 0; i < fator; i++) {
            ElderDragon = new Chefe();
            if (ElderDragon) {
                ElderDragon->setPosicao(
                    sf::Vector2f(
                    gerar_num_norm((tamanhoJanela.x - 300)/2.0, tamanhoJanela.x/6.0,0, static_cast<int>(tamanhoJanela.x - 300)),
                    gerar_num_norm(tamanhoJanela.y/2.0, tamanhoJanela.y/6.0, 0, static_cast<int>(tamanhoJanela.y))
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
        Obstaculos::Obstaculo_Dificil* Pinstouro = NULL;
        for (int i = 1; i <= 3; i++) {
            Pinstouro = new Obstaculos::Obstaculo_Dificil();
            if (Pinstouro) {
                Pinstouro->setPosicao(sf::Vector2f(300 * i, (rand() % tamanhoJanela.y - 300) + 300));
                gerenciadorColisao->incluirEntidade(Pinstouro);
                gerenciadorGravidade.aplicarGravidade(Pinstouro, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(Pinstouro));
            }
            Pinstouro = NULL;
        }
    }
} // Fases