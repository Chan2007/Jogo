#include "Primeira_Fase.h"

#include <iostream>

#include "Ente/Entidade/Obstaculo/Obstaculo_Medio/Portal.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Azulo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Estado/Gerenciador_Estado.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Fases {
    Primeira_Fase::Primeira_Fase() : Fase(), maxInimMedios(5) {
        Primeira_Fase::criarCenario();
        if (jogo->getJogador1())
            jogo->getJogador1()->setPosicao(sf::Vector2f(50.0f, 50.0f));

        if (jogo->getJogador2Ativo() && jogo->getJogador2())
            jogo->getJogador2()->setPosicao(sf::Vector2f(100.0f, 50.0f));

        if (!jogo->getCarregandoSave()) {
            Primeira_Fase::criarObstaculos();
            Primeira_Fase::criarInimigos();
        }
    }

    void Primeira_Fase::criarCenario() {
        // -------------------------------------------------------------------------
        // ATRIBUIÇÃO DE ATIVOS (LEAGUE DISPLAYS)
        // Os vídeos utilizados nesta tela são de propriedade da Riot Games, Inc.
        // Uso não comercial permitido para fins educacionais e comunitários.
        // -------------------------------------------------------------------------
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
                else if (it->getNome() == "Minion" || it->getNome() == "Azulo")
                    existemInimigos = true;
            }
            ++it;
        }

        if (!jogador1Vivo && !jogador2Vivo) {
            Gerenciadores::Gerenciador_Estado::getGerenciador().notificar(Gerenciadores::EVENTO_JOGADOR_MORREU);
            return; // Aborta o resto para evitar erros de ponteiro nulo
        }
        if (!existemInimigos) {
            Gerenciadores::Gerenciador_Estado::getGerenciador().notificar(Gerenciadores::EVENTO_FASE1_CONCLUIDA);
        }
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
        Personagens::Azulo* azulo = NULL;
        sementear();
        const int fator = static_cast<int>(gerar_num_exp(3, maxInimMedios, 2));
        for (int i = 1; i <= fator; i++) {
            azulo = new Personagens::Azulo();
            if (azulo) {
                azulo->setPosicao(sf::Vector2f(350*i, rand() % tamanhoJanela.height));
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
                portal->setPosicao(sf::Vector2f(350 * i, (rand() % tamanhoJanela.height - 300) + 300));
                gerenciadorColisao->incluirEntidade(portal);
                gerenciadorGravidade.aplicarGravidade(portal, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(portal));
            }
        }
        portal = NULL;
    }
    Memento* Primeira_Fase::salvarMemento() const {
        return new Primeira_FaseMemento(*this);
    }

    void Primeira_Fase::restaurarMemento(const Memento* memento) {
        Fase::restaurarMemento(memento);
        const Primeira_FaseMemento* pMemento = dynamic_cast<const Primeira_FaseMemento*>(memento);
        if (pMemento) {
            maxInimMedios = pMemento->maxInimMediosMemento;
            diretorio_Frames_Fase = pMemento->diretorio_Frames_FaseMemento;
        }
    }

}
