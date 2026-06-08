#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"

Gerenciadores::Gerenciador_Grafico* Ente::gerenciadorGrafico = &Gerenciadores::Gerenciador_Grafico::getGerenciador();
