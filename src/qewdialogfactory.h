//
// C++ Interface: qewdialogfactory
//
// Description: 
//
//
// Author: Jose M. Cuadra <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QewDIALOGFACTORY_H
#define QewDIALOGFACTORY_H

#include "qewextensibledialog.h"

class QewDialogFactory{

public:
    QewDialogFactory();
    ~QewDialogFactory();
    
    QewExtensibleDialog* createDialog(QewExtensibleDialog::QewContainerType cont_type, QWidget *w_parent = 0, bool connect = true,
                         bool withButtons = true, const char *name = 0, const QByteArray &cont_name = "");

};

#endif
