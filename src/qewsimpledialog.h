//
// C++ Interface: qewsimpledialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QEWSIMPLEDIALOG_H
#define QEWSIMPLEDIALOG_H

#include "qewextensibledialog.h"
#include <QFrame>

class QTreeWidgetItem;

class QDESIGNER_WIDGET_EXPORT QewSimpleDialog : public QewExtensibleDialog
{

    Q_OBJECT

    friend class QewDialogFactory;
    friend class QewSimpleDialogPlugin;
    
public:
    QewSimpleDialog(QWidget *w_parent = 0, const char *name = 0);
public:
    ~QewSimpleDialog();
    
    bool setUpDialog(void);
    
    void setCountInitPages(bool countInitPages);

protected:
    uint pagesNumber(void);
    QWidget* addPage(QewExtensibleDialog* extensible_child, const QString& caption,
                     QTreeWidgetItem* parentItem = 0, int index = -1);
    QWidget* addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                         QewExtensibleDialog* w_page, const QString& caption,
                         QTreeWidgetItem* parentItem = 0, int index = -1);
    QewExtensibleDialog* newPage(const QString& caption, const QByteArray& cont_name);    
    void removePage(QewExtensibleDialog* extensible_child, QTreeWidgetItem* parentItem = 0);

    bool setContainerWidget(void);
    void removeLayoutMargin(void);

private:
    QFrame* containerFrame;
    int init_num_pages;
    bool countInitPages;

};

#endif
