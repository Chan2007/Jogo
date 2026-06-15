//
// Created by Henrique on 05/05/2026.
//

#include "Plataforma.h"

#include <iostream>

#include "Sistema/Caminho/Encontrar_Caminho.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

namespace Obstaculos {

	Plataforma::Plataforma(TipoPlataforma tipo) :
		Obstaculo(),
		tipoPlataforma(tipo),
		altura(60)
	{
		setTipo(Entidades::ENTIDADE_OBSTACULO);
		perigoso = false;

		std::string arquivoSprite = "/assets/Tiles_pack/Tileset_12.png";
		arquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo(arquivoSprite);

		if (arquivoSprite.empty()) { std::cerr << "Erro: Arquivo não encontrado! Verifique o nome: " << arquivoSprite << std::endl; }
		else if (getTextura().loadFromFile(arquivoSprite)) { getSprite().setTexture(getTextura()); }
		else { std::cerr << "Erro: A textura falhou ao carregar: " << arquivoSprite << std::endl; }

		getSprite().setOrigin(16.f, 16.f);
		criar(tipo);
	}

	Plataforma::~Plataforma(){}

	void Plataforma::criar(const TipoPlataforma tipo) {
		switch (tipo) {
			case NORMAL1:

				getSprite().setTextureRect(sf::IntRect(64, 64, 32, 32));
				getSprite().setScale(10.f, 1.5f);
				break;

			case NORMAL2:

				getSprite().setTextureRect(sf::IntRect(0, 96, 32, 32));
				getSprite().setScale(7.f, 1.2f);
				break;

			case NORMAL3:

				getSprite().setTextureRect(sf::IntRect(128, 96, 32, 32));
				getSprite().setScale(3.f, 3.f);
				break;

			case CHAO:

				getSprite().setTextureRect(sf::IntRect(96, 64, 32, 32));
				getSprite().setScale(100.0f, 1.0f);
				break;

		}
	}

    void Plataforma::obstaculizar(Personagens::Jogador* J) {
        if (!J) return;
		gerenciadorGravidade.aoTocarChao(J, sf::Vector2f(0.f, -1.f));
    }


} // namespace Obstaculos