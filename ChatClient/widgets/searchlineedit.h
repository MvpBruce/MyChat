#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>
#include <QObject>

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchLineEdit(QWidget* parent = nullptr);
    void setMaxLength(int nMaxLen);

protected:
    void focusOutEvent(QFocusEvent *event) override;

private:
    void limitTextLength(QString text);

signals:
    void sig_foucus_out();

private:
    int m_MaxLen;
};

#endif // SEARCHLINEEDIT_H
