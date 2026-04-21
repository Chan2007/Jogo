#include "main.h"

#include <direct.h>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sys/stat.h>

#include "Animador_Fundo/animador_fundo.h"

namespace {

bool fileExists(const std::string& path) {
    struct _stat fileInfo;
    return _stat(path.c_str(), &fileInfo) == 0;
}

std::string joinPath(const std::string& left, const std::string& right) {
    if (left.empty()) {
        return right;
    }

    if (left[left.size() - 1] == '/' || left[left.size() - 1] == '\\') {
        return left + right;
    }

    return left + "/" + right;
}

std::string getParentPath(const std::string& path) {
    const std::string::size_type slashPos = path.find_last_of("/\\");
    if (slashPos == std::string::npos) {
        return std::string();
    }

    return path.substr(0, slashPos);
}

std::string findAssetsFrameDirectory() {
    char currentDirectoryBuffer[_MAX_PATH];
    if (_getcwd(currentDirectoryBuffer, sizeof(currentDirectoryBuffer)) == NULL)
        return std::string();

    std::string currentPath(currentDirectoryBuffer);
    while (!currentPath.empty()) {
        const std::string candidate = joinPath(currentPath, "assets/bg_frames/frame0001.png");
        if (fileExists(candidate)) {
            return joinPath(currentPath, "assets/bg_frames/");
        }

        const std::string parentPath = getParentPath(currentPath);
        if (parentPath == currentPath) {
            break;
        }

        currentPath = parentPath;
    }

    return std::string();
}

int computeFrameStep(int totalFrames, int preferredStep, int maxFramesToLoad) {
    if (preferredStep <= 0) {
        preferredStep = 1;
    }

    if (maxFramesToLoad <= 0 || totalFrames <= maxFramesToLoad) {
        return preferredStep;
    }

    const int requiredStep = (totalFrames + maxFramesToLoad - 1) / maxFramesToLoad;
    return preferredStep > requiredStep ? preferredStep : requiredStep;
}

}

int main() {
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Jogo LoL", sf::Style::Fullscreen);

    BackgroundAnimator bg;
    bg.setTargetSize(window.getSize());
    const int totalFrames = 376;
    const int preferredFrameStep = 2;
    const int maxFramesToLoad = 376;
    const int frameStep = computeFrameStep(totalFrames, preferredFrameStep, maxFramesToLoad);
    const std::string frameDirectory = findAssetsFrameDirectory();
    const bool loaded = !frameDirectory.empty() && bg.loadFrames(frameDirectory, totalFrames, 1, frameStep);

    if (!loaded) {
        std::cerr << "Nao foi possivel localizar a pasta assets ou carregar os frames de fundo." << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        bg.update();
        bg.draw(window);
        window.display();
    }

    return 0;
}
