//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_OBSERVADOR_JOGADOR_H
#define JOGO_OBSERVADOR_JOGADOR_H
#include <string>

namespace Gerenciadores {

    class Tecla {
        public:
            std::string acao;
            bool pressionada;
    };

    class Observador_Input {
        private:
            std::string lastAcao;
            bool wasPressionada;
        public:
            Observador_Input(){};
            virtual ~Observador_Input() {}
            void processarInput(const Tecla& evento) {
                // Evita "tempestade de ações"
                if (evento.acao == lastAcao && evento.pressionada && wasPressionada) return;
                lastAcao = evento.acao;
                wasPressionada = evento.pressionada;

                aoApertarTecla(evento);
            }
            virtual void aoApertarTecla(const Tecla& evento) = 0;
    };

} // namespace Sistemas


#endif //JOGO_OBSERVADOR_JOGADOR_H