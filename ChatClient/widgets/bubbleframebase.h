#ifndef BUBBLEFRAMEBASE_H
#define BUBBLEFRAMEBASE_H

#include <QFrame>
#include <QWidget>
#include <QHBoxLayout>
#include "global/global.h"

class BubbleFrameBase : public QFrame
{
    Q_OBJECT
public:
    BubbleFrameBase(Role role, QWidget* parent = nullptr);
    void setMargin(int nMargin);
    void setWidget(QWidget* widget);

protected:
    void paintEvent(QPaintEvent* e);

private:
    QHBoxLayout* m_pHLayout;
    Role m_Role;
    int m_nMargin;

    static int s_nWidth;
};

#endif // BUBBLEFRAMEBASE_H
