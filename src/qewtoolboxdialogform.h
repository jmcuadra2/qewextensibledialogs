//
// C++ Implementation: qewsimpledialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QEWTOOLBOXDIALOGFORM_H
#define QEWTOOLBOXDIALOGFORM_H

#include "qewtoolboxdialog.h"

namespace Ui
{
class QewToolBoxDialogForm;
}

class QDESIGNER_WIDGET_EXPORT QewToolBoxDialogForm : public QewToolBoxDialog
{
    Q_OBJECT

public:
    QewToolBoxDialogForm(QWidget* parent = 0, const char* name = 0);
    ~QewToolBoxDialogForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QewToolBoxDialogForm* ui;

};

#endif // QEWTOOLBOXDIALOGFORM_H
