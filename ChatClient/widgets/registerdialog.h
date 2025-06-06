#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_btn_GetCode_clicked();

private:
    void showTip(QString str, bool isError);
    void initHandlers();

private:
    void slot_register_finished(RequstID id, QString res, ErrorCodes ec);

private:
    Ui::RegisterDialog *ui;
    QMap<RequstID, std::function<void(const QJsonObject&)>> m_handlers;
};

#endif // REGISTERDIALOG_H
