#include "GraphDraw.h"

void drawText(QPainter &pt, const QRectF &r, int w, const QString &text) {
    Qt::Alignment a = Qt::AlignCenter;
    QRectF br = pt.boundingRect(r, a, text);
    if (br.left() < 0) {
        br.moveTo(0, br.y());
        a = Qt::AlignLeft | Qt::AlignVCenter;
    }
    else if (br.right() > w) {
        br.moveTo(w-br.width(), br.y());
        a = Qt::AlignRight | Qt::AlignVCenter;
    }
    pt.drawText(br, a, text);
}
