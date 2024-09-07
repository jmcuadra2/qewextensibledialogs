//
// C++ Implementation: qewtreeviewdialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>

#include <QTreeWidget>
//#include <Qt/qobject.h>
//#include <QtGui/qlayout.h>
//#include <QtGui/QStackedWidget>
//#include <QtGui/qtooltip.h>
#include <QStackedWidget>
#include <QtDebug>

#include "qewtreeviewdialog.h"
#include <QVBoxLayout>

/*!
    \class QewTreeViewDialog qewtreeviewdialog.h
    \brief This is the base class for the QewTreeViewDialog branch.
    
    This class show its qew-children in individual pages within nested widgets stacks (QWidgetStack). This class, branch, is the library's jewell. It allows deep nesting of qew-dialogs without crowding main dialog with tabs for navigation, through which users can get lost, and without increasing main dialog size becuase of tabs. For navigation this class use a QTreeWidget where users can select, doubleclicking, the page they want to edit. Only qew-dialogs intended to be top level have to get this navigator. For more information see \ref ui_templat "qew-templates".
    
    \author Jose M. Cuadra
*/

/*!
    \property QewTreeViewDialog::treeNavigatorName
    This property has the tree navigator's name. Set this property's value with setTreeNavigatorName() and get this property's value with treeNavigatorName(). Normally you set this property at designing time with Qt Designer.
*/

/*!
    \fn QewTreeViewDialog::treeNavigatorName(void) const
    \brief Gets tree navigator's name property.
    
    QewTreeViewDialog::treeNavigatorName property is only necessary to make a valid top level dialog.
    
    @return The name of the tree navigator.
*/

/*!
    \fn QewTreeViewDialog::setTreeNavigatorName(const QCString&)
    \brief Sets tree navigator's name.
    
    Sets the QewTreeViewDialog::treeNavigatorName property, normally you set this property designing the form, in other case you have to set this name after construction, before calling any other function for this object. It is only necessary for top level dialogs.
    
    @param navigat_name Name for tree navigator.
    
*/

/*!
    \brief The constructor.

    Calls base class constructor QewExtensibleDialog::QewExtensibleDialog().
*/

QewTreeViewDialog::QewTreeViewDialog(QWidget *w_parent, const char *name, bool sameDialogForAllChildren)
    : QewExtensibleDialog(w_parent, name)
{
    containerWidgetStack = 0;
    tree_view = 0;
    currentItem = NULL;
    expandedAtInit = true;
    setSetUpOk(false);
    setSameDialogForAllChildren(sameDialogForAllChildren);

}

/*!
    \brief The destructor.

    Calls base class destructor QewExtensibleDialog::~QewExtensibleDialog().
*/

QewTreeViewDialog::~QewTreeViewDialog()
{
}

/*!
    \brief Sets the qew-dialog for use.
    
    Reads properties an sets the corresponding objects: a container widget, from QWidgetStack class, and tree navigator, from QTreeWidget class, if it is necessary, and tries to make a valid qew-dialog.
    
    @return True if the qew-dialog could be set as a valid qew-dialog.
*/

bool QewTreeViewDialog::setUpDialog(void)
{
    bool ret = false;
    ret = setContainerWidget();
    if(ret) {
        setButtonsWidget();
        removeLayoutMargin();
        if(!sameDialogForAllChildren) {
            containerWidgetStack->setCurrentIndex(0); // always before setDesignedCaptions()
            setDesignedCaptions();
        }
        if(QewParent())
            setTreeNavigator();
        else {
            ret = setTreeNavigator();
            if(!ret)
                errMsg(QewNoTreeNavigator);
        }
    }
    setSetUpOk(ret);
    return ret;
}

/*!
    \brief Looks for tree navigator and connects it.
    
    This function looks for a QTreeWidget with name set by QewTreeViewDialog::treeNavigatorName property and, if succes, it inserts captions from qew-children and their children pages recursively in the QTreeWidget.

    @return True is tree navigator was found, false otherwise.
    \sa insertChildrenInTree().
 */

