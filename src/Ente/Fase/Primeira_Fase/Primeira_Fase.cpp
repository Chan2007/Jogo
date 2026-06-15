#include "Primeira_Fase.h"

#include <iostream>

#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Inimigo_Medio.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Primeira_Fase::Primeira_Fase() : Fase(), maxInimigos(5) {
        Primeira_Fase::criarCenario();
        Primeira_Fase::criarObstaculos();
        Primeira_Fase::criarInimigos();
        Primeira_Fase::criarProjeteis();
        criarJogadores();
        criarInimMedios();
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

        renderizar();
        definirLimitesJanela();
    }

    // Renderiza para a janela
    void Primeira_Fase::renderizar() {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas(gerenciadorGrafico->getJanela());

    }
    
    void Primeira_Fase::criarInimMedios() {
        Inimigo_Medio* azulo = NULL;
        sementear();
        const int fator = static_cast<int>(gerar_num_exp(1, 5, 0.5));
        for (int i = 0; i < fator; i++) {
            azulo = new Inimigo_Medio();
            if (azulo) {
                azulo->setPosicao(sf::Vector2f((rand() % (tamanhoJanela.y - 300)) + 300, rand() % tamanhoJanela.y));
                gerenciadorColisao->incluirEntidade(azulo);
                gerenciadorGravidade.aplicarGravidade(azulo, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(azulo));
            }
        }
        azulo = NULL;
    }
}
