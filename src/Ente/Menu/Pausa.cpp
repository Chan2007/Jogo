//
// Created by Henrique on 16/06/2026.
//

#include "Pausa.h"

#include <iostream>
#include "Sistema/Caminho/Encontrar_Caminho.h"


Pausa::Pausa() :
    Ente(),
    texturaFundo(),
    indiceSelecionado(0)
{
    gerenciadorGrafico = &Gerenciadores::Gerenciador_Grafico::getGerenciador();

    std::string caminhoFundo = "assets/bg_frames/bg_derived.jpg";
    std::string caminhoRealFundo = Encontrar_Caminho::acharDiretorio_Arquivo(caminhoFundo);

    if (!caminhoRealFundo.empty()) {
        if (texturaFundo.loadFromFile(caminhoRealFundo)) { // Carrega direto do arquivo para o objeto da classe
            spriteFundo.setTexture(texturaFundo);

            spriteFundo.setScale(
                static_cast<float>(gerenciadorGrafico->getSize().width) / texturaFundo.getSize().x,
                static_cast<float>(gerenciadorGrafico->getSize().height) / texturaFundo.getSize().y
            );
        }
        else
            std::cerr << "Erro fatal: SFML nao conseguiu decodificar a imagem!" << std::endl;

    }
    std::string caminhoFonte = Encontrar_Caminho::acharDiretorio_Arquivo("fonts/Roboto/Roboto-Medium.ttf", "qt-material-widgets/fonts/Roboto/Roboto-Medium.ttf");
    if (caminhoFonte.empty())
        std::cerr << "Aviso: Fonte de pausa nao encontrada no caminho padrao." << std::endl;

    fonte.loadFromFile(caminhoFonte);

    const std::string retomar = "Retomar Jogo";
    const std::string salvar = "Salvar Jogo";
    const std::string voltar = "Voltar para o Menu";
    textosBotoes.push_back(retomar);
    textosBotoes.push_back(salvar);
    textosBotoes.push_back(voltar);
    inicializarBotoes();
}

Pausa::~Pausa() {
    botoes.clear();
}

void Pausa::inicializarBotoes() {
    botoes.clear();
    sf::VideoMode tamanhoJanela = gerenciadorGrafico->getSize();
    float startY = tamanhoJanela.height / 2.0f - 50.0f;

    for (int i = 0; i < textosBotoes.size(); ++i) {
        sf::Text texto;
        texto.setFont(fonte);
        texto.setString(textosBotoes[i]);
        texto.setCharacterSize(45);

        // Destaca o item selecionado
        if (i == indiceSelecionado) {
            texto.setFillColor(sf::Color::Red);
            texto.setStyle(sf::Text::Bold);
        }
        else
            texto.setFillColor(sf::Color::Black);

        // Centraliza o texto horizontalmente de acordo com o tamanho dele
        sf::FloatRect textRect = texto.getLocalBounds();
        texto.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        texto.setPosition(sf::Vector2f(tamanhoJanela.width / 2.0f, startY + i * 80.0f));

        botoes.push_back(texto);
    }
}

void Pausa::atualizarItemSelecionado(int direcao) {
    // Restaura cor do botão anteriormente focado
    botoes[indiceSelecionado].setFillColor(sf::Color::Black);
    botoes[indiceSelecionado].setStyle(sf::Text::Regular);

    indiceSelecionado += direcao;
    if (indiceSelecionado < 0) {
        indiceSelecionado = botoes.size() - 1;
    }
    else if (indiceSelecionado >= static_cast<int>(botoes.size())) {
        indiceSelecionado = 0;
    }

    // Aplica destaque ao novo item focado
    botoes[indiceSelecionado].setFillColor(sf::Color::Red);
    botoes[indiceSelecionado].setStyle(sf::Text::Bold);
}

int Pausa::cliqueMouse() {
    if (!gerenciadorGrafico) return -1;

    sf::Vector2i mousePos = gerenciadorGrafico->getMousePosition();
    for (int i = 0; i < botoes.size(); ++i) {
        if (botoes[i].getGlobalBounds().contains(mousePos.x,mousePos.y))
            return i;
    }
    return -1; // Nenhum item clicado
}

void Pausa::desenhar() {
    if (!gerenciadorGrafico) return;

    sf::View visaoAnterior = gerenciadorGrafico->getView();
    gerenciadorGrafico->setView(gerenciadorGrafico->getDefaultView());
    gerenciadorGrafico->draw(spriteFundo);

    for (int i = 0; i < botoes.size(); ++i)
        gerenciadorGrafico->draw(botoes[i]);

    gerenciadorGrafico->setView(visaoAnterior);
}
void Pausa::ajustarPosicoes() {
    if (!gerenciadorGrafico) return;

    sf::VideoMode tamanhoJanela = gerenciadorGrafico->getSize();

    spriteFundo.setScale(
        static_cast<float>(tamanhoJanela.width) / texturaFundo.getSize().x,
        static_cast<float>(tamanhoJanela.height) / texturaFundo.getSize().y
    );


    float startY = tamanhoJanela.height / 2.0f - (botoes.size() * 80.0f) / 2.0f;
    for (size_t i = 0; i < botoes.size(); ++i) {
        float xPos = tamanhoJanela.width / 2.0f;
        float yPos = startY + i * 80.0f;
        botoes[i].setPosition(sf::Vector2f(xPos, yPos));
    }
}