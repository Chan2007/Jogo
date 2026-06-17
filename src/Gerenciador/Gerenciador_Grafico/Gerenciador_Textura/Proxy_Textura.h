#ifndef PROXY_TEXTURA_H
#define PROXY_TEXTURA_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

namespace Gerenciadores {
    class Gerenciador_Textura;
    class Proxy_Textura {
        private:
            std::string pathToLoad;
            sf::Mutex mutex;
            bool bufferReady;
            bool threadRunning;
            sf::Thread* thread;
            Gerenciador_Textura* gerenciadorTextura;

            void loadThread();
            void preLoadNextFrame(const std::string& caminho);

        public:
            explicit Proxy_Textura(Gerenciador_Textura* ger);
            ~Proxy_Textura();
            sf::Texture *getTexture(const std::string &caminho, bool forcarSincrono = false);

    };
}
#endif