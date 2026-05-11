//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_GERENCIADOR_TECLAS_H
#define JOGO_GERENCIADOR_TECLAS_H

#include <SFML/Window.hpp>
#include <vector>
#include "Jogo_Principal/Sistema/Input/Observador_Teclas.h"
#include "Jogo_Principal/Sistema/Input/Mapeador_Teclas.h"

namespace Gerenciadores {
    class Tecla;
    class Mapeador_Teclas;
    class Gerenciador_Input {
        private:
            std::vector <Observador_Teclas*> observadores;
            Mapeador_Teclas mapeador;

        public:
            Gerenciador_Input();
            ~Gerenciador_Input();

            // Padrão observer
            void inscrever(Tecla* obs);
            void desinscrever(Tecla* obs);
            void notificarObservadores(const sf::Event& evento) const;

            Mapeador_Teclas* getMapeador() {return &mapeador;}

    };
} // Sistema

#endif //JOGO_GERENCIADOR_TECLAS_H