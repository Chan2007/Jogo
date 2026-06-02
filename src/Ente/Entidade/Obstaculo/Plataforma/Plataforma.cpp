//
// Created by Henrique on 05/05/2026.
//

#include "Plataforma.h"

namespace Obstaculos {

	Plataforma::Plataforma(TipoPlataforma tipo) :
		Obstaculo(),
		altura(32),
		largura(32),
		tipoPlataforma(tipo)
	{
		setTipo(Entidades::ENTIDADE_OBSTACULO);
		perigoso = false;

		std::string arquivoSprite = "/assets/Tiles_pack/Tileset_12.png";
		Encontrar_Diretorio diretorio;
		arquivoSprite = diretorio.acharDiretorio_Arquivo(arquivoSprite);
		if (arquivoSprite.empty()) {
			std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << arquivoSprite << std::endl;
		}
		else if (getTextura().loadFromFile(arquivoSprite)) {
			getCorpo().setTexture(getTextura());
		}
		else {
			std::cerr << "Erro: A textura falhou ao carregar: " << arquivoSprite << std::endl;
		}

		criar(tipo);
	}

	Plataforma::~Plataforma(){
	}

	void Plataforma::criar(TipoPlataforma tipo) {
		switch (tipo) {
			case NORMAL1:

				getCorpo().setTextureRect(sf::IntRect(64, 64, 32, 32));
				getCorpo().setOrigin(16.f, 16.f);
				getCorpo().setScale(6.f, 1.5f);
				break;

			case NORMAL2:

				getCorpo().setTextureRect(sf::IntRect(0, 96, 32, 32));
				getCorpo().setOrigin(16.f, 16.f);
				getCorpo().setScale(4.f, 1.2f);
				break;

			case NORMAL3:

				getCorpo().setTextureRect(sf::IntRect(128, 96, 32, 32));
				getCorpo().setOrigin(16.f, 16.f);
				getCorpo().setScale(3.f, 3.f);
				break;

			case CHAO:

				Encontrar_Diretorio diretorio;
				std::string arquivoSprite = "/assets/Tiles_pack/Tileset_12.png";
				arquivoSprite = diretorio.acharDiretorio_Arquivo(arquivoSprite);
				if (arquivoSprite.empty()) {
					std::cerr << "Erro: Arquivo nao encontrado! Verifique o nome: " << arquivoSprite << std::endl;
				}
				else if (getTextura().loadFromFile(arquivoSprite)) {
					getCorpo().setTexture(getTextura());
				}
				else {
					std::cerr << "Erro: A textura falhou ao carregar: " << arquivoSprite << std::endl;
				}
				getCorpo().setTextureRect(sf::IntRect(96, 64, 32, 32));
				getCorpo().setOrigin(16.f, 16.f);
				getCorpo().setScale(50.f, 2.f);
				break;

		}
	}

	void Plataforma::desenhar(sf::RenderWindow& window) {
		window.draw(getCorpo());
	}

	void Plataforma::atualizar() {

	}

	void Plataforma::salvar() {

	}

	void Plataforma::mover() {

	}

	float Plataforma::getAltura() {
		return altura;
	}

	float Plataforma::getLargura() {
		return largura;
	}

    void Plataforma::setPlataforma(Personagens::Jogador* J) {
        if (!J) return;

        if (J->getGerenciadorGravidade()) {
			J->getGerenciadorGravidade()->aoTocarChao(J, sf::Vector2f(0.f, -1.f));
        }

    }


} // namespace Obstaculos