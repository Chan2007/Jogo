#include "Primeira_Fase.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Primeira_Fase::Primeira_Fase() : Fase(), maxInimigos(5) {
        // Inicializa o cenário apenas uma vez na criação da fase
        Primeira_Fase::criarCenario();

        Primeira_Fase::criarObstaculos();
        Primeira_Fase::criarInimigos();
        Primeira_Fase::criarProjeteis();
        criarJogadores();
        criarInimMedios();
    }

    void Primeira_Fase::criarCenario() {
        const sf::RenderWindow& janela = gerenciadorGrafico->getJanela();
        const sf::VideoMode tamanhoJanela = sf::VideoMode(janela.getSize().x, janela.getSize().y);

        diretorio_Frames_Fase = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase1");
        diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

        gerenciadorGrafico->setSize(tamanhoJanela);

        if (!diretorio_Frames_Fase.empty())
            Gerenciadores::Gerenciador_Grafico::getGerenciador().loadAnimation(diretorio_Frames_Fase,"bg_fase1_",376,2,4,3);

        if (!diretorio_Frames_Fase.empty())
            gerenciadorGrafico->loadAnimation(diretorio_Frames_Fase, "bg_fase1_", 376, 1, 4, 3);

        if (!diretorio_Audio.empty())
            trocarMusica(1);
    }

    // Inputs específicos da Fase 1
    void Primeira_Fase::processarEventos(const sf::Event& evento) {
        if (evento.type == sf::Event::KeyPressed) {
            // TODO -> Ex.: Se pressionar ESC, o Jogo pausa
        }
    }

    // Evolução da física/lógica no frame atual
    void Primeira_Fase::atualizar(const float dt) {
        gerenciadorGrafico->updateAnimation();
        
        // Executa os gerenciadores de física usando o delta time recebido do Jogo
        gerenciadorColisao.executar(gerenciadorGrafico->getGerenciador().getJanela().getSize(), &gerenciadorGravidade);
        gerenciadorGravidade.executar(dt);
    }

    // Renderiza para a janela
    void Primeira_Fase::renderizar(sf::RenderWindow& janela) {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas(janela);
    }

    void Primeira_Fase::criarJogadores() {
        gerenciadorColisao.incluirEntidade(&jogador);

        jogador.setCampeao(Personagens::CAMPEAO_NAAFIRI);
        jogador.setPosicao(sf::Vector2f(100.f, 800.f));
        std::cout << "Jogador criado: " << jogador.getNome() << std::endl;

        gerenciadorGravidade.aplicarGravidade(&jogador, true);
        LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(&jogador));
    }
}
