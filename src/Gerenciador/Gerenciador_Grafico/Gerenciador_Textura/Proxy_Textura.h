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
            Gerenciadores::Gerenciador_Textura* gerenciadorTextura;

            void loadThread();

        public:
            explicit Proxy_Textura(Gerenciadores::Gerenciador_Textura* ger);
            ~Proxy_Textura();

            void preLoadNextFrame(const std::string& caminho);

            bool isThreadRunning();
    };
}
#endif