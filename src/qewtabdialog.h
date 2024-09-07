//
// C++ Interface: qewtabdialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QewTABDIALOG_H
#define QewTABDIALOG_H

#include <QTabWidget>
#include "qewextensibledialog.h"

class QTreeWidgetItem;

class QDESIGNER_WIDGET_EXPORT QewTabDialog : public QewExtensibleDialog
{ 
    Q_OBJECT
    friend class QewDialogFactory;
    friend class QewTabDialogPlugin;
    
public:
    QewTabDialog(QWidget *w_parent = 0, const char *name = 0);
    
public:
    ~QewTabDialog();

    bool setUpDialog(void);

protected:
    uint pagesNumber(void);
    QList<QWidget*> getContainerPages(void);
    QWidget* addPage(QewExtensibleDialog* extensible_child, const QString& caption,
                     QTreeWidgetItem* parentItem = 0, int index = -1);
    QWidget* addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                         QewExtensibleDialog* w_page, const QString& cap,
                         QTreeWidgetItem* parentItem = 0, int index = -1);
    QewExtensibleDialog* newPage(const QString& caption, const QByteArray &cont_name);
    void removePage(QewExtensibleDialog* extensible_child, QTreeWidgetItem* parentItem = 0);
    bool setContainerWidget(void);

private:
    QTabWidget* containerTabWidget;
};

#endif
