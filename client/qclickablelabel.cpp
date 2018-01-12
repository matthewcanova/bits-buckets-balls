#include "qclickablelabel.h"

QClickableLabel::QClickableLabel( QWidget* parent)
    : QLabel(parent)
{
}

QClickableLabel::~QClickableLabel()
{
}

void QClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked(this->objectName());
}

