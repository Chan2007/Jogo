//
// Created by Henrique on 06/05/2026.
//

#include "Pinstouro.h"

#include <iostream>

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Obstaculos {

	Pinstouro::Pinstouro() :
	Obstaculo(),
	impulso(1100.f),
	danosidade(60)
	{
		perigoso = true;
		setNome("Pinstouro");

		std::string caminhoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/Obstaculos/plant_blast_cone.png"); // obtido em: https://modelviewer.lol/

		if (caminhoSprite.empty()) { std::cerr << "Erro: Arquivo não encontrado! Verifique o nome: " << caminhoSprite << std::endl; }
		else if (getTextura().loadFromFile(caminhoSprite)) { getSprite().setTexture(getTextura()); }
		else { std::cerr << "Erro: A textura falhou ao carregar: " << caminhoSprite << std::endl; }

		getSprite().setOrigin(1225.f, 1122.f);
		getSprite().setTextureRect(sf::IntRect(0, 0, 2450, 2244));
		getSprite().setScale(0.04f, 0.045f);
	}

	Pinstouro::~Pinstouro(){}

	void Pinstouro::executar() {

	}

	void Pinstouro::obstaculizar(Personagens::Jogador* p) {
		if (p) {
			if (perigoso) {
				p->receberDano(static_cast<int>(danosidade));
				std::cout << " Pinstouro lancou o jogador" << std::endl;
			}
			sf::Vector2f posJog = p->getPosicao();
			sf::FloatRect tamJog = p->getTamanho();
			sf::Vector2f posObs = getPosicao();
			sf::FloatRect tamObs = getTamanho();

			float tamx = tamJog.width / 2 + tamObs.width / 2;
			float dx = posObs.x - posJog.x;
			float tamy = tamJog.height / 2 + tamObs.height / 2;
			float dy = posObs.y - posJog.y;

			float multx = std::abs(dx/tamx);
			float multy = std::abs(dy/tamy);

			if (dx > 0) { p->setVelocidade(sf::Vector2f(-(multx * impulso * 2), -(multy * impulso))); }
			else { p->setVelocidade(sf::Vector2f(multx * impulso * 2, -(multy * impulso))); }

			sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();
			const int rangeX = janela.getSize().x - static_cast<int>(tamObs.width);
			const int rangeY = janela.getSize().y - static_cast<int>(tamObs.height);

			if (rangeX > 0 && rangeY > 0) {
				Ente::sementear();
				float randomX = static_cast<float>(rand() % rangeX) + (tamObs.width / 2.f);
				float randomY = static_cast<float>(rand() % rangeY) + (tamObs.height / 2.f);
				setPosicao(sf::Vector2f(randomX, randomY));
			}
		}
	}

	void Pinstouro::salvar() {
		if (buffer) {
			(*buffer) << "PINSTOURO ";

			salvarDataBuffer();
			salvarObstaculo();

			(*buffer) << '\n';
		}
	}

} // Obstaculo