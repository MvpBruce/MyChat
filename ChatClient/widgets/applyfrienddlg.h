#ifndef APPLYFRIENDDLG_H
#define APPLYFRIENDDLG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class ApplyFriendDlg;
}

class SearchInfo;
class StateLabel;
class ApplyFriendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriendDlg(QWidget *parent = nullptr);
    ~ApplyFriendDlg();

    void InitTipLabels();
    void AddTipLabels(StateLabel* pLable, QPoint curPoint, QPoint& nextPoint, int nWidth, int nHeight);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void SetSearchInfo(std::shared_ptr<SearchInfo> pSearchInfo);

public slots:
    void slot_show_more();
    void slot_change_tip(QString text, ClickState state);
    void slot_apply_ok();
    void slot_apply_cancel();

private:
    void ResetLabesl();

private:
    Ui::ApplyFriendDlg *ui;
    QPoint m_curTipPoint;
    QPoint m_labelPoint;
    std::vector<QString> m_tipData;
    QMap<QString, StateLabel*> m_mapTips;
    std::shared_ptr<SearchInfo> m_pSearchInfo;
};

#endif // APPLYFRIENDDLG_H
