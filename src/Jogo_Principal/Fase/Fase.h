//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

namespace Gerenciadores {
    class Gerenciador_Colisao;
}

namespace Listas {
    class ListaEntidades;
}
namespace Fases {
    class Fase {
        private:
            Listas::ListaEntidades* LEntidades;
            Gerenciadores::Gerenciador_Colisao* gerenciador_colisao;
        protected:
            virtual void criarPlataformas() = 0;
            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarProjetil() = 0;
            void criarCenario();
        public:
            Fase(Gerenciadores::Gerenciador_Colisao* gerenciador);
            ~Fase();
            virtual void executar();

    };
} // Fases

#endif //JOGO_FASE_H