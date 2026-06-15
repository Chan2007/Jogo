#include "Proxy_Textura.h"
#include "Gerenciador_Textura.h"

namespace Gerenciadores {
    void Proxy_Textura::loadThread() {
        gerenciadorTextura->carregarTextura(pathToLoad);
        mutex.lock();
        bufferReady = true;
        threadRunning = false;
        mutex.unlock();
    }

    Proxy_Textura::Proxy_Textura(Gerenciadores::Gerenciador_Textura* ger): pathToLoad(), mutex(), bufferReady(false)
    , threadRunning(false), thread(NULL), gerenciadorTextura(ger)
    {}

    Proxy_Textura::~Proxy_Textura() {
        if (thread) {
            thread->wait();
            delete thread;
            thread = NULL;
        }
    }

    bool Proxy_Textura::isThreadRunning() {
        sf::Lock lock(mutex);
        return threadRunning;
    }

    void Proxy_Textura::preLoadNextFrame(const std::string& caminho) {
        mutex.lock();
        const bool rodando = threadRunning;
        mutex.unlock();

        if (rodando) return;

        if (thread) {
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
}