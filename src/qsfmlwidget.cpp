#include "qsfmlwidget.h"

QSFMLWidget::QSFMLWidget(QWidget *parent) : QWidget(parent), m_initialized(false)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
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
        bool contextAtivado = m_window.setActive(true);
        if (contextAtivado) {
            OnInit();
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
