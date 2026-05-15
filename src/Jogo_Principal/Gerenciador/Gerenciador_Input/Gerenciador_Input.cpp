//
// Created by Henrique on 09/05/2026.
//

#include "Gerenciador_Input.h"
#include "Jogo_Principal/Sistema/Input/Observador_Input.h"

namespace Gerenciadores {
    class Tecla;
}
namespace Gerenciadores {
    void Gerenciador_Input::notificarObservadores(const sf::Event& evento) const {
        if (evento.type == sf::Event::KeyPressed) {
            std::string acao = mapeador.getAcao(evento.key.code);
            if (!acao.empty()) {
                Tecla e;
                e.acao = acao;
                e.pressionada = true;
                for (int i = 0; i < static_cast<int>(observadores.size()); i++) observadores[i]->aoApertarTecla(e);
            }
        }

        else if (evento.type == sf::Event::MouseButtonPressed) {}
        else if (evento.type == sf::Event::JoystickButtonPressed) {}
    }

} // Sistema