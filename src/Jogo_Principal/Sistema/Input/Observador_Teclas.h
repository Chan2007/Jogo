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

    class Observador_Teclas {
        public:
            virtual ~Observador_Teclas() {}
            virtual void aoApertarTecla(const Tecla& evento) = 0;
    };

} // namespace Sistemas


#endif //JOGO_OBSERVADOR_JOGADOR_H