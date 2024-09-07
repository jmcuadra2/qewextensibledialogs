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

#include "qewsimpledialogform.h"
#include "ui_qewsimpledialogform.h"

/*
 *  Constructs a QewSimpleDialogForm which is a child of 'parent', with the
 *  name 'name'.'
 */
QewSimpleDialogForm::QewSimpleDialogForm(QWidget* parent, const char* name)
    : QewSimpleDialog(parent, name)
{
    ui = new Ui::QewSimpleDialogForm;
    ui->setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QewSimpleDialogForm::~QewSimpleDialogForm()
{
    delete ui;
}

void QewSimpleDialogForm::changeEvent(QEvent *e)
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

