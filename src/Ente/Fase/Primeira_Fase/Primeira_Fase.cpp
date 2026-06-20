#include "Primeira_Fase.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Obstaculo_Medio/Portal.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Azulo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Primeira_Fase::Primeira_Fase() : Fase(), maxInimMedios(5) {
        Primeira_Fase::criarCenario();

        if (!jogo->getCarregandoSave()) {
            Primeira_Fase::criarObstaculos();
            Primeira_Fase::criarInimigos();
        }
    }

    void Primeira_Fase::criarCenario() {

        diretorio_Frames_Fase = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase1");
        diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

        if (!diretorio_Frames_Fase.empty())
            gerenciadorGrafico->loadAnimation(diretorio_Frames_Fase,"bg_fase1_",376,2,4,3);

        if (!diretorio_Audio.empty())
            trocarMusica(1);
    }

    // Inputs específicos da Fase 1
    void Primeira_Fase::processarEventos(const sf::Event& evento) {
        gerenciadorInput.notificarObservadores(evento);
    }

    // Evolução da física/lógica no frame atual
    void Primeira_Fase::executar() {
        LEntidades.percorrer();

        gerenciadorGrafico->updateAnimation();

        gerenciadorGravidade.executar();
        gerenciadorColisao->executar();
        definirLimitesJanela();
    }

    // Renderiza para a janela
    void Primeira_Fase::desenhar() {
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
    
    void Primeira_Fase::criarInimMedios() {
        Azulo* azulo = NULL;
        sementear();
        const int fator = static_cast<int>(gerar_num_exp(3, maxInimMedios, 2));
        for (int i = 1; i <= fator; i++) {
            azulo = new Azulo();
            if (azulo) {
                azulo->setPosicao(sf::Vector2f(350*i, rand() % tamanhoJanela.y));
                gerenciadorColisao->incluirEntidade(azulo);
                gerenciadorGravidade.aplicarGravidade(azulo, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(azulo));
            }
        }
        azulo = NULL;
    }
    void Primeira_Fase::criarObstMedios() {

        Obstaculos::Portal* portal = NULL;
        sementear();

        const int fator = gerar_num_norm(3, 0.75, 3, 6);

        for (int i = 1; i <= fator; i++) {
            portal = new Obstaculos::Portal();
            if (portal) {

                portal->setPosicao(sf::Vector2f(350 * i, (rand() % tamanhoJanela.y - 300) + 300));
                gerenciadorColisao->incluirEntidade(portal);
                gerenciadorGravidade.aplicarGravidade(portal, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(portal));
            }
        }
        portal = NULL;
    }

}
