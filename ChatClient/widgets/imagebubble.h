#ifndef IMAGEBUBBLE_H
#define IMAGEBUBBLE_H

#include <QWidget>
#include "bubbleframebase.h"

class ImageBubble : public BubbleFrameBase
{
    Q_OBJECT
public:
    ImageBubble(const QPixmap& pixmap, Role role, QWidget* parent = nullptr);
};

#endif // IMAGEBUBBLE_H
