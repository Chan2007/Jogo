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
        Primeira_Fase::executar();
    }

    void Primeira_Fase::criarCenario() {
        const sf::RenderWindow& janela = gerenciadorGrafico->getJanela();
        const sf::VideoMode tamanhoJanela = sf::VideoMode(janela.getSize().x, janela.getSize().y);

        diretorio_Frames_Fase = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase1");
        diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

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
    void Primeira_Fase::executar() {
        const float dt = 0.016f;

        LEntidades.executarTodas();
        if (Entidades::Entidade::getListaEntidades()) { Entidades::Entidade::getListaEntidades()->executarTodas(); }

        gerenciadorGrafico->updateAnimation();

        sf::Vector2u tamanhoAtual(desktop.width, desktop.height);

        gerenciadorGravidade.executar(dt);
        gerenciadorColisao.executar(tamanhoAtual, &gerenciadorGravidade);

        renderizar(Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela());
    }

    // Renderiza para a janela
    void Primeira_Fase::renderizar(sf::RenderWindow& janela) {
        gerenciadorGrafico->drawAnimation();
        LEntidades.desenharTodas(janela);
        if (Entidades::Entidade::getListaEntidades()) { Entidades::Entidade::getListaEntidades()->desenharTodas(janela); }
    }

    void Primeira_Fase::criarJogadores() {

        jogador.setCampeao(Personagens::CAMPEAO_NAAFIRI);
        jogador.setPosicao(sf::Vector2f((jogador.getTamanho().width)/2, desktop.height - (jogador.getTamanho().height)/2));
        std::cout << "Jogador criado: " << jogador.getNome() << std::endl;

        jogador.setGerenciadorGravidade(&gerenciadorGravidade);
        gerenciadorColisao.incluirEntidade(&jogador);
        gerenciadorGravidade.aplicarGravidade(&jogador, true);
        LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(&jogador));
        Personagens::Inimigo::incluirJogador(&jogador);
    }
    
    void Primeira_Fase::criarInimMedios() {
        Inimigo_Medio* azulo = NULL;
        Ente::sementear();
        const int fator = rand() % 3 + 3;
        for (int i = 0; i < fator; i++) {
            azulo = new Inimigo_Medio();
            if (azulo) {
                azulo->setPosicao(sf::Vector2f((rand() % (desktop.width - 300)) + 300, rand() % desktop.height));
                gerenciadorColisao.incluirEntidade(azulo);
                gerenciadorGravidade.aplicarGravidade(azulo, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(azulo));
            }
        }
        azulo = NULL;
    }
}
