//
// Created by Henrique on 05/05/2026.
//

#include "Jogo_Principal.h"

Jogo_Principal::Jogo_Principal() :
    totalFrames(376), max_intercalarFrames(5),
    intercalarFrames(0), Frames(0), desktopMode(sf::VideoMode::getDesktopMode()),
    window(desktopMode, "Jogo LoL", sf::Style::Default), gerenciador_txt(), bgAnimation(&gerenciador_txt), diretorio(),
    diretorio_Frame(diretorio.acharDiretorio_Arquivo("assets/bg_frames/")), bgMusic(),
    diretorio_Audio(diretorio.acharDiretorio_Arquivo("assets/bg_audios/bg_music")),
    diretorio_Musica(), event(), jogador()
{
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

Jogo_Principal::~Jogo_Principal(){}

int Jogo_Principal::checarIntercalo(const int totalFrames, int step, const int frames) {
	const int min_step = ceil(static_cast<double>(totalFrames) / frames);
	return (step > min_step ? step : min_step);
}

void Jogo_Principal::executar() {
    Renderiza_Background();
    Renderiza_Audio();
    Inicializa_Jogador();
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

void Jogo_Principal::Inicializa_Jogador() {
    int opcao = 0;
    std::cout << "Escolha seu Campeao:\n0 - Naafiri\n1 - Yasuo\nDigite o numero: ";
    std::cin >> opcao;

    // Converte o inteiro para o tipo do Enum (C++03 exige o static_cast explícito)
    Personagens::EscolhaCampeao escolhaDoPlayer = static_cast<Personagens::EscolhaCampeao>(opcao);

    // Instancia o jogador passando a escolha dinâmica!
    jogador.setCampeao(escolhaDoPlayer);

    gerenciador_grav.aplicarGravidade(&jogador, true);
    Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(&jogador);

    jogador.setGerenciadorGravidade(&gerenciador_grav);
}


void Jogo_Principal::exibicao() {

    jogador.setPosicao(sf::Vector2f(100.f, 50.f));

    while (window.isOpen()) {
        float dt = relogio_fisica.restart().asSeconds();

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        bgAnimation.update();
        jogador.atualizar();

        gerenciador_grav.executar(dt);
        Gerenciadores::Gerenciador_Colisao::getInstancia().executar(&jogador, &gerenciador_grav);

        bgAnimation.draw(window);
        jogador.desenhar(window);

        window.display();
    }
}