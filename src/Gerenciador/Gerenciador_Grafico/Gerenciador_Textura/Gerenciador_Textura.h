#ifndef JOGO_GERENCIADOR_TEXTURA_H
#define JOGO_GERENCIADOR_TEXTURA_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace Gerenciadores{
    class Gerenciador_Textura {
        private:
            std::map<std::string, sf::Texture*> texturas;
            mutable sf::Mutex texturaMutex;

        public:
            Gerenciador_Textura();
            ~Gerenciador_Textura();

            bool carregarTextura(const std::string& caminho);
            sf::Texture* buscarTextura(const std::string& caminho) const;
    };
}

#endif //JOGO_GERENCIADOR_TEXTURA_H
