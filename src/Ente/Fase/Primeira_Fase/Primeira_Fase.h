
#ifndef JOGO_PRIMEIRA_FASE_H
#define JOGO_PRIMEIRA_FASE_H

#include "jogo.h"
#include "Ente/Fase/Fase.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"

class Encontrar_Caminho;

namespace Fases {
    class Primeira_Fase: public Fase {
        private:
            const int maxInimMedios;

            void processarEventos(const sf::Event &evento);
            void desenhar();

            std::string diretorio_Frames_Fase;
        protected:

            void criarObstaculos() {
                criarObstMedios();
            }
            void criarInimigos() {
                criarInimMedios();
            }
            void criarCenario();

            void criarInimMedios();
            void criarObstMedios();

        public:
            Primeira_Fase();
            ~Primeira_Fase(){};
            void executar();
        };
} // Fases

#endif //JOGO_PRIMEIRA_FASE_H