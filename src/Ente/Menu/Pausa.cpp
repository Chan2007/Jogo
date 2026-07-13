//
// Created by Henrique on 16/06/2026.
//

#include "Pausa.h"

#include <iostream>
#include "Sistema/Caminho/Encontrar_Caminho.h"


Pausa::Pausa() : Ente(), texturaFundo(), index(0)
{
    gerenciadorGrafico = &Gerenciadores::Gerenciador_Grafico::getGerenciador();

    std::string caminhoFundo = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/bg_derived.jpg");

    if (!caminhoFundo.empty()) {
        if (texturaFundo.loadFromFile(caminhoFundo))
            spriteFundo.setTexture(texturaFundo);
        else
            std::cerr << "Erro fatal: SFML nao conseguiu decodificar a imagem!" << std::endl;

    }
    std::string caminhoFonte = Encontrar_Caminho::acharDiretorio_Arquivo("qt-material-widgets/fonts/Roboto/Roboto-Medium.ttf");
    if (!caminhoFonte.empty())
        Fonte.loadFromFile(caminhoFonte);
    else
        std::cerr << "Aviso: Fonte de pausa nao encontrada no caminho padrao." << std::endl;

    const std::string retomar = "Retomar Jogo";
    const std::string salvar = "Salvar Jogo";
    const std::string voltar = "Voltar para o Menu";
    textosBotoes.push_back(retomar);
    textosBotoes.push_back(salvar);
    textosBotoes.push_back(voltar);
    initBotoes();
}

Pausa::~Pausa() {
    Botoes.clear();
    textosBotoes.clear();
}

void Pausa::initBotoes() {
    Botoes.clear();
    sf::VideoMode tamanhoJanela = gerenciadorGrafico->getSize();
    float startY = tamanhoJanela.height / 2.0f - 50.0f;

    for (int i = 0; i < textosBotoes.size(); ++i) {
        sf::Text texto;
        texto.setFont(Fonte);
        texto.setString(textosBotoes[i]);
        texto.setCharacterSize(45);
        texto.setFillColor(sf::Color::Black);

        // Centraliza o texto horizontalmente de acordo com o tamanho dele
        sf::FloatRect textRect = texto.getLocalBounds();
        texto.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        texto.setPosition(sf::Vector2f(tamanhoJanela.width / 2.0f, startY + i * 80.0f));

        Botoes.push_back(texto);
    }
}

void Pausa::atualizarItem(int direcao) {
    // Restaura cor do botão anteriormente focado
    Botoes[index].setFillColor(sf::Color::Black);
    Botoes[index].setStyle(sf::Text::Regular);

    index += direcao;
    if (index < 0) {
        index = Botoes.size() - 1;
    }
    else if (index >= static_cast<int>(Botoes.size())) {
        index = 0;
    }

    // Aplica destaque ao novo item focado
    Botoes[index].setFillColor(sf::Color::Red);
    Botoes[index].setStyle(sf::Text::Bold);
}

int Pausa::clicado() {
    if (!gerenciadorGrafico) return -1;

    const sf::Vector2i mousePos = gerenciadorGrafico->getMousePosition();
    for (int i = 0; i < Botoes.size(); ++i) {
        if (Botoes[i].getGlobalBounds().contains(mousePos.x,mousePos.y))
            return i;
    }
    return -1; // Nenhum item clicado
}

void Pausa::desenhar() {
    if (!gerenciadorGrafico) return;

    sf::View visaoAnterior = gerenciadorGrafico->getView();
    gerenciadorGrafico->setView(gerenciadorGrafico->getDefaultView());
    gerenciadorGrafico->draw(spriteFundo);

    for (int i = 0; i < Botoes.size(); ++i)
        gerenciadorGrafico->draw(Botoes[i]);

    gerenciadorGrafico->setView(visaoAnterior);
}
void Pausa::redimensionarTela() {
    if (!gerenciadorGrafico) return;

    sf::VideoMode tamanhoJanela = gerenciadorGrafico->getSize();

    spriteFundo.setScale(
        static_cast<float>(tamanhoJanela.width) / texturaFundo.getSize().x,
        static_cast<float>(tamanhoJanela.height) / texturaFundo.getSize().y
    );


    float startY = tamanhoJanela.height / 2.0f - (Botoes.size() * 80.0f) / 2.0f;
    for (int i = 0; i < static_cast<int>(Botoes.size()); ++i) {
        float xPos = tamanhoJanela.width / 2.0f;
        float yPos = startY + i * 80.0f;
        Botoes[i].setPosition(sf::Vector2f(xPos, yPos));
    }
}
void Pausa::aoApertarTecla(const Gerenciadores::Tecla& evento) {
    if (!evento.pressionada) return;

    if (evento.acao == "pausa_cima")
        atualizarItem(-1);
    else if (evento.acao == "pausa_baixo")
        atualizarItem(1);
}