bool QewTreeViewDialog::setTreeNavigator(void)
{
    bool ret = false;
    if((ret = findWidget(treeNavigatorName(), "QTreeWidget"))) {
        tree_view = findChild<QTreeWidget*>(treeNavigatorName());
        if(QewParent() && QewParent()->inherits("QewTreeViewDialog")) {
            delete tree_view;
            tree_view = 0;
        }
        else {
            QTreeWidgetItem* top_item = tree_view->topLevelItem(0);
            insertChildrenInTree(this, top_item, descendantWidgets);
        }
        // currentItem is unexplainably changed at subclasses setupUi()
        currentItem = 0;
    }
    return ret;
}

/*!
    \brief Sets the container widget.

    This function looks for a QWidgetStack with name set by QewExtensibleDialog::containerName property and sets it. It can send error message to users.
    
    @return True if container widget was found and it inherits QWidgetStack, false otherwise.
*/ 

bool QewTreeViewDialog::setContainerWidget(void)
{

    bool ret = findWidget(containerName(), "");
    if(!ret)
        errMsg(QewContainerMissing);
    else {
        if((ret = findWidget(containerName(), "QStackedWidget"))) {
            containerWidgetStack = findChild<QStackedWidget*>(containerName());
            setBaseContainerWidget((QWidget*)containerWidgetStack);
        }
        else
            errMsg(QewWrongContainer);
    }
    return ret;
}

/*!
    \brief Number of top-level pages.
    
    Returns the number of top-level pages, i.e., the number of visible children widgets from container widget.
    
    @return The number of top-level pages.
*/ 

uint QewTreeViewDialog::pagesNumber(void)
{
    return getContainerPages().count();
}

/*!
    \brief Gets container widget pages.
    
    Gets container widget visible children widgets corresponding to dialog pages.

    @return A list of container pages.

    \warning This function is dependent on an internal Qt name: "qt_invisible_widgetstack" see qwidgetstack.cpp.
*/

QList<QWidget*> QewTreeViewDialog::getContainerPages(void)
{
    QList<QWidget*> cont_pages;
    QString nname;
    if(containerWidgetStack && !getSameDialogForAllChildren()) {
        const QList<QObject *> o_list = containerWidgetStack->children();
        if(!o_list.isEmpty()) {
            QListIterator<QObject *> it_o(o_list);
            it_o.toBack();
            while(it_o.hasPrevious()) {
                QObject* o_children = it_o.previous();
                if(o_children->isWidgetType()){
                    QWidget* s_widget = (QWidget*)o_children;
                    nname = s_widget->objectName();
                    if(!nname.isEmpty())
                        cont_pages.append(s_widget);
                }
            }
//            while(it_o.hasNext()) {
//                QObject* o_children = it_o.next();
//                if(o_children->isWidgetType()){
//                    QWidget* s_widget = (QWidget*)o_children;
//                    nname = s_widget->objectName();
//                    if(!nname.isEmpty())
//                        cont_pages.append(s_widget);
//                }
//            }
        }
    }
    return cont_pages;
}

/*!
    \brief Add an individual page.
    
    Adds a qew-child, as a widget, to container widget. It is intended to be invoked from addExtensibleChild(). It also adds qew-child caption to tree navigator.
    
    @param extensible_child A pointer to qew-child to add.
    @param caption A caption for \a extensible_child.
    @return A pointer to \a extensible_child or 0 if it could not be added.
*/

