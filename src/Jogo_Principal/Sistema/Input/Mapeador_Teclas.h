//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_MAPEADOR_TECLAS_H
#define JOGO_MAPEADOR_TECLAS_H

#include <SFML/Window.hpp>
#include <map>
#include <string>

namespace Gerenciadores {
    class Mapeador_Teclas {
        private:
            std::map <sf::Keyboard::Key, std::string> mapeamento_teclas;
            std::map <unsigned int, std::string> mapeamento_joystick;
            std::map <sf::Joystick::Axis, std::string> mapeamento_eixos;

        public:
            Mapeador_Teclas();
            ~Mapeador_Teclas();

            void padrao();
            void padraoJoystick();

            std::string getAcao(sf::Keyboard::Key tecla) const;
            std::string getAcaoJoystick(unsigned int botao) const;

            bool estaMapeado(sf::Keyboard::Key tecla) const;
            bool estaMapeado(unsigned int botao) const;

            void remapearBotao(unsigned int eixo, const std::string& acao);
            void remapearTecla(sf::Keyboard::Key tecla, const std::string& acao);

            void desmapearTecla(sf::Keyboard::Key tecla);
            void desmapearBotao(unsigned int botao);

            std::string getAcaoEixo(sf::Joystick::Axis eixo) const;
            sf::Keyboard::Key getTecla(const std::string& acao) const;

            static std::string getNomeTecla(sf::Keyboard::Key tecla);
            static std::string getNomeBotao(unsigned int botao);
    };
} // Sistema

#endif //JOGO_MAPEADOR_TECLAS_H