#include "Proxy_Textura.h"
#include "Gerenciador_Textura.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include <SFML/OpenGL.hpp>

namespace Gerenciadores {
    void Proxy_Textura::loadThread() {
        sf::Context context;
        gerenciadorTextura->carregarTextura(pathToLoad);
        glFlush();
        mutex.lock();
        bufferReady = true;
        threadRunning = false;
        mutex.unlock();
    }

    Proxy_Textura::Proxy_Textura(Gerenciadores::Gerenciador_Textura* ger): pathToLoad(), mutex(), bufferReady(false)
    , threadRunning(false), thread(NULL), gerenciadorTextura(ger) {

    }

    Proxy_Textura::~Proxy_Textura() {
        if (thread) {
            thread->wait();
            delete thread;
            thread = NULL;
        }
        gerenciadorTextura = NULL;
    }

    void Proxy_Textura::preLoadNextFrame(const std::string& caminho) {
        mutex.lock();
        const bool rodando = threadRunning;
        mutex.unlock();

        if (rodando) return;

        if (thread) {
            thread->wait();
            delete thread;
            thread = NULL;
        }

        pathToLoad = caminho;

        mutex.lock();
        bufferReady = false;
        threadRunning = true;
        mutex.unlock();

        thread = new sf::Thread(&Proxy_Textura::loadThread, this);
        thread->launch();
    }
    sf::Texture* Proxy_Textura::getTexture(const std::string& caminho, bool forcarSincrono) {
        // Se já foi carregada e está no cache, retorna ela imediatamente
        sf::Texture* texturaReal = gerenciadorTextura->buscarTextura(caminho);
        if (texturaReal) return texturaReal;
        if (forcarSincrono) {
            gerenciadorTextura->carregarTextura(caminho);
            return gerenciadorTextura->buscarTextura(caminho);
        }
        // Se não está no cache e a thread não começou a carregar, dispara o preLoad
        mutex.lock();
        const bool rodando = threadRunning;
        mutex.unlock();
        if (!rodando && pathToLoad != caminho)
            preLoadNextFrame(caminho);

        // Se a thread terminou, consome o buffer
        mutex.lock();
        if (bufferReady) {
            bufferReady = false; // Consome o evento
            mutex.unlock();

            return gerenciadorTextura->buscarTextura(caminho);
        }
        mutex.unlock();
        // Se chegou aqui, significa que a Thread AINDA está rodando no fundo
        return NULL;
    }
}
