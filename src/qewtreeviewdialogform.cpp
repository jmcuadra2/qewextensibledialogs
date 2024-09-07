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

#include "qewtreeviewdialogform.h"
#include "ui_qewtreeviewdialogform.h"

/*
 *  Constructs a QewTreeViewDialogForm which is a child of 'parent', with the
 *  name 'name'.'
 */
QewTreeViewDialogForm::QewTreeViewDialogForm(QWidget* parent, const char* name)
    : QewTreeViewDialog(parent, name)
{
    ui = new Ui::QewTreeViewDialogForm;
    ui->setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QewTreeViewDialogForm::~QewTreeViewDialogForm()
{
    delete ui;
}

void QewTreeViewDialogForm::changeEvent(QEvent *e)
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
