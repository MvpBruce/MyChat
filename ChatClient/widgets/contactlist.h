#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QListWidget>

class UserContactItem;
class ContactList : public QListWidget
{
    Q_OBJECT
public:
    ContactList(QWidget* parent = nullptr);
    void ShowRedPoint(bool bShow = true);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void AddContactList();

public slots:
    void slot_item_clicked(QListWidgetItem* item);

signals:
    void sig_load_contacts();

private:
    UserContactItem* m_pContactItem;
};

#endif // CONTACTLIST_H
