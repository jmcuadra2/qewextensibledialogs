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

#include "qewtabdialogform.h"
#include "ui_qewtabdialogform.h"

/*
 *  Constructs a QewTabDialogForm which is a child of 'parent', with the
 *  name 'name'.'
 */
QewTabDialogForm::QewTabDialogForm(QWidget* parent, const char* name)
    : QewTabDialog(parent, name)
{
    ui = new Ui::QewTabDialogForm;
    ui->setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QewTabDialogForm::~QewTabDialogForm()
{
    delete ui;
}


void QewTabDialogForm::changeEvent(QEvent *e)
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
