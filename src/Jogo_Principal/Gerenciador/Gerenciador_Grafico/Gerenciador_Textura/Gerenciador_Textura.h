#ifndef JOGO_GERENCIADOR_TEXTURA_H
#define JOGO_GERENCIADOR_TEXTURA_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Gerenciador_Textura {
    private:
        std::map<std::string, sf::Texture*> texturas;

    public:
        Gerenciador_Textura();
        ~Gerenciador_Textura();

        bool carregar_textura(const std::string& caminho);
        void limpar_textura();
        sf::Texture* buscar_textura(const std::string& caminho) const;
};

#endif //JOGO_GERENCIADOR_TEXTURA_H
