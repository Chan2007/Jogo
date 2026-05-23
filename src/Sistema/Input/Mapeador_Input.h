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
            std::string template_getAcao(const Map& mapa, tipoObj obj) const {
                typename Map::const_iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->first == obj) return it->second;
                    ++it;
                }
                return "";
            }
            template <typename KeyType, typename Map>
            KeyType template_getObj(const Map& mapa, const std::string& acao) const {
                typename Map::const_iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->second == acao) return it->first;
                    ++it;
                }
                return KeyType();
            }

            template <typename Map, typename tipoObj>
            static bool template_estaMapeado(const Map& mapa, tipoObj obj) {
                return mapa.find(obj) != mapa.end();
            }
            template <typename Map, typename TipoObj>
            void template_remapear(Map& mapa, TipoObj obj, const std::string& acao) {
                typename Map::iterator it = mapa.begin();
                while (it != mapa.end()) {
                    if (it->second == acao) {
                        typename Map::iterator temp = it;
                        mapa.erase(it++);
                    }
                    else ++it;
                }
                mapa[obj] = acao;
            }
            template <typename Map, typename tipoObj>
            static void template_desmapear(Map& mapa, tipoObj obj) {
                mapa.erase(obj);
            }

        public:
            Mapeador_Input();
            ~Mapeador_Input();

            void padrao();
            void padraoJoystick();

            std::string getAcao(sf::Keyboard::Key tecla) const {return template_getAcao(mapeamento_teclas, tecla);}
            std::string getAcao(sf::Mouse::Button botao) const {return template_getAcao(mapeamento_mouse, botao);}
            std::string getAcao(unsigned int botao) const {return template_getAcao(mapeamento_joystick, botao);}
            std::string getAcao(sf::Joystick::Axis eixo) const {return template_getAcao(mapeamento_eixos, eixo);}

            bool estaMapeado(sf::Keyboard::Key tecla) const {return template_estaMapeado(mapeamento_teclas, tecla);}
            bool estaMapeado(sf::Mouse::Button botao) const {return template_estaMapeado(mapeamento_mouse, botao);}
            bool estaMapeado(unsigned int botao) const {return template_estaMapeado(mapeamento_joystick, botao);}
            bool estaMapeado(sf::Joystick::Axis eixo) const {return template_estaMapeado(mapeamento_eixos, eixo);}

            void remapear(sf::Keyboard::Key tecla, const std::string& acao) {template_remapear(mapeamento_teclas, tecla, acao);}
            void remapear(sf::Mouse::Button botao, const std::string& acao) {template_remapear(mapeamento_mouse, botao, acao);}
            void remapear(unsigned int botao, const std::string& acao) {template_remapear(mapeamento_joystick, botao, acao);}
            void remapear(sf::Joystick::Axis eixo, const std::string& acao) {template_remapear(mapeamento_eixos, eixo, acao);}

            void desmapear(sf::Keyboard::Key tecla) {template_desmapear(mapeamento_teclas, tecla);}
            void desmapear(sf::Mouse::Button botao) {template_desmapear(mapeamento_mouse, botao);}
            void desmapear(unsigned int botao) {template_desmapear(mapeamento_joystick, botao);}
            void desmapear(sf::Joystick::Axis eixo) {template_desmapear(mapeamento_eixos, eixo);}

            sf::Keyboard::Key getTecla(const std::string& acao) const {return template_getObj<sf::Keyboard::Key>(mapeamento_teclas, acao);}
            sf::Mouse::Button getBotao_Mouse(const std::string& acao) const {return template_getObj<sf::Mouse::Button>(mapeamento_mouse, acao);}
            unsigned int getBotao_Joystick(const std::string& acao) const {return template_getObj<unsigned int>(mapeamento_joystick, acao);}
            sf::Joystick::Axis getEixo(const std::string& acao) const {return template_getObj<sf::Joystick::Axis>(mapeamento_eixos, acao);}

            static std::string getNomeTecla(sf::Keyboard::Key tecla);
            static std::string getNomeBotao_Mouse(sf::Mouse::Button botao);
            static std::string getNomeBotao_Joystick(unsigned int botao);
            static std::string getNomeEixo_Joystick(sf::Joystick::Axis eixo);
    };
} // Sistema

#endif //JOGO_MAPEADOR_TECLAS_H