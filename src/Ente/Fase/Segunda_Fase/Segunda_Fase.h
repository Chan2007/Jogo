//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_SEGUNDA_FASE_H
#define JOGO_SEGUNDA_FASE_H
#include "jogo.h"
#include "Ente/Fase/Fase.h"

namespace Fases {
    class Segunda_Fase: public Fase {
        private:
            int maxChefoes;
            std::string diretorio_Frames_Fase;

            void processarEventos(const sf::Event &evento);
            void desenhar();

            class Segunda_FaseMemento: public FaseMemento {
                private:
                    int maxChefoesMemento;
                    std::string diretorio_Frames_FaseMemento;
                    explicit Segunda_FaseMemento(const Segunda_Fase& f) :
                    FaseMemento(f), maxChefoesMemento(f.maxChefoes),
                    diretorio_Frames_FaseMemento(f.diretorio_Frames_Fase) {}

                    ~Segunda_FaseMemento() {}
                    friend class Segunda_Fase; // Permite ao pai acessar os dados privados
            };
        protected:
            void criarObstaculos() {
                criarObstDificeis();

            }
            void criarInimigos() {
                criarChefoes();
            }
            void criarProjeteis();
            void criarCenario();

            void criarObstDificeis();

            void criarChefoes();
        public:
            Segunda_Fase(Jogo* pJogo,  const std::string& nomeJ1 = "", const QString& campeaoJ1 = "",
                         const std::string& nomeJ2 = "", const QString& campeaoJ2 = "", bool jogador2Ativo = false);
            ~Segunda_Fase();

            Gerenciadores::Memento *salvarMemento() const;
            void restaurarMemento(const Gerenciadores::Memento *memento);

            void executar();

    };
} // Fases

#endif //JOGO_SEGUNDA_FASE_H