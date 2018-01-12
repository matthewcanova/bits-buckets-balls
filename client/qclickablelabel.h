#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <QObject>
#include <QLabel>

class QClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit QClickableLabel(QWidget* parent=0 );
    ~QClickableLabel();
signals:
    void clicked(QString label);

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // QCLICKABLELABEL_H
