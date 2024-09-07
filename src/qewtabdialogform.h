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

#ifndef QEWTABDIALOGFORM_H
#define QEWTABDIALOGFORM_H

#include "qewtabdialog.h"

namespace Ui
{
class QewTabDialogForm;
}

class QDESIGNER_WIDGET_EXPORT QewTabDialogForm : public QewTabDialog
{
    Q_OBJECT

public:
    QewTabDialogForm(QWidget* parent = 0, const char* name = 0);
    ~QewTabDialogForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QewTabDialogForm* ui;

};

#endif // QEWTABDIALOGFORM_H