QWidget* QewTreeViewDialog::addPage(QewExtensibleDialog* extensible_child,
                              const QString& caption, QTreeWidgetItem *parentItem, int index)
{
    QWidget* page = 0;
    if(containerWidgetStack) {
        containerWidgetStack->addWidget(extensible_child);
        page = extensible_child;
        extensible_child->setWindowTitle(caption);
        QewTreeViewDialog* extensible_parent = 0;
        QTreeWidget* superTreeView = 0;
        if(tree_view) {
            extensible_parent = this;
            if(!parentItem) {
                if(tree_view->currentItem())
                    parentItem = tree_view->currentItem();
                else
                    parentItem = tree_view->topLevelItem(tree_view->topLevelItemCount() - 1);
            }
        }
        else {
            superTreeView = getSuperTreeView(&extensible_parent);
            if(!parentItem) {
                if(superTreeView && superTreeView->currentItem() != 0)
                    parentItem = superTreeView->currentItem();
            }
        }

        if(parentItem) {
//            QTreeWidgetItem* top_item = tree_view->topLevelItem(0);

//            QTreeWidgetItem* top_item = superTreeView->topLevelItem(superTreeView->topLevelItemCount() - 1);
//            QTreeWidgetItem* parentItem = superTreeView->currentItem() != 0 ? superTreeView->currentItem() : top_item;
            QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(*parentItem);

//            tree_view->addTopLevelItem(itemToAdd);
            if(index > -1)
                parentItem->insertChild(index, itemToAdd);
            else
                parentItem->addChild(itemToAdd);
            itemToAdd->setText(0, caption);
            itemToAdd->setExpanded(expandedAtInit);
            extensible_parent->insertChildrenInTree(extensible_child, itemToAdd, extensible_parent->descendantWidgets);
        }
    }
    else
        errMsg(QewContainerMissing);
    return page;
}

/*!
    \brief Adds a list of pages.
    
    Adds a qew-children list, as widgets, to container widget. It creates a new child QewTreeViewDialog, setting its caption, and it inserts qew-children from list as children of this new child. Qew-children from list, as QWidgets, are inserted as pages in new child's container widget of QStackedWidget class. It also adds qew-children captions to tree navigator. It is intended to be invoked from addExtensibleChildrenList().
    
    @param extensible_children_list The qew-children list to add.
    @param w_page The empty QewTreeViewDialog created with newPage(), child of \b this, that is go to be the real parent of each qew_dialog from \a extensible_children_list.
    @param caption A text for tree item asociated to \a extensible_children_list.
    @return A pointer to actual QStackedWidget where qew_dialogs from \a extensible_children_list was added or 0 if list could not be added.
*/

