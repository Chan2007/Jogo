//
// Created by Henrique on 09/05/2026.
//

#include "Gerenciador_Input.h"
#include "Sistema/Input/Observador_Input.h"

namespace Gerenciadores {
    class Tecla;
}
namespace Gerenciadores {
    Gerenciador_Input::Gerenciador_Input() : observadores(), mapeador() {}

    void Gerenciador_Input::notificarObservadores(const sf::Event& evento) const {
        std::string acaoFinal = "";
        bool pressionada = false;

        // TECLADO
        if (evento.type == sf::Event::KeyPressed || evento.type == sf::Event::KeyReleased) {
            acaoFinal = mapeador.getAcao(evento.key.code);
            pressionada = (evento.type == sf::Event::KeyPressed);
        }

        // MOUSE
        else if (evento.type == sf::Event::MouseButtonPressed || evento.type == sf::Event::MouseButtonReleased) {
            acaoFinal = mapeador.getAcao(evento.mouseButton.button);
            pressionada = (evento.type == sf::Event::MouseButtonPressed);
        }

        // BOTÕES DO JOYSTICK
        else if (evento.type == sf::Event::JoystickButtonPressed || evento.type == sf::Event::JoystickButtonReleased) {
            std::string acaoBase = mapeador.getAcao(evento.joystickButton.button); // Retorna "pular", "atacar"
            if (!acaoBase.empty()) {
                int idPlayer = evento.joystickButton.joystickId + 1;
                acaoFinal = "j" + std::to_string(idPlayer) + "_" + acaoBase;
                pressionada = (evento.type == sf::Event::JoystickButtonPressed);
            }
        }

        // Se uma ação foi gerada, notifica os observadores
        if (!acaoFinal.empty()) {
            Tecla e;
            e.acao = acaoFinal;
            e.pressionada = pressionada;
            for (int i = 0; i < static_cast<int>(observadores.size()); i++)
                observadores[i]->processarInput(e);
            return;
        }

        // EIXOS ANALÓGICOS
        if (evento.type == sf::Event::JoystickMoved) {
            if (evento.joystickMove.axis == sf::Joystick::X || evento.joystickMove.axis == sf::Joystick::PovX) {
                std::string acaoBase = mapeador.getAcao(evento.joystickMove.axis);

                if (acaoBase == "mover_horizontal") {
                    int idPlayer = evento.joystickMove.joystickId + 1;
                    float posicao = evento.joystickMove.position;

                    Tecla eEsquerda, eDireita;
                    eEsquerda.acao = "j" + std::to_string(idPlayer) + "_mover_esquerda";
                    eDireita.acao = "j" + std::to_string(idPlayer) + "_mover_direita";

                    if (posicao > 30.f) {
                        eDireita.pressionada = true;
                        eEsquerda.pressionada = false;
                    }
                    else if (posicao < -30.f) {
                        eDireita.pressionada = false;
                        eEsquerda.pressionada = true;
                    }
                    else {
                        eDireita.pressionada = false;
                        eEsquerda.pressionada = false;
                    }

                    for (int i = 0; i < static_cast<int>(observadores.size()); i++) {
                        observadores[i]->processarInput(eDireita);
                        observadores[i]->aoApertarTecla(eEsquerda);
                    }
                }
            }
        }
    }
} // Sistema