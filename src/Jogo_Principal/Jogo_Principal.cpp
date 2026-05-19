//
// Created by Henrique on 05/05/2026.
//

#include "Jogo_Principal.h"

Jogo_Principal::Jogo_Principal() {
    while (true) {
        std::cout << "Quantos frames intercalar: " << "(maximo recomendado: " << max_intercalarFrames << ")" << std::endl;
        // Obs.: Se intercalarFrames for muito grande, a qualidade do cenário irá cair.
        if (std::cin >> intercalarFrames && intercalarFrames > 0 && intercalarFrames <= totalFrames) break;
        std::cout << "Entrada inválida! Digite um número positivo." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    while (true) {
        std::cout << "Maximo de frames a rodar: " << "(maximo: " << totalFrames << ")" << std::endl;
        // Obs.: Se maxFrames for muito pequeno, a qualidade do cenario irá dessincronizar.
        if (std::cin >> Frames && Frames > 0 && Frames < totalFrames) break;
        std::cout << "Entrada inválida! Digite um número positivo." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    intercalarFrames = checarIntercalo(totalFrames, intercalarFrames, Frames);
	executar();
}

int Jogo_Principal::checarIntercalo(const int totalFrames, int step, const int frames) {
	const int min_step = ceil(static_cast<double>(totalFrames) / frames);
	return (step > min_step ? step : min_step);
}

void Jogo_Principal::executar() {
    Renderiza_Background();
    Renderiza_Audio();
    exibicao();

}


void Jogo_Principal::Renderiza_Background() {
    window.setFramerateLimit(60);

    bgAnimation.setTargetSize(window.getSize());

    if (!diretorio_Frame.empty()) {
        if (bgAnimation.loadFrames(diretorio_Frame, totalFrames, 1, intercalarFrames))
            std::cout << "Frames de background carregados com sucesso!" << std::endl;
        else {
            std::cerr << "Falha ao carregar os frames de background. Verifique se os arquivos estão corretos." << std::endl;
        }
    }
    else {
        std::cerr << "Não foi possível localizar o diretório." << std::endl;
    }
}

void Jogo_Principal::Renderiza_Audio() {
    if (!diretorio_Audio.empty()) {
        const std::string diretorio_Musica = Encontrar_Diretorio::concatenarEnderecos(diretorio_Audio, "Aurora_s-Theme.ogg");
        if (bgMusic.loadMusic(diretorio_Musica)) {
            // Volume
            bgMusic.setVolume(0.0f);
            bgMusic.play();
            // Loop da música
            bgMusic.setLoop(true);
        }
        else { std::cerr << "Não foi possível carregar a música." << std::endl; }
    }
    else { std::cerr << "Não foi possível localizar a pasta de áudio. A música de fundo não será reproduzida." << std::endl; }
}

void Jogo_Principal::exibicao() {
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
        player.setFillColor(sf::Color::Green);
        player.setPosition(100.f, 50.f);
        window.display();
    }
}