QWidget* QewTreeViewDialog::addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                                        QewExtensibleDialog* w_page, const QString& caption,
                                        QTreeWidgetItem *parentItem, int index)
{
    QStackedWidget* page = 0;
    if(containerWidgetStack) {

        if(w_page->containerWidget()) {
            if(w_page->containerWidget()->inherits("QStackedWidget")) {

                QewTreeViewDialog* extensible_parent = 0;
                QTreeWidget* superTreeView = 0;

                page = (QStackedWidget*)w_page->containerWidget();
                w_page->layout()->setContentsMargins(0, 0, 0, 0);
                containerWidgetStack->addWidget(w_page);

                QListIterator<QewExtensibleDialog*> it(extensible_children_list);
                while (it.hasNext()) {
                    QewExtensibleDialog* qew = it.next();
                    qew->setModal(false);
                    page->addWidget(qew);
                }

                if(tree_view) {
                    extensible_parent = this;
                    if(!parentItem) {
                        if(tree_view->currentItem())
                            parentItem = tree_view->currentItem();
                        else
                            parentItem = tree_view->topLevelItem(tree_view->topLevelItemCount() - 1);
                    }
                }
                else {
                    superTreeView = getSuperTreeView(&extensible_parent);
                    if(!parentItem) {
                        if(superTreeView && superTreeView->currentItem() != 0)
                            parentItem = superTreeView->currentItem();
                    }
                }

                if(parentItem) {
                    QTreeWidgetItem* itemToAdd = 0;
//                    if(it.hasNext()) {
                        itemToAdd = new QTreeWidgetItem(parentItem);
                        if(index > -1)
                            parentItem->insertChild(index, itemToAdd);
                        else
                            parentItem->addChild(itemToAdd);
                        itemToAdd->setText(0, caption);
                        itemToAdd->setExpanded(expandedAtInit);
                        extensible_parent->descendantWidgets.insert(itemToAdd, page);
//                    }

                    int i = 1;
                    it.toFront();
                    while (it.hasNext()) {
                        QewExtensibleDialog* qew = it.next();
                        QString cap = qew->windowTitle().isEmpty()
                                ? QString(qew->objectName()) : qew->windowTitle();
                        cap = cap.isEmpty() ? (caption + " " + QString::number(i)) : cap;
                        QTreeWidgetItem* item = new QTreeWidgetItem(itemToAdd);
                        parentItem->addChild(itemToAdd);
                        item->setText(0, cap);
                        item->setExpanded(expandedAtInit);
                        insertChildrenInTree(qew, item, extensible_parent->descendantWidgets);
                        ++i;
                    }
                }

//                if(tree_view) {
////                    QTreeWidgetItem* top_item = tree_view->topLevelItem(0);
//                    QTreeWidgetItem* top_item = tree_view->
//                            topLevelItem(tree_view->topLevelItemCount() - 1);
//                    QTreeWidgetItem* itemToAdd = 0;
////                    if(it.hasNext()) {
//                        itemToAdd = new QTreeWidgetItem(top_item);
//                        tree_view->addTopLevelItem(itemToAdd);
//                        itemToAdd->setText(0, caption);
//                        itemToAdd->setExpanded(expandedAtInit);
//                        descendantWidgets.insert(itemToAdd, page);
////                    }

//                    int i = 1;
//                    it.toFront();
//                    while (it.hasNext()) {
//                        QewExtensibleDialog* qew = it.next();
//                        QString cap = qew->windowTitle().isEmpty()
//                                ? QString(qew->objectName()) : qew->windowTitle();
//                        cap = cap.isEmpty() ? (caption + " " + QString::number(i)) : cap;
//                        QTreeWidgetItem* item = new QTreeWidgetItem(itemToAdd);
//                        tree_view->addTopLevelItem(item);
//                        item->setText(0, cap);
//                        item->setExpanded(expandedAtInit);
//                        insertChildrenInTree(qew, item, descendantWidgets);
//                        ++i;
//                    }
//                }
            }
        }
        else
            w_page->errMsg(QewContainerMissing);
    }
    else
        errMsg(QewContainerMissing);
    return (QWidget*)page;
}

/*!
    \brief Creates an empty page where you can add qew_dialogs lists.
    
    Creates a qew-dialog of QewTreeViewDialogEmpty class, with only an empty QStackedWidget as container widget. It is intended to be invoked from addExtensibleChildrenList().
    
    @param caption A caption for the new qew-dialog.
    @return A pointer to created qew-dialog or 0 if it could not be set up.

*/

QewExtensibleDialog* QewTreeViewDialog::newPage(const QString& caption, const QByteArray &cont_name)
{ 
    QewExtensibleDialog* w_page = 0;
    QStackedWidget* page = 0;
    if(containerWidgetStack) {
        QewTreeViewDialog* new_page = new QewTreeViewDialog(this);
        page = new QStackedWidget(new_page);        
        if(cont_name.trimmed().isEmpty()) {
            page->setObjectName("QewContainerWidgetStack");
            new_page->setContainerName("QewContainerWidgetStack");
        }
        else {
            page->setObjectName(cont_name);
            new_page->setContainerName(cont_name);
        }
        new_page->setTreeNavigatorName("QewListView");
        if(new_page->setUpDialog()) {
            w_page = new_page;
            w_page->setWindowTitle(caption);
            QVBoxLayout* wLayout = new QVBoxLayout(w_page);
            wLayout->addWidget(page);
        }
        else
            delete new_page;
    }
    else
        errMsg(QewContainerMissing);
    return w_page;
}

