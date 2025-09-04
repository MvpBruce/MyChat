#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include <QObject>
#include <QWidget>

class SearchInfo;
class QDialog;
class SearchList : public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* pEdit);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void AddTipItem();
    void WaitPending(bool bPending = true);

private:
    std::shared_ptr<QDialog> m_pFindDlg;
    QWidget* m_pSearchEdit;
    bool m_bPending;

private slots:
    void slot_item_clicked(QListWidgetItem* pItem);
    void slot_user_search(std::shared_ptr<SearchInfo> pSearchInfo);
};

#endif // SEARCHLIST_H
