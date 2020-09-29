#include "listviewdelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>

listViewDelegate::listViewDelegate(QObject  *parent)
    : QItemDelegate(parent)
{

}

void listViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
 {
    QItemDelegate::paint(painter, option, index);
      if(index.row()==8)//since i have to make it display only at (8,0) position .
     {
      if(index.column()==0)
      {
         QStyleOptionButton button;
         QRect r = option.rect;//getting the rect of the cell
         int x,y,w,h;
         x = r.left() + r.width() - 20;//the X coordinate
         y = r.top();//the Y coordinate
         w = 15;//button width(based on the requirement)
         h = 15;//button height(based on the requirement)
      button.icon= QIcon(QString::fromUtf8("Resources/HelpIcon.png"));
      button.iconSize = QSize(20,20);
         button.rect = QRect(x,y,w,h);
         button.text = "";//no text . since if text will be given then it will push the icon to left side based on the coordinates .
         button.state = QStyle::State_Enabled;

         //QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);

      QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .

      }
      }
}

 bool listViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {
     if( event->type() == QEvent::MouseButtonRelease )
     {
         QMouseEvent * e = (QMouseEvent *)event;
         int clickX = e->x();
         int clickY = e->y();

         QRect r = option.rect;//getting the rect of the cell
         int x,y,w,h;
         x = r.left() + r.width() - 30;//the X coordinate
         y = r.top();//the Y coordinate
         w = 30;//button width
         h = 30;//button height

         if( clickX > x && clickX < x + w )
             if( clickY > y && clickY < y + h )
             {
                 QDialog * d = new QDialog();
                 d->setGeometry(0,0,100,100);
                 d->show();
             }
     }

     return true;
 }
