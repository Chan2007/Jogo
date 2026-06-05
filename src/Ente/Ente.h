#include <time.h>
#include <windows.h>

class Gerenciador_Grafico;

class Ente {
protected:
	int id;
	static Gerenciador_Grafico* pGerenciadorGrafico;
public:
	static void sementear() {
		rand(); Sleep(100); time_t t;
		srand((unsigned)time(&t)); rand();
	}
};