//
// Created by Henrique on 09/05/2026.
//

#include "Segunda_Fase.h"

#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Segunda_Fase::Segunda_Fase() : Fase(), maxChefoes(2) {
        // Inicializa o cenário apenas uma vez na criação da fase
        Segunda_Fase::criarCenario();

        Segunda_Fase::criarObstaculos();
        Segunda_Fase::criarInimigos();
        Segunda_Fase::criarProjeteis();
    }

    Segunda_Fase::~Segunda_Fase() {}

    void Segunda_Fase::criarCenario() {
        sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();
        const sf::Vector2u tamanhoJanela = janela.getSize();

        diretorio_Frames_Fase = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase2");
        diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

        animadorFase.setSheetTargetSize(tamanhoJanela);

        if (!diretorio_Frames_Fase.empty()) {
            animadorFase.loadFrames(diretorio_Frames_Fase, "bg_fase2_", 451, 2, 4, 7);
        }

        if (!diretorio_Audio.empty()) {
            trocarMusica(2);
        }
    }

    // 1. Cuidar apenas dos inputs específicos da Fase 1
    void Segunda_Fase::processarEventos(const sf::Event& evento) {
        if (evento.type == sf::Event::KeyPressed) {
            // Exemplo: Se pressionar ESC, o Jogo poderá capturar para pausar
        }
    }

    // 2. Cuidar apenas da evolução da física/lógica no frame atual
    void Segunda_Fase::atualizar(const float dt) {
        animadorFase.update();

        // Executa gerenciadores de física usando o delta time recebido do Jogo
        gerenciadorColisao.executar(Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela().getSize(), &gerenciadorGravidade);
        gerenciadorGravidade.executar(dt);
    }

    // 3. Cuidar apenas de mandar os elementos para a janela
    void Segunda_Fase::renderizar(sf::RenderWindow& janela) {
        animadorFase.draw(janela);
        LEntidades.desenharTodas(janela);
    }
} // Fases