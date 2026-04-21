//
// Created by Henrique on 21/04/2026.
//

#ifndef JOGO_FIND_DIRECTORY_H
#define JOGO_FIND_DIRECTORY_H

#include <string>
#include "Jogo_Principal/Animador_Fundo/animador_fundo.h"


class Find_Directory {
    private:
        static bool fileExists(const std::string &path);
        static std::string joinPath(const std::string &left, const std::string &right);
        static std::string getParentPath(const std::string &path);
    public:
        Find_Directory();
        ~Find_Directory();
        std::string findFolderDirectory(const std::string &exampleFileDirectory);
};
#endif