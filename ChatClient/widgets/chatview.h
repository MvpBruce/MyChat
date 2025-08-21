#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QEvent>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);
    void appendChild(QWidget* widget);
protected:
    bool eventFilter(QObject *o, QEvent *e) override;

private slots:
    void onVScrollChanged(int min, int max);

private:
    bool m_bAppended;
    QScrollArea* m_pScrollArea;
};

#endif // CHATVIEW_H
