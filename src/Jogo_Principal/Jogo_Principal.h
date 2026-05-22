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
#include "Jogo_Principal/Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Jogo_Principal/Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Diretorio/Encontrar_Diretorio.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"
#include "Entidade/Personagem/Jogador/Jogador.h"


class Jogo_Principal {
private:
    const int totalFrames;
    const int max_intercalarFrames;
    int intercalarFrames;
    int Frames;
    const sf::VideoMode desktopMode;
    sf::RenderWindow window;
    sf::Clock relogio_fisica;
    Gerenciadores::Gerenciador_Gravidade gerenciador_grav;
    Gerenciador_Textura gerenciador_txt;
    Animador_Fundo bgAnimation;
    Encontrar_Diretorio diretorio;
    const std::string diretorio_Frame;
    Audio bgMusic;
    const std::string diretorio_Audio;
    const std::string diretorio_Musica;
    sf::Event event;
    Personagens::Jogador jogador;

public:
    Jogo_Principal();
    ~Jogo_Principal();
    int checarIntercalo(const int totalFrames, int step, const int frames);
    void executar();
    void Renderiza_Background();
    void Renderiza_Audio();
    void Inicializa_Jogador();
    void exibicao();
};


#endif //JOGO_JOGO_PRINCIPAL_H;