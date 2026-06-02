#include "Gerenciador_Textura.h"

Gerenciador_Textura::Gerenciador_Textura() : texturas() {}

Gerenciador_Textura::~Gerenciador_Textura() {
    limparTextura();
}

bool Gerenciador_Textura::carregarTextura(const std::string& caminho) {
    sf::Texture* textura = new sf::Texture();
    if (!textura->loadFromFile(caminho)) {
        delete textura;
        return false;
    }

    textura->setSmooth(true);

    {
        sf::Lock lock(texturaMutex);
        if (texturas.find(caminho) != texturas.end()) {
            delete textura;
            return true;
        }
        texturas[caminho] = textura;
    }

    return true;
}
void Gerenciador_Textura::limparTextura() {
    sf::Lock lock(texturaMutex);

    std::map<std::string, sf::Texture*>::iterator it = texturas.begin();
    while (it != texturas.end()) {
        if (it->second) delete it->second;
        ++it;
    }

    texturas.clear();
}

sf::Texture* Gerenciador_Textura::buscarTextura(const std::string& caminho) const {
    sf::Lock lock(texturaMutex);
    const std::map<std::string, sf::Texture*>::const_iterator it = texturas.find(caminho);
    return (it == texturas.end()) ? NULL : it->second;
}
