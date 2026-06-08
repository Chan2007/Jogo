// Audio.h
#ifndef JOGO_AUDIO_H
#define JOGO_AUDIO_H

#include <string>
#include "SFML/Audio/Music.hpp"

namespace Gerenciadores{
    class Gerenciador_Audio {
        private:
            sf::Music musica;
            bool musicaAtivada; // Controla se o áudio global está ativado ou mutado
            // Singleton
            Gerenciador_Audio();
            // Proibe o uso de construtor de cópia e atribuição
            Gerenciador_Audio(const Gerenciador_Audio&);
            Gerenciador_Audio& operator=(const Gerenciador_Audio&);

        public:
            ~Gerenciador_Audio();

            // Méthodo estático para acessar a instância global
            static Gerenciador_Audio& getGerenciador();

            void loadMusic(const std::string& filePath);
            void setVolume(float volumePercent);
            float getVolume() const;
            void play();
            void pause();
            void stop();
            void setLoop(bool loop);
            bool isPlaying() const;

            // Métodos adicionais para o controle do Menu (Interface Qt)
            void ativarMusica(bool ligada);
            bool musicaEstaAtivada() const;
    };
}

#endif //JOGO_AUDIO_H