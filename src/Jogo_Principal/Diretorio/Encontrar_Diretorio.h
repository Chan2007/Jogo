//
// Created by Henrique on 21/04/2026.
//

#ifndef JOGO_FIND_DIRECTORY_H
#define JOGO_FIND_DIRECTORY_H

#include <string>
#include "Jogo_Principal/Animador_Fundo/animador_fundo.h"


class Encontrar_Diretorio {
    private:
        static bool arquivoExiste(const std::string &path);
        static std::string getCaminho_Parente(const std::string &path);
    public:
        Encontrar_Diretorio();
        ~Encontrar_Diretorio();
        static std::string concatenarEnderecos(const std::string &left, const std::string &right);
        std::string acharDiretorio_Arquivo(const std::string &exampleFileDirectory);
};
#endif