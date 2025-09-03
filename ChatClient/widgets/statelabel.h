#ifndef STATELABEL_H
#define STATELABEL_H

#include <QLabel>
#include "global/global.h"

class StateLabel : public QLabel
{
    Q_OBJECT
public:
    StateLabel(QWidget* parent = nullptr);
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    void SetState(QString normal="", QString hover="", QString press="", QString select="", QString selectHover="", QString selectPress="");

    ClickState GetCurState();
    void SetCurState(ClickState state);
    void ResetNormalState();

private:
    QString m_strNormal;
    QString m_strNormalHover;
    QString m_strNormalPress;

    QString m_strSelected;
    QString m_strSelectedHover;
    QString m_strSelectedPress;
    ClickState m_curState;

signals:
    void clicked(QString, ClickState);
};

#endif // STATELABEL_H
