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

#include "qewtoolboxdialogform.h"
#include "ui_qewtoolboxdialogform.h"

/*
 *  Constructs a QewToolBoxDialogForm which is a child of 'parent', with the
 *  name 'name'.'
 */
QewToolBoxDialogForm::QewToolBoxDialogForm(QWidget* parent, const char* name)
    : QewToolBoxDialog(parent, name)
{
    ui = new Ui::QewToolBoxDialogForm;
    ui->setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QewToolBoxDialogForm::~QewToolBoxDialogForm()
{
    delete ui;
}

void QewToolBoxDialogForm::changeEvent(QEvent *e)
{
    QewExtensibleDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

