//
// Created by Henrique on 21/04/2026.
//

#include "find_Directory.h"
#include <direct.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <sys/stat.h>

Find_Directory::Find_Directory() {
}
Find_Directory::~Find_Directory() {
}
bool Find_Directory::fileExists(const std::string& path) {
    struct _stat fileInfo;
    return _stat(path.c_str(), &fileInfo) == 0;
}

std::string Find_Directory::joinPath(const std::string& left, const std::string& right) {
    if (left.empty())
        return right;

    if (left[left.size() - 1] == '/' || left[left.size() - 1] == '\\')
        return left + right;

    return left + "/" + right;
}

std::string Find_Directory::getParentPath(const std::string& path) {
    const std::string::size_type slashPos = path.find_last_of("/\\");
    if (slashPos == std::string::npos)
        return std::string();

    return path.substr(0, slashPos);
}

std::string Find_Directory::findFolderDirectory(const std::string &FileDirectory) {
    char currentDirectoryBuffer[_MAX_PATH];
    if (_getcwd(currentDirectoryBuffer, sizeof(currentDirectoryBuffer)) == NULL)
        return std::string();

    std::string currentPath(currentDirectoryBuffer);
    while (!currentPath.empty()) {
        const std::string candidate = joinPath(currentPath, FileDirectory);
        if (fileExists(candidate))
            return joinPath(currentPath, FileDirectory);

        const std::string parentPath = getParentPath(currentPath);
        if (parentPath == currentPath)
            break;

        currentPath = parentPath;
    }

    return std::string();
}
