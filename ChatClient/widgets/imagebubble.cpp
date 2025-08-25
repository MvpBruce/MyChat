#include "imagebubble.h"
#include <QLabel>

#define IMAGE_WIDTH 160
#define IMAGE_HEIGHT 90

ImageBubble::ImageBubble(const QPixmap& pixmap, Role role, QWidget* parent)
    : BubbleFrameBase(role, parent)
{
    QLabel* pLable = new QLabel();
    pLable->setScaledContents(true);
    QPixmap pix = pixmap.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT), Qt::KeepAspectRatio);
    pLable->setPixmap(pix);
    this->setWidget(pLable);

    int nLeftMargin = this->layout()->contentsMargins().left();
    int nRightMargin = this->layout()->contentsMargins().right();
    int nBottomMargin = this->layout()->contentsMargins().bottom();
    setFixedSize(pix.width() + nLeftMargin + nRightMargin, pix.height() + 2 * nBottomMargin);
}
