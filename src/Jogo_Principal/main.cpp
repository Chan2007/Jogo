#include "main.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <cmath>
#include "Diretorio/Encontrar_Diretorio.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"
using namespace std;

int checarIntercalo(const int totalFrames, int step, const int frames) {
    const int min_step = ceil(static_cast<double>(totalFrames)/ frames);
    return (step > min_step ? step : min_step);
}

int main() {
    const int totalFrames = 376;
    const int max_intercalarFrames = 5;
    int intercalarFrames = 0;
    int Frames = 0;
    while (true) {
        cout << "Quantos frames intercalar: " << "(maximo recomendado: " << max_intercalarFrames << ")" << endl;
        // Obs.: Se intercalarFrames for muito grande, a qualidade do cenário irá cair.
        if (cin >> intercalarFrames && intercalarFrames > 0 && intercalarFrames <= totalFrames) break;
        cout << "Entrada inválida! Digite um número positivo." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }

    while (true) {
        cout << "Maximo de frames a rodar: " << "(maximo: " << totalFrames << ")" << endl;
        // Obs.: Se maxFrames for muito pequeno, a qualidade do cenario irá dessincronizar.
        if (cin >> Frames && Frames > 0 && Frames < totalFrames) break;
        cout << "Entrada inválida! Digite um número positivo." << std::endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
    intercalarFrames = checarIntercalo(totalFrames, intercalarFrames, Frames);

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Default);
    window.setFramerateLimit(60);

    Animador_Fundo bgAnimation;
    bgAnimation.setTargetSize(window.getSize());

    Encontrar_Diretorio diretorio;

    const string diretorio_Frame = diretorio.acharDiretorio_Arquivo("assets/bg_frames/");
    if (!diretorio_Frame.empty()) {
        if (bgAnimation.loadFrames(diretorio_Frame, totalFrames, 1, intercalarFrames))
            cout << "Frames de background carregados com sucesso!" << endl;
        else {
            cerr << "Falha ao carregar os frames de background. Verifique se os arquivos estão corretos." << endl;
            return -1;
        }
    }
    else {
        cerr << "Não foi possível localizar o diretório." << endl;
        return -1;
    }

    Audio bgMusic;
    const string diretorio_Audio = diretorio.acharDiretorio_Arquivo("assets/bg_audios/bg_music");
    if (!diretorio_Audio.empty()) {
        const string diretorio_Musica = Encontrar_Diretorio::concatenarEnderecos(diretorio_Audio,  "Aurora_s-Theme.ogg");
        if (bgMusic.loadMusic(diretorio_Musica)) {
            // Volume
            bgMusic.setVolume(0.0f);
            bgMusic.play();
            // Loop da música
            bgMusic.setLoop(true);
        }
        else {cerr << "Não foi possível carregar a música." << endl;}
    }
    else { cerr << "Não foi possível localizar a pasta de áudio. A música de fundo não será reproduzida." << endl; }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        bgAnimation.update();
        bgAnimation.draw(window);
        sf::RectangleShape player({40.f, 40.f});
        player.setFillColor(sf::Color::Green);
        player.setPosition(100.f, 50.f);
        window.display();
    }

    return 0;
}