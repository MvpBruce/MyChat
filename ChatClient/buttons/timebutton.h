#ifndef TIMEBUTTON_H
#define TIMEBUTTON_H
#include <QPushButton>
#include <QTimer>

class TimeButton : public QPushButton
{
public:
    TimeButton(QWidget *parent = nullptr);
    ~TimeButton();

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QTimer* m_pTimer;
    int m_nCount;
};

#endif // TIMEBUTTON_H
