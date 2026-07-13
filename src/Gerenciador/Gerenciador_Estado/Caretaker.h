//
// Created by Henrique on 20/06/2026.
//

#ifndef JOGO_CARETAKER_H
#define JOGO_CARETAKER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Memento.h"

namespace Gerenciadores {
    class Caretaker {
        private:
            std::vector<Memento*> mementos;
        public:
            ~Caretaker() {
                clearHistory();
            }

            void addMemento(Memento* memento) {
                if (memento) mementos.push_back(memento);
            }

            Memento* getMemento(const int index) const {
                if (index < mementos.size() && index >= 0) return mementos[index];
                return NULL;
            }

            Memento* popMemento() {
                if (mementos.empty()) return NULL;
                Memento* m = mementos.back();
                mementos.pop_back();
                return m;
            }

            void clearHistory() {
                for (int i = 0; i < mementos.size(); ++i)
                    delete mementos[i];
                mementos.clear();
            }
            void salvarEmArquivoTXT(const std::string& caminhoArquivo, int fase) const {
                std::ofstream arquivo(caminhoArquivo);

                if (!arquivo.is_open()) {
                    std::cerr << "Erro ao abrir o arquivo de save!" << std::endl;
                    return;
                }

                for (int i = 0; i < mementos.size(); ++i) {
                    // Se não for jogador, ignora silenciosamente.
                    if (mementos[i]->getTipo() != "Jogador") continue;

                    arquivo << " ";
                    mementos[i]->gravarNoArquivo(arquivo, fase);
                    arquivo << "\n";
                }

                arquivo.close();
            }
            struct DadosJogadorSalvo {
                std::string nome;
                float pontos;
                int fase;
            };

            std::vector<DadosJogadorSalvo> carregarDeArquivoTXT(const std::string& caminhoArquivo) {
                std::vector<DadosJogadorSalvo> listaJogadores;
                std::ifstream arquivo(caminhoArquivo);

                if (!arquivo.is_open()) return listaJogadores; // Retorna lista vazia se o arquivo não existir

                std::string linha;
                while (std::getline(arquivo, linha)) {
                    if (linha.empty()) continue;

                    std::stringstream ss(linha);
                    int fase;
                    std::string nome;
                    float pontos;

                    // O arquivo foi salvo como "[Fase] [Nome] [Pontos]"
                    ss >> fase >> nome >> pontos;

                    DadosJogadorSalvo dados;
                    dados.fase = fase;
                    dados.nome = nome;
                    dados.pontos = pontos;
                    listaJogadores.push_back(dados);
                }
                arquivo.close();
                return listaJogadores;
            }
    };
}

#endif //JOGO_CARETAKER_H