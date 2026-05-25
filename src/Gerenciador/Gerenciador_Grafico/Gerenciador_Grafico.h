//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_GRAFICO_H
#define JOGO_GERENCIADOR_GRAFICO_H
#include <SFML/Graphics.hpp>
using namespace sf;
namespace Gerenciadores {
    class gerenciador_grafico {
    private:
        Sprite sprite;
        RenderWindow window;
        gerenciador_grafico();
        // Proibir o uso de construtora de cópia e atribuição à cópias
        gerenciador_grafico(const gerenciador_grafico&);
        gerenciador_grafico& operator=(const gerenciador_grafico&);
    public:
        ~gerenciador_grafico();
        gerenciador_grafico& get_grafico();
        RenderWindow& get_window();
        bool isOpen() const;
        void clear();
        void draw(const RectangleShape& retangulo);
        void draw();
        void show();
        void close();

    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_GRAFICO_H