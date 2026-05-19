//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGO_PRINCIPAL_H
#define JOGO_JOGO_PRINCIPAL_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <cmath>
#include "Diretorio/Encontrar_Diretorio.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"


class Jogo_Principal {
private:
    const int totalFrames;
    const int max_intercalarFrames;
    int intercalarFrames;
    int Frames;
    const sf::VideoMode desktopMode;
    sf::RenderWindow window;
    Animador_Fundo bgAnimation;
    Encontrar_Diretorio diretorio;
    const std::string diretorio_Frame;
    Audio bgMusic;
    const std::string diretorio_Audio;
    const std::string diretorio_Musica;
    sf::Event event;
    sf::RectangleShape player;

public:
    Jogo_Principal() :
    totalFrames(376), max_intercalarFrames(5),
    intercalarFrames(0), Frames(0), desktopMode(sf::VideoMode::getDesktopMode()),
    window(desktopMode, "Jogo LoL", sf::Style::Default), bgAnimation(), diretorio(),
    diretorio_Frame(diretorio.acharDiretorio_Arquivo("assets/bg_frames/")), bgMusic(),
    diretorio_Audio(diretorio.acharDiretorio_Arquivo("assets/bg_audios/bg_music")),
    diretorio_Musica(), event(), player(sf::Vector2f(40.0f, 40.0f))
    {}
    ~Jogo_Principal() {}
    int checarIntercalo(const int totalFrames, int step, const int frames);
    void executar();
    void Renderiza_Background();
    void Renderiza_Audio();
    void exibicao();
};


#endif //JOGO_JOGO_PRINCIPAL_H