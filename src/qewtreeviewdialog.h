//
// C++ Interface: qewtreeviewdialog
//
// Description:
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QEWTREEVIEWDIALOG_H
#define QEWTREEVIEWDIALOG_H

#include <QHash>
#include <QStringList>
#include <QByteArray>
#include <QTreeWidget>
#include "qewextensibledialog.h"

class QStackedWidget;

class QDESIGNER_WIDGET_EXPORT QewTreeViewDialog : public QewExtensibleDialog
{
    Q_OBJECT
    Q_PROPERTY(QByteArray treeNavigatorName READ treeNavigatorName WRITE setTreeNavigatorName DESIGNABLE true SCRIPTABLE true STORED true)

    friend class QewDialogFactory;
    friend class QewTreeViewDialogPlugin;

public:
    QewTreeViewDialog(QWidget *w_parent = 0, const char *name = 0, bool sameDialogForAllChildren = false);

public:
    ~QewTreeViewDialog();

    bool setUpDialog(void);
    void setTreeNavigatorName(const QByteArray& navigat_name) { navigator_name =  navigat_name ; }
    const QByteArray& treeNavigatorName(void) const { return navigator_name ; }

    void setExpandedAtInit(bool expandedAtInit);

    QStringList getDesignedCaptions() { return designed_captions; }

public slots:
    virtual void changeCurrentItemText(const QString &text);
    virtual void changeItemText(QTreeWidgetItem*item, const QString &text);
    virtual void setCurrentWidget(QTreeWidgetItem* itemToShow);    

protected:
    uint pagesNumber(void);
    QList<QWidget*> getContainerPages(void);
    QWidget* addPage(QewExtensibleDialog* extensible_child, const QString& caption,
                     QTreeWidgetItem* parentItem = 0, int index = -1);
    QWidget* addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                         QewExtensibleDialog* w_page, const QString& caption,
                         QTreeWidgetItem* parentItem = 0, int index = -1);
    QewExtensibleDialog* newPage(const QString& caption, const QByteArray &cont_name);
    void removePage(QewExtensibleDialog* extensible_child, QTreeWidgetItem* parentItem = 0);
    bool setContainerWidget(void);

    void insertChildrenInTree(QewExtensibleDialog* extensible_child, QTreeWidgetItem* item, QHash<void*, QWidget*> & descendants_dict);
    void removeChildrenFromTree(QewExtensibleDialog* extensible_child, QHash<void*, QWidget*> & descendants_dict);

    bool setTreeNavigator(void);
    virtual QStringList designedCaptions(void) {return QStringList("") ; }
    void setDesignedCaptions(void);

    virtual void currentItemTextNotChanged(const QString &/*text*/) {}
    virtual void currentItemTextChanged(const QString &/*text*/) {}

    int childExists(const QString &text, QTreeWidgetItem* parent = 0, QTreeWidgetItem *item = 0);
    void removeItemFromTree(QTreeWidgetItem* item, bool alsoWidgets = false);

    QTreeWidget* getSuperTreeView(QewTreeViewDialog **extensible_parent);

protected:
   QTreeWidget* getTreeView() { return tree_view; }
   QWidget* getCurrentWidget();

protected:
    QHash<void*, QWidget*> descendantWidgets;

private:
    QStackedWidget* containerWidgetStack;
    QTreeWidget* tree_view;
//    QHash<void*, QWidget*> descendantWidgets;
    QByteArray navigator_name;
    QStringList designed_captions;
    QTreeWidgetItem* currentItem;
    bool expandedAtInit;    
};

#endif
