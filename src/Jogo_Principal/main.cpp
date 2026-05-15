#include "main.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include "Diretorio/Encontrar_Diretorio.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"

using namespace std;

int main() {
    const int totalFrames = 376;
    const int intercalarFrames = 1;

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Default);
    window.setFramerateLimit(60);

    Gerenciador_Textura gerenciadorTextura;
    Animador_Fundo bgAnimation(&gerenciadorTextura);
    bgAnimation.setPosicao(sf::Vector2f(0.f, 0.f));
    bgAnimation.setTargetSize(window.getSize());

    Encontrar_Diretorio diretorio;

    const string diretorio_Frame = diretorio.acharDiretorio_Arquivo("assets/bg_frames/aumentadas");
    const int colunasSpritesheet = 4;
    const int linhasSpritesheet = 3;
    if (!diretorio_Frame.empty()) {
        if (bgAnimation.loadFrames(diretorio_Frame, totalFrames, 1, intercalarFrames,
                                   colunasSpritesheet, linhasSpritesheet))
            cout << "Frames de background carregados com sucesso!" << endl;
        else {
            cerr << "Falha ao carregar os frames de background. Verifique se os arquivos estao corretos." << endl;
            return -1;
        }
    }
    else {
        cerr << "Nao foi possivel localizar o diretorio." << endl;
        return -1;
    }

    Audio bgMusic;
    const string diretorio_Audio = diretorio.acharDiretorio_Arquivo("assets/bg_audios/bg_music");
    if (!diretorio_Audio.empty()) {
        const string diretorio_Musica = Encontrar_Diretorio::concatenarEnderecos(diretorio_Audio, "Aurora_s-Theme.ogg");
        if (bgMusic.loadMusic(diretorio_Musica)) {
            bgMusic.setVolume(0.0f);
            bgMusic.play();
            bgMusic.setLoop(true);
        }
        else {
            cerr << "Nao foi possivel carregar a musica." << endl;
        }
    }
    else {
        cerr << "Nao foi possivel localizar a pasta de audio. A musica de fundo nao sera reproduzida." << endl;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                const sf::Vector2u novoTamanho(event.size.width, event.size.height);
                bgAnimation.setTargetSize(novoTamanho);
                bgAnimation.setPosicao(sf::Vector2f(0.f, 0.f));
            }
        }

        window.clear();
        bgAnimation.update();
        bgAnimation.draw(window);

        sf::RectangleShape player(sf::Vector2f(40.f, 40.f));
        player.setFillColor(sf::Color::Green);
        player.setPosition(100.f, 50.f);
        window.draw(player);

        window.display();
    }

    return 0;
}
