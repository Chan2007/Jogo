#include "main.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

#include "Animador_Fundo/animador_fundo.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Jogo LoL");

    BackgroundAnimator bg;
    const int totalFrames = 376;
    const int frameStep = 2;
    const char* framePaths[] = {
        "assets/bg_frames/",
        "../assets/bg_frames/",
        "../../assets/bg_frames/",
        "../../../assets/bg_frames/"
    };
    const int framePathCount = sizeof(framePaths) / sizeof(framePaths[0]);
    bool loaded = false;

    for (int i = 0; i < framePathCount; ++i) {
        if (bg.loadFrames(framePaths[i], totalFrames, 1, frameStep)) {
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        std::cerr << "Nao foi possivel carregar os frames de fundo a partir dos caminhos configurados." << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        bg.update();
        bg.draw(window);
        window.display();
    }

    return 0;
}
