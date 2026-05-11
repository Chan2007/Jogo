//
// Created by Henrique on 09/05/2026.
//

#include "Mapeador_Teclas.h"

namespace Gerenciadores {
    Mapeador_Teclas::Mapeador_Teclas() {
        padrao();
    }
    void Mapeador_Teclas::padrao() {
        mapeamento_teclas.clear();
        mapeamento_joystick.clear();
        mapeamento_eixos.clear();

        mapeamento_teclas[sf::Keyboard::W] = "pular";
        mapeamento_teclas[sf::Keyboard::Up] = "pular";
        mapeamento_teclas[sf::Keyboard::A] = "mover_esquerda";
        mapeamento_teclas[sf::Keyboard::Left] = "mover_esquerda";
        mapeamento_teclas[sf::Keyboard::D] = "mover_direita";
        mapeamento_teclas[sf::Keyboard::Right] = "mover_direita";

        mapeamento_teclas[sf::Keyboard::Space] = "pular";
        mapeamento_teclas[sf::Keyboard::LControl] = "atacar";
        mapeamento_teclas[sf::Keyboard::LShift] = "correr";
        mapeamento_teclas[sf::Keyboard::E] = "habilidade1";
        mapeamento_teclas[sf::Keyboard::Q] = "habilidade2";

        mapeamento_teclas[sf::Keyboard::Escape] = "menu";
        mapeamento_teclas[sf::Keyboard::Space] = "pause";
    }
    void Mapeador_Teclas::padraoJoystick() {
        // Botão A (Xbox) ou X (PS)
        mapeamento_joystick[0] = "pular";
        // Botão X (Xbox) ou □ (PS)
        mapeamento_joystick[1] = "habilidade1";
        // Botão B (Xbox) ou O (PS)
        mapeamento_joystick[2] = "atacar";
        // Botão Y (Xbox) ou △ (PS)
        mapeamento_joystick[3] = "habilidade2";
        // Botão LB
        mapeamento_joystick[4] = "desacelerar";
        // Botão RB
        mapeamento_joystick[5] = "acelerar";
        // Botão Select
        mapeamento_joystick[6] = "pausar";
        // Botão Start
        mapeamento_joystick[7] = "menu";

        mapeamento_eixos[sf::Joystick::X] = "mover_horizontal";
    }
    std::string Mapeador_Teclas::getAcao(const sf::Keyboard::Key tecla) const {
        std::map <sf::Keyboard::Key, std::string>::const_iterator it;
        it = mapeamento_teclas.find(tecla);
        if (it != mapeamento_teclas.end()) return it->second;
        return "";
    }
    std::string Mapeador_Teclas::getAcaoJoystick(unsigned int botao) const {
        std::map <unsigned int, std::string>::const_iterator it;
        it = mapeamento_joystick.find(botao);
        if (it != mapeamento_joystick.end()) return it->second;
        return "";
    }
    std::string Mapeador_Teclas::getAcaoEixo(sf::Joystick::Axis eixo) const {
        std::map <sf::Joystick::Axis, std::string>::const_iterator it;
        it = mapeamento_eixos.find(eixo);
        if (it != mapeamento_eixos.end()) return it->second;
        return "";
    }

    bool Mapeador_Teclas::estaMapeado(sf::Keyboard::Key tecla) const {
        return mapeamento_teclas.find(tecla) != mapeamento_teclas.end();
    }
    bool Mapeador_Teclas::estaMapeado(unsigned int botao) const {
        return mapeamento_joystick.find(botao) != mapeamento_joystick.end();
    }

    void Mapeador_Teclas::remapearBotao(unsigned int botao, const std::string& acao) {
        std::map<unsigned int, std::string>::iterator it = mapeamento_joystick.begin();
        while (it != mapeamento_joystick.end()) {
            if (it->second == acao)
                it = mapeamento_joystick.erase(it++);
            else ++it;
        }
        mapeamento_joystick[botao] = acao;
    }
    void Mapeador_Teclas::remapearTecla(sf::Keyboard::Key tecla, const std::string& acao) {
        std::map<sf::Keyboard::Key, std::string>::iterator it ;
        it =  mapeamento_teclas.begin();
        while (it != mapeamento_teclas.end()) {
            if (it->second == acao) {
                it = mapeamento_teclas.erase(it++);
            }
            else ++it;
        }
        mapeamento_teclas[tecla] = acao;
    }
    void Mapeador_Teclas::desmapearTecla(sf::Keyboard::Key tecla) {mapeamento_teclas.erase(tecla);}
    void Mapeador_Teclas::desmapearBotao(unsigned int botao) {mapeamento_joystick.erase(botao);}

