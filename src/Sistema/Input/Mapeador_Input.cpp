//
// Created by Henrique on 09/05/2026.
//

#include "Mapeador_Input.h"

namespace Gerenciadores {
    Mapeador_Input::Mapeador_Input() {
        mapeamento_teclas.clear();
        mapeamento_mouse.clear();
        mapeamento_joystick.clear();
        mapeamento_eixos.clear();
        padrao();
        padraoJoystick();
    }
    void Mapeador_Input::padrao() {
        mapeamento_teclas[sf::Keyboard::W] = "j1_pular";
        mapeamento_teclas[sf::Keyboard::I] = "j2_pular";
        mapeamento_teclas[sf::Keyboard::A] = "j1_mover_esquerda";
        mapeamento_teclas[sf::Keyboard::J] = "j2_mover_esquerda";
        mapeamento_teclas[sf::Keyboard::D] = "j1_mover_direita";
        mapeamento_teclas[sf::Keyboard::L] = "j2_mover_direita";

        mapeamento_teclas[sf::Keyboard::LShift] = "j1_correr";
        mapeamento_teclas[sf::Keyboard::RShift] = "j2_correr";

        mapeamento_teclas[sf::Keyboard::E] = "j1_atacar";
        mapeamento_teclas[sf::Keyboard::Q] = "j1_habilidade2";
        mapeamento_teclas[sf::Keyboard::O] = "j2_atacar";
        mapeamento_teclas[sf::Keyboard::U] = "j2_habilidade2";

        mapeamento_teclas[sf::Keyboard::Escape] = "pausar";
        mapeamento_teclas[sf::Keyboard::Space] = "escudo";
    }
    void Mapeador_Input::padraoJoystick() {
        // Botão A (Xbox) ou X (PS)
        mapeamento_joystick[0] = "pular";
        // Botão X (Xbox) ou □ (PS)
        mapeamento_joystick[1] = "atacar";
        // Botão B (Xbox) ou O (PS)
        mapeamento_joystick[2] = "habilidade";
        // Botão Y (Xbox) ou △ (PS)
        mapeamento_joystick[3] = "";
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
    std::string Mapeador_Input::getNomeTecla(const sf::Keyboard::Key tecla) {
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

            default: return "Unknown";
        }
    }
    std::string Mapeador_Input::getNomeBotao_Mouse(const sf::Mouse::Button botao) {
        switch (botao) {
            case sf::Mouse::Left:   return "Left";
            case sf::Mouse::Right:  return "Right";
            case sf::Mouse::Middle: return "Middle";
            case sf::Mouse::XButton1: return "XButton1";
            case sf::Mouse::XButton2: return "XButton2";
            default: return "Unknown";
        }
    }
    std::string Mapeador_Input::getNomeBotao_Joystick(const unsigned int botao) {
        switch (botao) {
            case 0: return "Button0";
            case 1: return "Button1";
            case 2: return "Button2";
            case 3: return "Button3";
            case 4: return "Button4";
            case 5: return "Button5";
            case 6: return "Button6";
            case 7: return "Button7";
            case 8: return "Button8";
            case 9: return "Button9";
            case 10: return "Button10";
            case 11: return "Button11";
            case 12: return "Button12";
            case 13: return "Button13";
            case 14: return "Button14";
            case 15: return "Button15";
            default: return "Unknown";
        }
    }
    std::string Mapeador_Input::getNomeEixo_Joystick(const sf::Joystick::Axis eixo) {
        switch (eixo) {
            case sf::Joystick::X: return "X";
            case sf::Joystick::Y: return "Y";
            case sf::Joystick::Z: return "Z";
            case sf::Joystick::R: return "R";
            case sf::Joystick::U: return "U";
            case sf::Joystick::V: return "V";
            case sf::Joystick::PovX: return "PovX";
            case sf::Joystick::PovY: return "PovY";
            default: return "Unknown";
        }
    }

} // Sistema