#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QMainWindow>
#include <QItemDelegate>

class listViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit listViewDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

};

#endif // LISTVIEWDELEGATE_H
