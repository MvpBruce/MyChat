#ifndef TIPLABEL_H
#define TIPLABEL_H

#include <QLabel>

class TipLabel : public QLabel
{
    Q_OBJECT
public:
    TipLabel(QWidget* parent = nullptr);
    void mouseReleaseEvent(QMouseEvent *ev) override;

signals:
    void clicked(QString);
};

#endif // TIPLABEL_H
