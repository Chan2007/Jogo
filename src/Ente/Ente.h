#ifndef ENTE_H
#define ENTE_H

#include <ctime>
#include <windows.h>

#include "Gerenciador/Gerenciador_Estado/Memento.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"

class Gerenciador_Grafico;
class Ente {
	protected:
		int id;
		static int contId;
		static Gerenciadores::Gerenciador_Grafico *gerenciadorGrafico;
		class EnteMemento: public Gerenciadores::Memento {
			private:
				int idMemento;
			protected:
				explicit EnteMemento(const Ente& e): idMemento(e.id) {}
				virtual ~EnteMemento() {}
				friend class Ente; // Permite ao pai acessar os dados privados
		};
	public:
		static void sementear() {
			rand(); Sleep(100); time_t t;
			srand((unsigned)time(&t)); rand();
		}

		virtual void executar() = 0;

		static double gerar_num_norm(double media, double desvpad, int a = 0, int b = 10);
		static double gerar_num_exp(double a = 0, double b = 10, double lambda = 1.0);
		static double gerar_num_binom(double a = 0, double b = 10, int numero_tentativas = 10, double p_sucesso = 0.5);

		Ente();
		virtual ~Ente() { contId -= 1; }

		virtual Gerenciadores::Memento* salvarMemento() const;
		virtual void restaurarMemento(const Gerenciadores::Memento* memento);

		void setId(int i) { id = i; }
		int getId() { return id; }
};
#endif
