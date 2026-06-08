#ifndef ENTE_H
#define ENTE_H

#include <time.h>
#include <windows.h>

#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"

class Gerenciador_Grafico;

class Ente {
	protected:
		int id;
		static Gerenciadores::Gerenciador_Grafico *gerenciadorGrafico;
	public:
		static void sementear() {
			rand(); Sleep(100); time_t t;
			srand((unsigned)time(&t)); rand();
		}
};
#endif
