#include "painted_panel.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPainterPath>
#include <QStyleOptionButton>

namespace Widgets {
    PaintedPanel::PaintedPanel(QWidget *parent): QFrame(parent) {
        setAttribute(Qt::WA_TranslucentBackground);
    }

    void PaintedPanel::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QRectF frameRect = rect().adjusted(8, 8, -8, -8);
        const qreal radius = 28.0;

        const QRectF shadowRect = frameRect.adjusted(3, 3, 7, 7);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 35)); // Cor preta translúcida suave

        // Um raio ligeiramente maior faz as curvas se alinharem perfeitamente
        painter.drawRoundedRect(shadowRect, radius + 1.0, radius + 1.0);


        // Fundo branco do painel
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColorConstants::Svg::white);
        painter.drawRoundedRect(frameRect, radius, radius);

        const QPixmap backgroundImage("../assets/bg_frames/bg_derived.jpg");

        if (!backgroundImage.isNull()) {
            painter.save();

            // Mantém o recorte nos cantos arredondados
            const QRectF innerRect = frameRect.adjusted(2, 2, -2, -2);
            QPainterPath clipPath;
            clipPath.addRoundedRect(innerRect, radius - 2.0, radius - 2.0); // Raio ligeiramente menor
            painter.setClipPath(clipPath);

            // Calcula o tamanho expandindo proporcionalmente até preencher thodo o painel
            QSize targetSize = backgroundImage.size();
            targetSize.scale(innerRect.size().toSize(), Qt::KeepAspectRatioByExpanding);

            // Centraliza o retângulo (as sobras vão ficar para fora do clip e sumir)
            const QRect targetRect(
                innerRect.left() + (innerRect.width() - targetSize.width()) / 2,
                innerRect.top() + (innerRect.height() - targetSize.height()) / 2,
                targetSize.width(),
                targetSize.height()
            );

            painter.drawPixmap(targetRect, backgroundImage);

            painter.restore();
        }

        // Moldura externa preta
        painter.setPen(QPen(QColor(0, 0, 0, 220), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(frameRect.adjusted(3, 3, -3, -3), radius - 1.0, radius - 1.0);

        // Linhas horizontais decorativas com contraste preto/cinza
        const qreal cx = frameRect.center().x();
        const qreal margem = frameRect.width() * 0.1;

        // Gradiente de cima
        QLinearGradient linhaTop(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
        linhaTop.setColorAt(0.0,  QColor(0, 0, 0, 0));
        linhaTop.setColorAt(0.3,  QColor(0, 0, 0, 110));
        linhaTop.setColorAt(0.5,  QColor(80, 80, 80, 160));
        linhaTop.setColorAt(0.7,  QColor(0, 0, 0, 110));
        linhaTop.setColorAt(1.0,  QColor(0, 0, 0, 0));
        painter.setPen(QPen(QBrush(linhaTop), 3));
        painter.drawLine(QPointF(frameRect.left() + margem, frameRect.top() + 20),
                         QPointF(frameRect.right() - margem, frameRect.top() + 20));

        // Gradiente de baixo
        QLinearGradient linhaBase(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
        linhaBase.setColorAt(0.0,  QColor(0, 0, 0, 0));
        linhaBase.setColorAt(0.3,  QColor(0, 0, 0, 90));
        linhaBase.setColorAt(0.5,  QColor(60, 60, 60, 140));
        linhaBase.setColorAt(0.7,  QColor(0, 0, 0, 90));
        linhaBase.setColorAt(1.0,  QColor(0, 0, 0, 0));
        painter.setPen(QPen(QBrush(linhaBase), 3));
        painter.drawLine(QPointF(frameRect.left() + margem, frameRect.bottom() - 20),
                         QPointF(frameRect.right() - margem, frameRect.bottom() - 20));

        // Losango central ornamental em preto/cinza
        const qreal cl1 = frameRect.top() + 20;
        QPolygonF losango1;
        losango1 << QPointF(cx,     cl1 - 8)
                 << QPointF(cx + 8, cl1    )
                 << QPointF(cx,     cl1 + 8)
                 << QPointF(cx - 8, cl1    );

        painter.setPen(QPen(QColor(0, 0, 0, 180), 1));
        painter.setBrush(QColor(180, 180, 180, 110));
        painter.drawPolygon(losango1);

        const qreal cl2 = frameRect.bottom() - 20;
        QPolygonF losango2;
        losango2 << QPointF(cx,     cl2 - 8)
                 << QPointF(cx + 8, cl2    )
                 << QPointF(cx,     cl2 + 8)
                 << QPointF(cx - 8, cl2    );

        painter.setPen(QPen(QColor(0, 0, 0, 180), 1));
        painter.setBrush(QColor(180, 180, 180, 110));
        painter.drawPolygon(losango2);
    }
}
