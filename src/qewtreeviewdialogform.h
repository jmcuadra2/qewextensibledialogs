#ifndef QEWTREEVIEWDIALOGFORM_H
#define QEWTREEVIEWDIALOGFORM_H

#include "qewtreeviewdialog.h"

namespace Ui
{
class QewTreeViewDialogForm;
}

class QDESIGNER_WIDGET_EXPORT QewTreeViewDialogForm : public QewTreeViewDialog
{
    Q_OBJECT

public:
    QewTreeViewDialogForm(QWidget* parent = 0, const char* name = 0);
    ~QewTreeViewDialogForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QewTreeViewDialogForm* ui;

};

#endif // QEWTREEVIEWDIALOGFORM_H
