// Created by Henrique on 16/06/2026.
//

#include "Gerenciador_Estado.h"
#include "jogo.h"
#include <iostream>

namespace Gerenciadores {

    Gerenciador_Estado::Gerenciador_Estado() : pJogo(NULL) {}

    Gerenciador_Estado::~Gerenciador_Estado() {
        pJogo = NULL;
    }

    Gerenciador_Estado& Gerenciador_Estado::getGerenciador() {
        static Gerenciador_Estado instancia;
        return instancia;
    }

    void Gerenciador_Estado::setJogo(Jogo* jogo) {
        pJogo = jogo;
    }

    void Gerenciador_Estado::notificar(EventoJogo evento) {
        if (!pJogo) return;

        switch (evento) {
            case EVENTO_JOGADOR_MORREU:
                std::cout << "[Mediator] Jogador morreu! Redirecionando para o Menu Principal..." << std::endl;
                pJogo->salvarJogoAtual("save.txt"); // Salva o progresso antes de mudar de fase
                pJogo->mudarEstado(Jogo::TelaMenu); // Ou um estado de Game Over, não?
                break;

            case EVENTO_FASE1_CONCLUIDA:
                std::cout << "[Mediator] Fase 1 concluida! Avancando para a Fase 2..." << std::endl;
                pJogo->salvarJogoAtual("save.txt"); // Salva o progresso antes de mudar de fase
                pJogo->mudarEstado(Jogo::TelaFase2);
                break;

            case EVENTO_FASE2_CONCLUIDA:
                std::cout << "[Mediator] Fase 2 concluida! Fim de jogo. Voltando ao Menu..." << std::endl;
                pJogo->mudarEstado(Jogo::TelaMenu);
                break;

            default:
                break;
        }
    }

} // Gerenciadoresores