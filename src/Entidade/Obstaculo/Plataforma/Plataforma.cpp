//
// Created by Henrique on 05/05/2026.
//

#include "Plataforma.h"
#include "Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h" // Para usar o aoTocarChao

namespace Obstaculos {

	Plataforma::Plataforma() :
		Obstaculo(),
		altura(60.f),
		largura(250.f)
	{
		perigoso = false;
	}

	Plataforma::~Plataforma(){
	}





} // namespace Obstaculos