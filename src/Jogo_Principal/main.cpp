#include "main.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

#include "Diretorio/find_Directory.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"

int computeFrameStep(const int totalFrames, int preferredStep, const int maxFramesToLoad) {
    if (preferredStep <= 0) {
        preferredStep = 1;
    }

    if (maxFramesToLoad <= 0 || totalFrames <= maxFramesToLoad) {
        return preferredStep;
    }

    const int requiredStep = (totalFrames + maxFramesToLoad - 1) / maxFramesToLoad;
    return preferredStep > requiredStep ? preferredStep : requiredStep;
}

int main() {
    const int totalFrames = 376;
    int pulodeFrames = 0;
    int maximodeFrames = 0;
    while (true) {
        std::cout << "Quantos frames intercalar: ";
        // Obs.: Se pulodeFrames for muito grande, pode ser que a qualidade do cenario caia.
        if (std::cin >> pulodeFrames && pulodeFrames > 0 && pulodeFrames < totalFrames)
            break;
        std::cout << "Entrada invalida! Digite um numero positivo." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    while (true) {
        std::cout << "Maximo de frames a rodar: ";
        // Obs.: Se maxideFrames for muito pequeno, pode ser que a qualidade do cenario caia.
        if (std::cin >> maximodeFrames && maximodeFrames > 0 && maximodeFrames < totalFrames)
            break;
        std::cout << "Entrada invalida! Digite um numero positivo." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Fullscreen);
    Animador_Fundo bgAnimation;
    bgAnimation.setTargetSize(window.getSize());

    Find_Directory directoryFinder;

    const int frameStep = computeFrameStep(totalFrames, pulodeFrames, maximodeFrames);
    const std::string frameDirectory = directoryFinder.findFolderDirectory("assets/bg_frames/");
    const bool loaded = !frameDirectory.empty() && bgAnimation.loadFrames(frameDirectory, totalFrames, 1, frameStep);
    if (!loaded) {
        std::cerr << "Nao foi possivel localizar a pasta assets ou carregar os frames de fundo." << std::endl;
        return -1;
    }

    const std::string audioDirectory = directoryFinder.findFolderDirectory("assets/bg_audios/bg_music");
    Audio bgMusic;
    if (!audioDirectory.empty()) {
        std::string separador = (audioDirectory.back() == '/' || audioDirectory.back() == '\\') ? "" : "/";
        const std::string musicPath = audioDirectory + separador + "Aurora_s-Theme.ogg";
        if (bgMusic.loadMusic(musicPath)) {
            bgMusic.setVolume(50.0f);  // 50% de volume
            bgMusic.play();
            bgMusic.setLoop(true);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        bgAnimation.update();
        bgAnimation.draw(window);
        window.display();
    }

    return 0;
}