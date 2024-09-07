#include "qewtreepage.h"

QewTreePage::QewTreePage(QWidget *parent) : QWidget(parent)
{

}

QewTreePage::~QewTreePage()
{

}

void QewTreePage::enterEvent(QEvent */*event*/)
{
    emit emitActivated(true, windowTitle());
}

void QewTreePage::leaveEvent(QEvent */*event*/)
{
    emit emitActivated(false, windowTitle());
}