void QewTreeViewDialog::removePage(QewExtensibleDialog *extensible_child, QTreeWidgetItem *parentItem)
{
    if(containerWidgetStack) {

        QewTreeViewDialog* extensible_parent = 0;
        QTreeWidget* superTreeView = 0;
        if(tree_view) {
            extensible_parent = this;
        }
        else {
            superTreeView = getSuperTreeView(&extensible_parent);
            if(!parentItem) {
                if(superTreeView && superTreeView->currentItem() != 0)
                    parentItem = superTreeView->currentItem();
            }
        }

        if(parentItem) {
            containerWidgetStack->removeWidget(extensible_child);

            extensible_parent->removeChildrenFromTree(extensible_child, extensible_parent->descendantWidgets);
        }
        delete extensible_child;
    }
}

/*!
    \brief Raises a qew-dialog page when user doubleclicks on correspondent tree navigator item.
    
    This slot is connected to QTreeWidget signal doubleClicked() in order to raise correspondent page in dialog.
    
    @param itemToShow A pointer to doubleclicked QTreeWidget item.
    
*/

void QewTreeViewDialog::setCurrentWidget(QTreeWidgetItem* itemToShow)
{
    if(!itemToShow)
        return;
    if(!containerWidgetStack) {
        errMsg(QewContainerMissing);
        return;
    }
    currentItem = itemToShow;
    QWidget* w_child = descendantWidgets[currentItem];
    QWidget* w_parent = 0;
    QList<QWidget*> stacked_parents;
    QList<QWidget*> stacked_children;
    while(w_child && (w_child != containerWidgetStack)) {
        w_parent = w_child->parentWidget();
        if(w_parent) {
            if(w_parent->inherits("QStackedWidget")) {
                stacked_parents.prepend(w_parent);
                stacked_children.prepend(w_child);
            }
        }
        w_child = w_parent;
    }

    QListIterator<QWidget*> itp(stacked_parents);
    QListIterator<QWidget*> itc(stacked_children);
    while (itp.hasNext()) {
        ((QStackedWidget*)itp.next())->setCurrentWidget(itc.next());
    }
}

void QewTreeViewDialog::changeCurrentItemText(const QString &text)
{
    bool exists = false;
    QTreeWidgetItem* parent = 0;
    if(tree_view) {
        if(tree_view->currentItem()) {
            parent = tree_view->currentItem()->parent();
            exists = childExists(text, parent) > -1;
            if(exists) {
                currentItemTextNotChanged(text);
                return;
            }

            tree_view->currentItem()->setText(0, text);
            if(parent)
                parent->sortChildren(0, Qt::AscendingOrder);

            currentItemTextChanged(text);
        }
    }
}

void QewTreeViewDialog::changeItemText(QTreeWidgetItem* item, const QString &text)
{
    bool exists = false;
    QTreeWidgetItem* parent = 0;
    if(tree_view && item) {
        parent = item->parent();
        exists = childExists(text, parent, item) > -1;
        if(exists) {
            currentItemTextNotChanged(text);
            return;
        }

        item->setText(0, text);
        if(parent)
            parent->sortChildren(0, Qt::AscendingOrder);

        currentItemTextChanged(text);
    }
}

/*!
    \brief Inserts qew-children captions in tree navigator for user selection.
    
    This is a recursive function. This class has a dictionary that relates QTreeWidget items with qew-descendants, or their container widgets if these are from QStackedWidget class. Dictionary keeps all qew-descendants not only direct qew-children. Relation is used by setCurrentWidget(). Dictionary is passed by reference, so it is the same dictionary through all recursive calls. Process can only be triggered from a dialog with valid tree navigator, normally a top level dialog. If procces arrive to a qew-dialog from other hierarchy branch this qew-dialog is inserted, but recursion stops for this qew-descendants tree branch, the others classes have to know how to change between their own widgets.

    @param extensible_child A pointer to qew-child to be inserted in.
    @param item A pointer to QTreeWidget item to be related with \a extensible_child, it is dictionary key.
    @param descendants_dict The pointer dictionary. It uses pointers to QTreeWidget items of type void* as keys and it operates on pointer to qew-descendants, or their container widgets if these are from QStackedWidget class, of type QWidget*.
 */
