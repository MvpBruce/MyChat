#include "tiplabel.h"
#include <QMouseEvent>

TipLabel::TipLabel(QWidget* parent)
    :QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void TipLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        emit clicked(this->text());
        return;
    }

    QLabel::mouseMoveEvent(ev);
}
