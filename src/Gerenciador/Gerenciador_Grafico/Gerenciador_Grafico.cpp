//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Grafico.h"

namespace Gerenciadores {
    Gerenciador_Grafico::Gerenciador_Grafico():
        sprite(), window(),
        tamanho(), gerenciadorTextura(new Gerenciador_Textura()),
        animadorFundo(gerenciadorTextura)
    {
        window.setFramerateLimit(60);
    }
    Gerenciador_Grafico& Gerenciador_Grafico::getGerenciador() {
        static Gerenciador_Grafico instancia;
        return instancia;
    }
    bool Gerenciador_Grafico::loadAnimation(const std::string& prefix, const std::string& name,
                                         const int numFrames, const int frameStep,
                                         const unsigned int cols, const unsigned int rows) {
        if (!window.isOpen()) return false;
        animadorFundo.setSheetTargetSize(sf::Vector2u(tamanho.width, tamanho.height));
        animadorFundo.setSheetPosition(position);
        return animadorFundo.loadFrames(prefix, name, numFrames, frameStep, cols, rows);
    }

    void Gerenciador_Grafico::updateAnimation() {
        animadorFundo.update();
    }

    void Gerenciador_Grafico::drawAnimation() {
        animadorFundo.draw(window);
    }

} // Gerenciador