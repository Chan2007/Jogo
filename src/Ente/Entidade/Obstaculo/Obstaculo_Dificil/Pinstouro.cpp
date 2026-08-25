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
		nome = "Pinstouro";

		// -------------------------------------------------------------------------
		// ATRIBUIÇÃO DE ATIVOS (MODELVIEWER.LOL / KHADA)
		// Modelos 3D, malhas ou texturas obtidos via ModelViewer.lol.
		// Propriedade original dos personagens e artes: © Riot Games, Inc.
		// Uso estritamente acadêmico, educacional e não comercial.
		// -------------------------------------------------------------------------

		std::string caminhoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/Obstaculos/plant_blast_cone.png"); // obtido em: https://modelviewer.lol/
		try {
			if (caminhoSprite.empty())
				throw std::runtime_error("Erro: Arquivo não encontrado! Verifique o nome: " + caminhoSprite);
			if (!getTextura().loadFromFile(caminhoSprite))
				throw std::runtime_error("Erro: A textura falhou ao carregar: " + caminhoSprite);

			getSprite().setTexture(getTextura());
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}

		getSprite().setOrigin(1225.f, 1122.f);
		getSprite().setTextureRect(sf::IntRect(0, 0, 2450, 2244));
		getSprite().setScale(0.04f, 0.045f);
	}

	Pinstouro::~Pinstouro(){}

	void Pinstouro::executar() {}

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

			Gerenciadores::Gerenciador_Grafico& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador();
			const int rangeX = janela.getSize().width - static_cast<int>(tamObs.width);
			const int rangeY = janela.getSize().height - static_cast<int>(tamObs.height);

			if (rangeX > 0 && rangeY > 0) {
				sementear();
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
	Gerenciadores::Memento* Pinstouro::salvarMemento() const {
		return new PinstouroMemento(*this);
	};
	void Pinstouro::restaurarMemento(const Gerenciadores::Memento *memento) {
		Obstaculo::restaurarMemento(memento);

		const PinstouroMemento* pMemento = dynamic_cast<const PinstouroMemento*>(memento);
		if (pMemento) {
			impulso = pMemento->impulsoMemento;
			danosidade = pMemento->danosidadeMemento;
		}
	}

} // Obstaculo