
#ifndef JOGO_PRIMEIRA_FASE_H
#define JOGO_PRIMEIRA_FASE_H

#include "jogo.h"
#include "Ente/Fase/Fase.h"
#include "Ente/Fase/Fase.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"

class Encontrar_Caminho;

namespace Fases {
    class Primeira_Fase: public Fase {
        private:
            int maxInimMedios;
            std::string diretorio_Frames_Fase;
            void processarEventos(const sf::Event &evento);
            void desenhar();

            class Primeira_FaseMemento: public FaseMemento {
                private:
                    const int maxInimMediosMemento;
                    std::string diretorio_Frames_FaseMemento;
                    explicit Primeira_FaseMemento(const Primeira_Fase& f) :
                    FaseMemento(f), maxInimMediosMemento(f.maxInimMedios),
                    diretorio_Frames_FaseMemento(f.diretorio_Frames_Fase) {}

                    ~Primeira_FaseMemento() {}
                    friend class Primeira_Fase;
            };
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
            Primeira_Fase(Jogo* pJogo,  const std::string& nomeJ1 = "", const QString& campeaoJ1 = "",
                          const std::string& nomeJ2 = "", const QString& campeaoJ2 = "", bool jogador2Ativo = false);
            ~Primeira_Fase(){}

            Gerenciadores::Memento* salvarMemento() const;
            void restaurarMemento(const Gerenciadores::Memento *memento);

            void executar();
        };
} // Fases

#endif //JOGO_PRIMEIRA_FASE_H