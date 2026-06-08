//
// Created by Henrique on 21/04/2026.
//

#include "Gerenciador_Audio.h"

#include <iostream>
namespace Gerenciadores {
    Gerenciador_Audio::Gerenciador_Audio(): musica(), musicaAtivada(true) {}
    Gerenciador_Audio::~Gerenciador_Audio() {musica.stop();}
    Gerenciador_Audio& Gerenciador_Audio::getGerenciador() {
        static Gerenciador_Audio instancia; // Criada uma única vez
        return instancia;
    }
    void Gerenciador_Audio::loadMusic(const std::string& filePath) {
        if (!musica.openFromFile(filePath)) return;
        musica.setVolume(50.0f);
        musica.setLoop(true);
        musica.play();
    }
    void Gerenciador_Audio::setVolume(float volumePercent) {
        volumePercent = std::max(0.0f, std::min(volumePercent, 100.0f));
        musica.setVolume(volumePercent);
    }
    float Gerenciador_Audio::getVolume() const {return musica.getVolume();}
    void Gerenciador_Audio::play() {if (musicaAtivada) musica.play();}
    void Gerenciador_Audio::pause() {musica.pause();}
    void Gerenciador_Audio::stop() {musica.stop();}
    void Gerenciador_Audio::setLoop(const bool loop) {musica.setLoop(loop);}
    bool Gerenciador_Audio::isPlaying() const {return musica.getStatus() == sf::Music::Playing;}

    void Gerenciador_Audio::ativarMusica(const bool ligada) {
        musicaAtivada = ligada;
        if (!musicaAtivada)
            musica.stop(); // Para imediatamente checkbox estiver desmarcado
        else if (musica.getDuration() != sf::Time::Zero) {
            if (musica.getStatus() == sf::Music::Paused || musica.getStatus() == sf::Music::Stopped)
                musica.play();
        }
    }

    bool Gerenciador_Audio::musicaEstaAtivada() const {
        return musicaAtivada;
    }
}