#include "backgroundwidget.h"
#include "Diretorio/Encontrar_Diretorio.h"

BackgroundWidget::BackgroundWidget(QWidget *parent)
    : QSFMLWidget(parent)
    , gerenciadorTextura()
    , animadorFundo(&gerenciadorTextura)
    , ultimoTamanho()
    , fundoCarregado(false)
{
}

void BackgroundWidget::OnInit()
{
    m_window.setFramerateLimit(60);
    ultimoTamanho = getSize();
    animadorFundo.setTargetSize(ultimoTamanho);

    const std::string diretorioFrames =
        Encontrar_Diretorio::acharDiretorio_Arquivo("assets/bg_frames/");

    if (!diretorioFrames.empty())
        fundoCarregado = animadorFundo.loadFrames(diretorioFrames, 376, 4, 4, 3);
}

void BackgroundWidget::OnUpdate()
{
    const sf::Vector2u tamanhoAtual = getSize();
    if (tamanhoAtual.x == 0 || tamanhoAtual.y == 0)
        return;

    if (tamanhoAtual != ultimoTamanho) {
        ultimoTamanho = tamanhoAtual;
        animadorFundo.setTargetSize(ultimoTamanho);
        m_window.setView(sf::View(sf::FloatRect(
            0.f,
            0.f,
            static_cast<float>(ultimoTamanho.x),
            static_cast<float>(ultimoTamanho.y)
        )));
    }

    if (!fundoCarregado)
        return;

    // Limpa apenas o fundo SFML
    m_window.clear(sf::Color::Black);

    // Renderiza o fundo animado
    animadorFundo.update();
    animadorFundo.draw(m_window);
}
