//
// Created by Henrique on 21/04/2026.
//

#include "Encontrar_Caminho.h"
#include <direct.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <sys/stat.h>

Encontrar_Caminho::Encontrar_Caminho() {}
Encontrar_Caminho::~Encontrar_Caminho() {}
bool Encontrar_Caminho::arquivoExiste(const std::string& path) {
    struct _stat fileInfo;
    return _stat(path.c_str(), &fileInfo) == 0;
}

std::string Encontrar_Caminho::concatenarEnderecos(const std::string &left, const std::string &right) {
    if (left.empty()) return right;
    if (left[left.size() - 1] == '/' || left[left.size() - 1] == '\\') return left + right;
    return left + "/" + right;
}

std::string Encontrar_Caminho::getCaminho_Parente(const std::string& path) {
    const std::string::size_type slashPos = path.find_last_of("/\\");
    if (slashPos == std::string::npos) return std::string();
    return path.substr(0, slashPos);
}

std::string Encontrar_Caminho::acharDiretorio_Arquivo(const std::string &FileDirectory) {
    char currentDirectory[_MAX_PATH];
    if (_getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) return std::string();
    std::string currentPath(currentDirectory);
    while (!currentPath.empty()) {
        const std::string candidate = concatenarEnderecos(currentPath, FileDirectory);
        if (arquivoExiste(candidate)) return candidate;
        const std::string parentPath = getCaminho_Parente(currentPath);
        if (parentPath == currentPath) break;
        currentPath = parentPath;
    }
    return std::string();
}
std::string Encontrar_Caminho::acharDiretorio_Arquivo(const std::string& FileDirectory1, const std::string& FileDirectory2) {
    std::string resultado = acharDiretorio_Arquivo(FileDirectory1);

    if (!resultado.empty()) return resultado;

    return acharDiretorio_Arquivo(FileDirectory2);
}
