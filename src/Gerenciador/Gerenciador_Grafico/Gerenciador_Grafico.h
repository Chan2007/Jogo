//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_GRAFICO_H
#define JOGO_GERENCIADOR_GRAFICO_H
#include <SFML/Graphics.hpp>
using namespace sf;
namespace Gerenciadores {
    class Gerenciador_Grafico {
    private:
        Sprite sprite;
        RenderWindow window;
        Gerenciador_Grafico();
        // Proibir o uso de construtora de cópia e atribuição à cópias
        Gerenciador_Grafico(const Gerenciador_Grafico&);
        Gerenciador_Grafico& operator=(const Gerenciador_Grafico&);
    public:
        ~Gerenciador_Grafico();
        static Gerenciador_Grafico& get_grafico();
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