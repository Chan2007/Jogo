//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGO_PRINCIPAL_H
#define JOGO_JOGO_PRINCIPAL_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <cmath>
#include "Diretorio/Encontrar_Diretorio.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"
using namespace std;

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
    const string diretorio_Frame;
    Audio bgMusic;
    const string diretorio_Audio;
    const string diretorio_Musica;
    sf::Event event;
    sf::RectangleShape player;
public:
    Jogo_Principal() : 
    totalframes(376), max_intercalarFrames(5), 
    intercalarFrames(0), Frames(0), desktopMode(sf::VideoMode::getDesktopMode()), 
    window(desktopMode, "Jogo LoL", sf::Style::Default), bgAnimation(), diretorio(), 
    diretorio_Frame(diretorio.acharDiretorio_Arquivo("assets/bg_frames/")), bgMusic(), 
    diretorio_Audio(diretorio.acharDiretorio_Arquivo("assets/bg_audios/bg_music")), 
    diretorio_Musica(), event(), player(40.f, 40.f)
    {}
    ~Jogo_Principal() {}
    int checarIntercalo(const int totalFrames, int step, const int frames);
    void Executar();
    void Renderiza_Background();
    void Renderiza_Audio();
    void Exibicao();
};


#endif //JOGO_JOGO_PRINCIPAL_H