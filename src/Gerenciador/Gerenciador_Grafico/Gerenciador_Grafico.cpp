//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Grafico.h"

namespace Gerenciadores {
    Gerenciador_Grafico::Gerenciador_Grafico() {
        sf::RenderWindow window(tamanho, "Jogo LoL", sf::Style::Default);
        window.setFramerateLimit(60);
    }
    Gerenciador_Grafico& Gerenciador_Grafico::get_grafico() {
        static Gerenciador_Grafico instancia;
        return instancia;
    }

} // Gerenciador