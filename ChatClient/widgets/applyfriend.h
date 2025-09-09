#ifndef APPLYFRIEND_H
#define APPLYFRIEND_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class ApplyFriend;
}

class SearchInfo;
class StateLabel;
class ApplyFriend : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriend(QWidget *parent = nullptr);
    ~ApplyFriend();

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
    Ui::ApplyFriend *ui;
    QPoint m_curTipPoint;
    QPoint m_labelPoint;
    std::vector<QString> m_tipData;
    QMap<QString, StateLabel*> m_mapTips;
    std::shared_ptr<SearchInfo> m_pSearchInfo;
};

#endif // APPLYFRIEND_H
