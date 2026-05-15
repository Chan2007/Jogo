#include "Gerenciador_Textura.h"

Gerenciador_Textura::Gerenciador_Textura() :
    texturas() {
}

Gerenciador_Textura::~Gerenciador_Textura() {
    limpar_textura();
}

bool Gerenciador_Textura::carregar_textura(const std::string& caminho) {
    if (texturas.find(caminho) != texturas.end()) return true;

    sf::Texture* textura = new sf::Texture();
    if (!textura->loadFromFile(caminho)) {
        delete textura;
        textura = NULL;
        return false;
    }

    textura->setSmooth(true);
    texturas[caminho] = textura;
    return true;
}

void Gerenciador_Textura::limpar_textura() {
    std::map<std::string, sf::Texture*>::iterator it = texturas.begin();
    while (it != texturas.end()) {
        if (it->second) delete it->second;
        ++it;
    }

    texturas.clear();
}

sf::Texture* Gerenciador_Textura::buscar_textura(const std::string& caminho) const {
    std::map<std::string, sf::Texture*>::const_iterator it = texturas.find(caminho);
    return (it == texturas.end()) ? 0 : it->second;
}
