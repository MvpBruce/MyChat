#ifndef STATEBTN_H
#define STATEBTN_H

#include <QObject>
#include <QPushButton>
//#include <QEvent>

class StateBtn : public QPushButton
{
    Q_OBJECT
public:
    StateBtn(QWidget* parent = nullptr);
    ~StateBtn();
    void SetState(QString normal, QString hover, QString press);

protected:
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    QString m_strNormal;
    QString m_strHover;
    QString m_strPress;
};

#endif // STATEBTN_H
