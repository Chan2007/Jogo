//
// Created by Henrique on 06/05/2026.
//

#include "Obstaculo_Medio.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

namespace Obstaculos {

	Obstaculo_Medio::Obstaculo_Medio() :
		Obstaculo(),
		altura(2002),
		largura(1548)
	{
		setNome("Portal");
		perigoso = false;
		
		std::string caminhoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("/assets/sprites/Obstaculos/hex_gate.png");

		if(caminhoSprite.empty()) { std::cerr << "Erro: Arquivo não encontrado! Verifique o nome: " << caminhoSprite << std::endl; }
		else if (getTextura().loadFromFile(caminhoSprite)) { getSprite().setTexture(getTextura()); }
		else { std::cerr << "Erro: A textura falhou ao carregar: " << caminhoSprite << std::endl; }

		getSprite().setOrigin(static_cast<float>(largura) / 2.f, static_cast<float>(altura) / 2.f);
		getSprite().setTextureRect(sf::IntRect(0, 0, largura, altura));
		getSprite().setScale(0.0775f, 0.09f);
	}

	Obstaculo_Medio::~Obstaculo_Medio() {}

	void Obstaculo_Medio::executar() {

	}

	void Obstaculo_Medio::obstaculizar(Personagens::Jogador* p) {
		if (p) {
			if (p->getTamanho().intersects(getTamanho())) {
				sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();
				const int rangeX = janela.getSize().x - p->getTamanho().width;
				const int rangeY = janela.getSize().y - p->getTamanho().height;

				if (rangeX > 0 && rangeY > 0) {
					Ente::sementear();
					float randomX = static_cast<float>(rand() % rangeX) + (p->getTamanho().width / 2.f);
					float randomY = static_cast<float>(rand() % rangeY) + (p->getTamanho().height / 2.f);
					p->setPosicao(sf::Vector2f(randomX, randomY));
					p->setVelocidade(sf::Vector2f(0.f, 0.f));

				}
			}
		}
	}

} // Obstaculo