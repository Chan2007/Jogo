//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_MAPEADOR_TECLAS_H
#define JOGO_MAPEADOR_TECLAS_H

#include <SFML/Window.hpp>
#include <map>
#include <string>

namespace Gerenciadores {
    class Mapeador_Input {
        private:
            std::map <sf::Keyboard::Key, std::string> mapeamento_teclas;
            std::map <sf::Mouse::Button, std::string> mapeamento_mouse;
            std::map <unsigned int, std::string> mapeamento_joystick;
            std::map <sf::Joystick::Axis, std::string> mapeamento_eixos;

            template <typename Map, typename tipoObj>
            std::string getAcao(const Map& mapa, tipoObj chave) const {
                typename Map::const_iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->first == chave) return it->second;
                    ++it;
                }
                return "";
            }
            template <typename KeyType, typename Map>
            KeyType getTecla(const Map& mapa, const std::string& acao) const {
                typename Map::const_iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->second == acao) return it->first;
                    ++it;
                }
                return KeyType();
            }

            template <typename Map, typename tipoObj>
            static bool estaMapeado(const Map& mapa, tipoObj chave) {
                return mapa.find(chave) != mapa.end();
            }
            template <typename Map, typename TipoObj>
            void remapear(Map& mapa, TipoObj chave, const std::string& acao) {
                typename Map::iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->second == acao) {
                        typename Map::iterator temp = it;
                        mapa.erase(it++);
                    }
                    else ++it;
                }
                mapa[chave] = acao;
            }
            template <typename Map, typename tipoObj>
            static void desmapear(Map& mapa, tipoObj chave) {
                mapa.erase(chave);
            }

        public:
            Mapeador_Input();
            ~Mapeador_Input(){};

            void padrao();
            void padraoJoystick();

            std::string getAcao(const sf::Keyboard::Key tecla) const {return getAcao(mapeamento_teclas, tecla);}
            std::string getAcao(const sf::Mouse::Button botao) const {return getAcao(mapeamento_mouse, botao);}
            std::string getAcao(const unsigned int botao) const {return getAcao(mapeamento_joystick, botao);}
            std::string getAcao(const sf::Joystick::Axis eixo) const {return getAcao(mapeamento_eixos, eixo);}

            bool estaMapeado(const sf::Keyboard::Key tecla) const {return estaMapeado(mapeamento_teclas, tecla);}
            bool estaMapeado(const sf::Mouse::Button botao) const {return estaMapeado(mapeamento_mouse, botao);}
            bool estaMapeado(const unsigned int botao) const {return estaMapeado(mapeamento_joystick, botao);}
            bool estaMapeado(const sf::Joystick::Axis eixo) const {return estaMapeado(mapeamento_eixos, eixo);}

            void remapear(const sf::Keyboard::Key tecla, const std::string& acao) {remapear(mapeamento_teclas, tecla, acao);}
            void remapear(const sf::Mouse::Button botao, const std::string& acao) {remapear(mapeamento_mouse, botao, acao);}
            void remapear(const unsigned int botao, const std::string& acao) {remapear(mapeamento_joystick, botao, acao);}
            void remapear(const sf::Joystick::Axis eixo, const std::string& acao) {remapear(mapeamento_eixos, eixo, acao);}

            void desmapear(const sf::Keyboard::Key tecla) {desmapear(mapeamento_teclas, tecla);}
            void desmapear(const sf::Mouse::Button botao) {desmapear(mapeamento_mouse, botao);}
            void desmapear(const unsigned int botao) {desmapear(mapeamento_joystick, botao);}
            void desmapear(const sf::Joystick::Axis eixo) {desmapear(mapeamento_eixos, eixo);}

            sf::Keyboard::Key getTecla(const std::string& acao) const {return getTecla<sf::Keyboard::Key>(mapeamento_teclas, acao);}
            sf::Mouse::Button getBotao_Mouse(const std::string& acao) const {return getTecla<sf::Mouse::Button>(mapeamento_mouse, acao);}
            unsigned int getBotao_Joystick(const std::string& acao) const {return getTecla<unsigned int>(mapeamento_joystick, acao);}
            sf::Joystick::Axis getEixo(const std::string& acao) const {return getTecla<sf::Joystick::Axis>(mapeamento_eixos, acao);}

            static std::string getNomeTecla(sf::Keyboard::Key tecla);
            static std::string getNomeBotao_Mouse(sf::Mouse::Button botao);
            static std::string getNomeBotao_Joystick(unsigned int botao);
            static std::string getNomeEixo_Joystick(sf::Joystick::Axis eixo);
    };
} // Sistema

#endif //JOGO_MAPEADOR_TECLAS_H