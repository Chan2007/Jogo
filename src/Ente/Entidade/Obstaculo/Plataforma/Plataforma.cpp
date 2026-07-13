//
// Created by Henrique on 05/05/2026.
//

#include "Plataforma.h"

#include <iostream>

#include "Sistema/Caminho/Encontrar_Caminho.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

namespace Obstaculos {

	Plataforma::Plataforma(TipoPlataforma tipo) :
		Obstaculo(),
		tipoPlataforma(tipo),
		altura(2)
	{
		perigoso = false;

		// -------------------------------------------------------------------------
		// ATRIBUIÇÃO DE ATIVOS (ITCH.IO)
		// free-pixel-art-tiles criado por TotusLotus.
		// Disponível em: https://totuslotus.itch.io/free-pixel-art-tiles
		// Utilizado estritamente para fins acadêmicos e educacionais.
		// -------------------------------------------------------------------------

		std::string arquivoSprite = "/assets/Tiles_pack/Tileset_12.png";
		arquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo(arquivoSprite);

		try {
			if (arquivoSprite.empty())
				throw std::runtime_error("Erro: Arquivo não encontrado! Verifique o nome: " + arquivoSprite);
			if (!getTextura().loadFromFile(arquivoSprite))
				throw std::runtime_error("Erro: A textura falhou ao carregar: " + arquivoSprite);

			sprite.setTexture(getTextura());
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		sprite.setOrigin(16.f, 8.f * altura);
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

	void Plataforma::salvar() {
		if (buffer) {
			(*buffer) << "PLATAFORMA ";

			salvarDataBuffer();
			salvarObstaculo();

			(*buffer) << tipoPlataforma << '\n';
		}
	}
	Gerenciadores::Memento* Plataforma::salvarMemento() const {
		return new PlataformaMemento(*this);
	};
	void Plataforma::restaurarMemento(const Gerenciadores::Memento *memento) {
		Obstaculo::restaurarMemento(memento);

		const PlataformaMemento* pMemento = dynamic_cast<const PlataformaMemento*>(memento);
		if (pMemento) {
			altura = pMemento->alturaMemento;
			tipoPlataforma = pMemento->tipoPlataformaMemento;
		}
	}
} // namespace Obstaculos