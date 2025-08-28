#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "global/global.h"

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);
    void SetState(QString normal, QString hover, QString press, QString select, QString selectHover, QString selectPress);

    WidgetState GetState();
    void ClearState();

    void SetSelected(bool bSelected);
    void AddRedPoint();
    void ShowRedPoint(bool bShow = true);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    QString m_strNormal;
    QString m_strNormalHover;
    QString m_strNormalPress;

    QString m_strSelected;
    QString m_strSelectedHover;
    QString m_strSelectedPress;

    WidgetState m_curState;
    QLabel* m_pReadPopint;

signals:
    void clicked();
};

#endif // STATEWIDGET_H