void QewTreeViewDialog::insertChildrenInTree(QewExtensibleDialog* extensible_child, QTreeWidgetItem* item, QHash<void*, QWidget*> & descendants_dict)
{
    if(!extensible_child || !item) {
        errMsg(QewEmptyChild);
        return;
    }
    if(extensible_child->inherits("QewTreeViewDialog")/* && !extensible_child->getSameDialogForAllChildren()*/) {
        QWidget* child_cont_w = extensible_child->containerWidget();
        if(child_cont_w) {
            QString txt = item->text(0);
            item->setExpanded(expandedAtInit);
            descendants_dict.insert(item, child_cont_w);
            QList<QWidget*> cont_pages = extensible_child->getContainerPages();
            QListIterator<QWidget*> it(cont_pages);
            while (it.hasNext()) {
                QWidget* qew = it.next();
                if(!qew->inherits("QewTreeViewDialog")) {
                    QTreeWidgetItem* w_item = new QTreeWidgetItem(item);
                    QString s_caption = qew->windowTitle().isEmpty() ? QString(qew->objectName()) : qew->windowTitle();
                    w_item->setText(0,s_caption);                    
                    item->addChild(w_item);
                    w_item->setExpanded(expandedAtInit);
                    descendants_dict.insert(w_item, qew);
                }
                else {
                    QList<QewExtensibleDialog*> ch_list = extensible_child->extensibleChildren();
                    QListIterator<QewExtensibleDialog*> it_ch(ch_list);
                    QTreeWidgetItem* child_item = 0;

                    while (it_ch.hasNext()) {
                        QewExtensibleDialog* qew = it_ch.next();
                        child_item = new QTreeWidgetItem(item);
                        tree_view->addTopLevelItem(child_item);
                        child_item->setText(0, qew->windowTitle());
                        child_item->setExpanded(expandedAtInit);
                        insertChildrenInTree(qew, child_item, descendants_dict);
                    }
                }
            }
        }
        else {
            QString txt = item->text(0);
            item->setExpanded(expandedAtInit);
            descendants_dict.insert(item, extensible_child);
        }

    }
    else {
        QString txt = item->text(0);
        item->setExpanded(expandedAtInit);
        descendants_dict.insert(item, extensible_child);
    }
    extensible_child->setItemTree(item);
}

void QewTreeViewDialog::removeChildrenFromTree(QewExtensibleDialog* extensible_child, QHash<void *, QWidget *> &descendants_dict)
{
    QTreeWidgetItem * item = extensible_child->getItemTree();
    if(extensible_child->inherits("QewTreeViewDialog")) {
        QWidget* child_cont_w = extensible_child->containerWidget();
        if(child_cont_w) {
            descendants_dict.remove(item);
            QList<QWidget*> cont_pages = extensible_child->getContainerPages();
            QListIterator<QWidget*> it(cont_pages);
            while (it.hasNext()) {
                QWidget* qew = it.next();
                if(!qew->inherits("QewTreeViewDialog")) {
                    void* w_item = descendants_dict.key(qew);
                    descendants_dict.remove(w_item);
                }
                else {
                    QList<QewExtensibleDialog*> ch_list = extensible_child->extensibleChildren();
                    QListIterator<QewExtensibleDialog*> it_ch(ch_list);

                    while (it_ch.hasNext()) {
                        QewExtensibleDialog* qew = it_ch.next();
                        removeChildrenFromTree(qew, descendants_dict);
                    }
                }
            }
        }
        else {
            descendants_dict.remove(item);
        }

    }
    else {
        descendants_dict.remove(item);
    }
    if(item->parent())
        item->parent()->removeChild(item);
}

/*!
    \fn QewTreeViewDialog::designedCaptions(void)
    \brief Captions for tree navigator items corresponding to qew-dialog initial pages.
    
    Qt Designer doesn't allow you to set caption property for QStackedWidget pages, it has its reasons. But QewTreeViewDialog uses them as text for tree navigator items, so you have to code them. Your subclasses must implement this function.
    
    @return Qew-dialog initial pages captions.
    \sa setDesignedCaptions().
*/

