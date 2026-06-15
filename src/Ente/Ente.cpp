#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"

#include <random>
#include <cmath>

int Ente::contId = 0;

Gerenciadores::Gerenciador_Grafico* Ente::gerenciadorGrafico = &Gerenciadores::Gerenciador_Grafico::getGerenciador();

Ente::Ente(): id(contId++){}

double Ente::gerar_num_norm(double media, double desvpad, int a, int b) {
    std::mt19937 gerador(static_cast<unsigned int>(std::time(0)));
    std::normal_distribution<double> distribuicao(media, desvpad);

    // Gera o primeiro número
    double resultado = distribuicao(gerador);

    while (true) {
        if (resultado >= a && resultado <= b)
            break;
        resultado = distribuicao(gerador);
    }
    return resultado;
}
double Ente::gerar_num_exp(double a, double b, double lambda) {
    if (a >= b) (a = b - 1.0);
    std::mt19937 gerador(static_cast<unsigned int>(std::time(0)));
    std::exponential_distribution<double> distribuicao(lambda);

    double exp = distribuicao(gerador);

    // Aplica a função distribuição acumulada para comprimir de 0 a infinito para o intervalo [a, b]
    double fator_compressao = 1.0 - std::exp(-exp);
    double resultado = a + (b - a) * fator_compressao;

    if (resultado > b) return b;
    if (resultado < a) return a;
    return resultado;
}

double Ente::gerar_num_binom(double a, double b, int numero_tentativas, double p_sucesso) {
    if (a >= b) (a = b - 1.0);
    if (numero_tentativas <= 0) numero_tentativas = 1;

    std::mt19937 gerador(static_cast<unsigned int>(std::time(0)));

    // Configura a distribuição binomial padrão do C++11
    std::binomial_distribution<int> distribuicao(numero_tentativas, p_sucesso);

    int sucessos = distribuicao(gerador);

    // Transforma a escala [0, numero_tentativas] para a escala [a, b]
    double proporcao = static_cast<double>(sucessos) / static_cast<double>(numero_tentativas);
    double resultado = a + (b - a) * proporcao;

    if (resultado > b) return b;
    if (resultado < a) return a;
    return resultado;
}
