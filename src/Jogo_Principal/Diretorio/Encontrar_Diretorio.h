//
// Created by Henrique on 21/04/2026.
//

#ifndef JOGO_FIND_DIRECTORY_H
#define JOGO_FIND_DIRECTORY_H

#include <string>


class Encontrar_Diretorio {
    private:
        static bool arquivoExiste(const std::string &path);
        static std::string getCaminho_Parente(const std::string &path);
    public:
        Encontrar_Diretorio();
        ~Encontrar_Diretorio();
        static const std::string concatenarEnderecos(const std::string &left, const std::string &right);
        const std::string acharDiretorio_Arquivo(const std::string &exampleFileDirectory);
};
#endif
