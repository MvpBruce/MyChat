#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include <QWidget>
#include <QTextEdit>
#include "bubbleframebase.h"

class TextBubble : public BubbleFrameBase
{
    Q_OBJECT
public:
    TextBubble(Role role, const QString& text, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* o, QEvent* e);

private:
    void setChatText(const QString &text);
    void adjustTextHeight();

private:
    QTextEdit* m_pTextEdit;
};

#endif // TEXTBUBBLE_H
