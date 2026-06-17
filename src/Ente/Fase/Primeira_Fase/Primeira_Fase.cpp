#include "Primeira_Fase.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Obstaculo_Medio/Obstaculo_Medio.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Inimigo_Medio.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Primeira_Fase::Primeira_Fase() : Fase(), maxInimMedios(5) {
        Primeira_Fase::criarCenario();
        Primeira_Fase::criarObstaculos();
        Primeira_Fase::criarInimigos();
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
        LEntidades.executarTodas();

        gerenciadorGrafico->updateAnimation();

        gerenciadorGravidade.executar();
        gerenciadorColisao->executar();
        definirLimitesJanela();
    }

    // Renderiza para a janela
    void Primeira_Fase::desenhar() {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas(gerenciadorGrafico->getJanela());
    }
    
    void Primeira_Fase::criarInimMedios() {
        Inimigo_Medio* azulo = NULL;
        sementear();
        const int fator = static_cast<int>(gerar_num_exp(1, maxInimMedios, 2));
        for (int i = 0; i < fator; i++) {
            azulo = new Inimigo_Medio();
            if (azulo) {
                azulo->setPosicao(sf::Vector2f(rand() % tamanhoJanela.y, rand() % tamanhoJanela.y));
                gerenciadorColisao->incluirEntidade(azulo);
                gerenciadorGravidade.aplicarGravidade(azulo, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(azulo));
            }
        }
        azulo = NULL;
    }
    void Primeira_Fase::criarObstMedios() {

        Obstaculos::Obstaculo_Medio* portal = NULL;
        sementear();

        const int fator = gerar_num_norm(3, 0.75, 0, 5);

        for (int i = 0; i <= fator; i++) {
            portal = new Obstaculos::Obstaculo_Medio();
            if (portal) {

                portal->setPosicao(sf::Vector2f(350 * i, (rand() % tamanhoJanela.y - 300) + 300));
                gerenciadorColisao->incluirEntidade(portal);
                gerenciadorGravidade.aplicarGravidade(portal, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(portal));
            }
        }
        portal = NULL;
        /*
        Obstaculos::Obstaculo_Dificil* Pinstouro = NULL;
        for (int i = 1; i <= 3; i++) {
            Pinstouro = new Obstaculos::Obstaculo_Dificil();
            if (Pinstouro) {
                Pinstouro->setPosicao(sf::Vector2f(300 * i, (rand() % desktop.height - 300) + 300));
                gerenciadorColisao.incluirEntidade(Pinstouro);
                gerenciadorGravidade.aplicarGravidade(Pinstouro, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(Pinstouro));
            }
            Pinstouro = NULL;
        }
        */
    }

}
