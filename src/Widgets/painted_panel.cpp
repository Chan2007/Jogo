#include "painted_panel.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>

PaintedPanel::PaintedPanel(QWidget *parent)
    : QFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void PaintedPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF frameRect = rect().adjusted(6, 6, -6, -6);

    // Fundo escuro profundo — Ender Lilies
    QLinearGradient fillGradient(frameRect.topLeft(), frameRect.bottomLeft());
    fillGradient.setColorAt(0.0,  QColor(8,  12, 22, 242));
    fillGradient.setColorAt(0.5,  QColor(11, 16, 28, 238));
    fillGradient.setColorAt(1.0,  QColor(7,  10, 19, 245));

    painter.setPen(Qt::NoPen);
    painter.setBrush(fillGradient);
    painter.drawRect(frameRect);  // sem arredondamento — austero

    // Moldura externa fina — azul-acinzentado frio
    painter.setPen(QPen(QColor(148, 172, 200, 68), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(frameRect.adjusted(1, 1, -1, -1));

    // Moldura interna — mais sutil, cria profundidade
    painter.setPen(QPen(QColor(110, 140, 172, 32), 1));
    painter.drawRect(frameRect.adjusted(6, 6, -6, -6));

    // Linhas horizontais decorativas no topo e base — ornamento Ender Lilies
    const qreal cx = frameRect.center().x();
    const qreal margem = frameRect.width() * 0.12;

    // linha topo
    QLinearGradient linhaTop(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
    linhaTop.setColorAt(0.0,  QColor(130, 165, 200, 0));
    linhaTop.setColorAt(0.3,  QColor(148, 178, 210, 70));
    linhaTop.setColorAt(0.5,  QColor(160, 190, 220, 90));
    linhaTop.setColorAt(0.7,  QColor(148, 178, 210, 70));
    linhaTop.setColorAt(1.0,  QColor(130, 165, 200, 0));
    painter.setPen(QPen(QBrush(linhaTop), 1));
    painter.drawLine(QPointF(frameRect.left() + margem, frameRect.top() + 14),
                     QPointF(frameRect.right() - margem, frameRect.top() + 14));

    // linha base
    QLinearGradient linhaBase(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
    linhaBase.setColorAt(0.0,  QColor(130, 165, 200, 0));
    linhaBase.setColorAt(0.3,  QColor(148, 178, 210, 55));
    linhaBase.setColorAt(0.5,  QColor(160, 190, 220, 72));
    linhaBase.setColorAt(0.7,  QColor(148, 178, 210, 55));
    linhaBase.setColorAt(1.0,  QColor(130, 165, 200, 0));
    painter.setPen(QPen(QBrush(linhaBase), 1));
    painter.drawLine(QPointF(frameRect.left() + margem, frameRect.bottom() - 14),
                     QPointF(frameRect.right() - margem, frameRect.bottom() - 14));

    // Losango central ornamental — detalhe típico do Ender Lilies
    const qreal cy = frameRect.top() + 14;
    QPolygonF losango;
    losango << QPointF(cx,     cy - 4)
            << QPointF(cx + 4, cy    )
            << QPointF(cx,     cy + 4)
            << QPointF(cx - 4, cy    );

    painter.setPen(QPen(QColor(155, 185, 215, 100), 1));
    painter.setBrush(QColor(140, 175, 210, 45));
    painter.drawPolygon(losango);
}
