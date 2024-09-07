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

#ifndef QEWSIMPLEDIALOGFORM_H
#define QEWSIMPLEDIALOGFORM_H

#include "qewsimpledialog.h"

namespace Ui
{
class QewSimpleDialogForm;
}

class QDESIGNER_WIDGET_EXPORT QewSimpleDialogForm : public QewSimpleDialog
{
    Q_OBJECT

public:
    QewSimpleDialogForm(QWidget* parent = 0, const char* name = 0);
    ~QewSimpleDialogForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QewSimpleDialogForm* ui;

};

#endif // QEWSIMPLEDIALOGFORM_H
