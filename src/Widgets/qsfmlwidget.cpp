#include "qsfmlwidget.h"
#include <QResizeEvent>

QSFMLWidget::QSFMLWidget(QWidget *parent) : QWidget(parent), m_initialized(false)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_PaintOnScreen);
    setFocusPolicy(Qt::StrongFocus);
}

QSFMLWidget::~QSFMLWidget()
{
    m_timer.stop();
    if (m_window.isOpen()) {
        m_window.close();
    }
}

QPaintEngine* QSFMLWidget::paintEngine() const
{
    return NULL;
}

void QSFMLWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!m_initialized)
    {
        ensurePolished();
        createWinId();

        m_window.create((sf::WindowHandle)winId());

        // Ativa contexto temporariamente apenas para OnInit
        const bool contextAtivado = m_window.setActive(true);
        if (contextAtivado) {
            OnInit();
            const QSize tamanhoAtual = size();
            if (tamanhoAtual.width() > 0 && tamanhoAtual.height() > 0) {
                m_window.setView(sf::View(sf::FloatRect(
                    0.f,
                    0.f,
                    static_cast<float>(tamanhoAtual.width()),
                    static_cast<float>(tamanhoAtual.height()))));
            }
            m_window.setActive(false); // Desativa para evitar conflitos
        }

        connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
        m_initialized = true;
    }

    if (!m_timer.isActive())
        m_timer.start(16);
}

void QSFMLWidget::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    m_timer.stop();
    if (m_window.isOpen()) {
        // Apenas desativa, não fecha
        m_window.setActive(false);
    }
}

void QSFMLWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (!m_initialized || !m_window.isOpen())
        return;

    const QSize tamanhoAtual = event ? event->size() : size();
    if (tamanhoAtual.width() <= 0 || tamanhoAtual.height() <= 0)
        return;

    m_window.setSize(sf::Vector2u(
        static_cast<unsigned int>(tamanhoAtual.width()),
        static_cast<unsigned int>(tamanhoAtual.height())));
    m_window.setView(sf::View(sf::FloatRect(
        0.f,
        0.f,
        static_cast<float>(tamanhoAtual.width()),
        static_cast<float>(tamanhoAtual.height()))));
}

void QSFMLWidget::onTimerTimeout()
{
    update();
}

void QSFMLWidget::paintEvent(QPaintEvent*)
{
    if (!m_initialized)
        return;

    if (m_window.getSize().x == 0 || m_window.getSize().y == 0)
    {
        return;
    }

    // Ativa contexto OpenGL para esta thread
    if (m_window.setActive(true))
    {
        OnUpdate();
        m_window.display();
        m_window.setActive(false); // Desativa para evitar conflitos
    }
}
