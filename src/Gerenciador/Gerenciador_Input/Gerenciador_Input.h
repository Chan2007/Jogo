//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_GERENCIADOR_TECLAS_H
#define JOGO_GERENCIADOR_TECLAS_H

#include <SFML/Window.hpp>
#include <vector>
#include "Sistema/Input/Observador_Input.h"
#include "Sistema/Input/Mapeador_Input.h"

namespace Gerenciadores {
    class Tecla;
    class Mapeador_Input;
    class Gerenciador_Input {
        private:
            std::vector <Observador_Input*> observadores;
            Mapeador_Input mapeador;
            Gerenciador_Input();
            Gerenciador_Input(const Gerenciador_Input&);
            Gerenciador_Input& operator=(const Gerenciador_Input&);
        public:
            static Gerenciador_Input& getGerenciador() {
                static Gerenciador_Input gerenciador;
                return gerenciador;
            }
            ~Gerenciador_Input(){
                for (int i = 0; i < observadores.size(); i++) {
                    if (observadores[i]) delete observadores[i];
                }
                observadores.clear();
            }

            // Padrão observer
            void inscrever(Observador_Input* obs){if (obs) observadores.push_back(obs); }
            void desinscrever(Observador_Input* obs) {
                if (obs)
                    observadores.erase(std::remove(observadores.begin(), observadores.end(), obs), observadores.end());
            }
            void notificarObservadores(const sf::Event& evento) const;

            Mapeador_Input* getMapeador() {return &mapeador;}

    };
} // Sistema

#endif //JOGO_GERENCIADOR_TECLAS_H