//
// Created by Henrique on 16/06/2026.
//

#ifndef JOGO_GERENCIADOR_ESTADO_H
#define JOGO_GERENCIADOR_ESTADO_H

class Jogo;

namespace Gerenciadores {

    enum EventoJogo {
        EVENTO_JOGADOR_MORREU,
        EVENTO_FASE1_CONCLUIDA,
        EVENTO_FASE2_CONCLUIDA
    };

    class Gerenciador_Estado {
    private:
        Jogo* pJogo;

        // Construtor privado para Singleton (opcional, ou passe a instância do Jogo)
        Gerenciador_Estado();

        Gerenciador_Estado(Gerenciador_Estado const&); // Proibir cópia
        void operator=(Gerenciador_Estado const&); // Proibir atribuição
    public:
        ~Gerenciador_Estado();

        static Gerenciador_Estado& getGerenciador();

        void setJogo(Jogo* jogo);
        Jogo* getJogo() { return pJogo; }

        // Padrão Mediator
        void notificar(EventoJogo evento);
    };

} // Gerenciadores

#endif //JOGO_GERENCIADOR_ESTADO_H