    sf::Keyboard::Key Mapeador_Teclas::getTecla(const std::string& acao) const {
        std::map<sf::Keyboard::Key, std::string>::const_iterator it;
        it = mapeamento_teclas.begin();
        while (it != mapeamento_teclas.end()) {
            if (it->second == acao) return it->first;
            ++it;
        }
        return sf::Keyboard::KeyCount;
    }
    std::string Mapeador_Teclas::getNomeTecla(sf::Keyboard::Key tecla) {
        switch (tecla) {
            case sf::Keyboard::A: return "A";
            case sf::Keyboard::B: return "B";
            case sf::Keyboard::C: return "C";
            case sf::Keyboard::D: return "D";
            case sf::Keyboard::E: return "E";
            case sf::Keyboard::F: return "F";
            case sf::Keyboard::G: return "G";
            case sf::Keyboard::H: return "H";
            case sf::Keyboard::I: return "I";
            case sf::Keyboard::J: return "J";
            case sf::Keyboard::K: return "K";
            case sf::Keyboard::L: return "L";
            case sf::Keyboard::M: return "M";
            case sf::Keyboard::N: return "N";
            case sf::Keyboard::O: return "O";
            case sf::Keyboard::P: return "P";
            case sf::Keyboard::Q: return "Q";
            case sf::Keyboard::R: return "R";
            case sf::Keyboard::S: return "S";
            case sf::Keyboard::T: return "T";
            case sf::Keyboard::U: return "U";
            case sf::Keyboard::V: return "V";
            case sf::Keyboard::W: return "W";
            case sf::Keyboard::X: return "X";
            case sf::Keyboard::Y: return "Y";
            case sf::Keyboard::Z: return "Z";

            case sf::Keyboard::Num0: return "0";
            case sf::Keyboard::Num1: return "1";
            case sf::Keyboard::Num2: return "2";
            case sf::Keyboard::Num3: return "3";
            case sf::Keyboard::Num4: return "4";
            case sf::Keyboard::Num5: return "5";
            case sf::Keyboard::Num6: return "6";
            case sf::Keyboard::Num7: return "7";
            case sf::Keyboard::Num8: return "8";
            case sf::Keyboard::Num9: return "9";

            case sf::Keyboard::Space: return "Space";
            case sf::Keyboard::Return: return "Enter";
            case sf::Keyboard::Escape: return "Esc";
            case sf::Keyboard::BackSpace: return "Backspace";
            case sf::Keyboard::Tab: return "Tab";

            case sf::Keyboard::Left: return "Left";
            case sf::Keyboard::Right: return "Right";
            case sf::Keyboard::Up: return "Up";
            case sf::Keyboard::Down: return "Down";

            case sf::Keyboard::LShift: return "LShift";
            case sf::Keyboard::RShift: return "RShift";
            case sf::Keyboard::LControl: return "LCtrl";
            case sf::Keyboard::RControl: return "RCtrl";
            case sf::Keyboard::LAlt: return "LAlt";
            case sf::Keyboard::RAlt: return "RAlt";

            case sf::Keyboard::F1: return "F1";
            case sf::Keyboard::F2: return "F2";
            case sf::Keyboard::F3: return "F3";
            case sf::Keyboard::F4: return "F4";
            case sf::Keyboard::F5: return "F5";
            case sf::Keyboard::F6: return "F6";
            case sf::Keyboard::F7: return "F7";
            case sf::Keyboard::F8: return "F8";
            case sf::Keyboard::F9: return "F9";
            case sf::Keyboard::F10: return "F10";
            case sf::Keyboard::F11: return "F11";
            case sf::Keyboard::F12: return "F12";
            case sf::Keyboard::F13: return "F13";
            case sf::Keyboard::F14: return "F14";
            case sf::Keyboard::F15: return "F15";

            case sf::Keyboard::PageUp: return "PageUp";
            case sf::Keyboard::PageDown: return "PageDown";
            case sf::Keyboard::End: return "End";
            case sf::Keyboard::Home: return "Home";
            case sf::Keyboard::Insert: return "Insert";
            case sf::Keyboard::Delete: return "Delete";
            case sf::Keyboard::Add: return "+";
            case sf::Keyboard::Subtract: return "-";
            case sf::Keyboard::Multiply: return "*";
            case sf::Keyboard::Divide: return "/";

            default:
                return "Unknown";
        }
    }
} // Sistema