//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Listas/ListaEntidades.h"
#include "Sistema/UI/animador_fundo.h"


namespace Gerenciadores {
    class Gerenciador_Colisao;
}
namespace Listas {
    class ListaEntidades;
}
namespace Fases {
    class Fase {
        protected:
            Listas::ListaEntidades LEntidades;
            Gerenciadores::Gerenciador_Colisao* gerenciador_colisao;
            Gerenciadores::Gerenciador_Textura gerenciadorTextura;
            Animador_Fundo animadorFase1;
            Animador_Fundo animadorFase2;

            virtual void criarPlataformas() = 0;
            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarProjetil() = 0;
            virtual void criarCenario() = 0;
            void criarJogadores();

        public:
            explicit Fase(Gerenciadores::Gerenciador_Colisao* gerenciador);
            ~Fase();
            virtual void executar();

    };
} // Fases

#endif //JOGO_FASE_H