/*!
    \brief Reads qew-dialog initial pages captions and inserts them in tree navigator.
    
    Reads a string list returned by designedCaptions() with captions for tree navigator items corresponding to qew-dialog initial pages and set them.
*/

void QewTreeViewDialog::setDesignedCaptions(void)
{
    QList<QWidget*> cont_pages = getContainerPages();
    designed_captions = designedCaptions();
    QStringList::Iterator it_s = designed_captions.begin();

    QListIterator<QWidget*> it(cont_pages);
    while (it.hasNext()) {
        if(it_s != designed_captions.end()) {
            it.next()->setWindowTitle(*it_s);
            ++it_s;
        }
        else
            break;
    }
}

int QewTreeViewDialog::childExists(const QString &text, QTreeWidgetItem* parent, QTreeWidgetItem* item)
{
    int index = -1;
    QewTreeViewDialog* extensible_parent = 0;
    QTreeWidget* treeWidget = getSuperTreeView(&extensible_parent);
    if(!treeWidget)
        return index;

    if(!parent)
        parent = treeWidget->currentItem();
    if(parent) {
       int cnt = parent->childCount();
       for(int i = 0; i < cnt; i++) {
           if(parent->child(i) == treeWidget->currentItem())
               continue;
           if(parent->child(i) == item)
               continue;
           if(parent->child(i)->text(0) == text) {
               index = i;
               break;
           }
       }
    }
    return index;
}

void QewTreeViewDialog::removeItemFromTree(QTreeWidgetItem *item, bool alsoWidgets)
{
    int cnt = item->childCount();
    for(int i = 0; i < cnt; i++) {
        QTreeWidgetItem *childItem = item->child(0);
        removeItemFromTree(childItem, alsoWidgets);
        item->removeChild(childItem);
        if(alsoWidgets)
            descendantWidgets.remove(childItem);
    }
    if(item->parent())
        item->parent()->removeChild(item);
    if(alsoWidgets)
        descendantWidgets.remove(item);
}

QTreeWidget *QewTreeViewDialog::getSuperTreeView(QewTreeViewDialog **extensible_parent)
{
    QTreeWidget* superTreeView = 0;
    QWidget* parentW = parentWidget();

    if(tree_view) {
        superTreeView = tree_view;
        *extensible_parent = this;
    }
    else {
        while(!superTreeView) {
            if(parentW->inherits("QewTreeViewDialog")) {
                *extensible_parent = (QewTreeViewDialog*)parentW;
                superTreeView = (*extensible_parent)->getTreeView();
            }
            parentW = parentW->parentWidget();
        }
    }
    return superTreeView;
}

QWidget *QewTreeViewDialog::getCurrentWidget()
{
    QWidget* currentWidget = 0;
//    if(containerWidgetStack)
//        currentWidget = containerWidgetStack->currentWidget();
    QewTreeViewDialog* extensibleParent = 0;
    QTreeWidget* treeView = getSuperTreeView(&extensibleParent);
    if(treeView) {
        currentItem = treeView->currentItem();
        if(currentItem) {
            currentWidget = extensibleParent->descendantWidgets[currentItem];
        }
    }
    return currentWidget;
}

void QewTreeViewDialog::setExpandedAtInit(bool expandedAtInit)
{
    this->expandedAtInit = expandedAtInit;
}


/*!
    \class QewTreeViewDialogEmpty qewtreeviewdialog.h
    \brief This class doesn't add functionality to its base class, but it is a concrete class.
    
    This class is intended for internal purposes only, it is instantiate from QewTreeViewDialog::newPage().
    
    \author Jose M. Cuadra
*/

/*!
    \fn QewTreeViewDialogEmpty::QewTreeViewDialogEmpty(QWidget*, const char*)
    \brief The constructor.

    Calls base class constructor QewTreeViewDialog::QewTreeViewDialog().
*/
