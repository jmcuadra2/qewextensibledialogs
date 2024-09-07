//
// C++ Implementation: qewtabdialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

//#include <QtGui/qtabwidget.h>
//#include <QtGui/qlayout.h>

#include "qewtabdialog.h"

/*!
    \class QewTabDialog qewtabdialog.h
    \brief This is the base class for the QewTabDialog branch.
    
    This class show its qew-children in individual pages within nested horizontally tabbed widgets (QTabWidget). For more information see \ref ui_templat "qew-templates".
    
    \author Jose M. Cuadra
*/


/*!
    \brief The constructor.

    Calls base class constructor QewExtensibleDialog::QewExtensibleDialog().
*/

QewTabDialog::QewTabDialog(QWidget *w_parent, const char *name)
    : QewExtensibleDialog(w_parent, name)
{
    containerTabWidget = 0;
    setSetUpOk(false);
}

/*!
    \brief The destructor.

    Calls base class destructor QewExtensibleDialog::~QewExtensibleDialog().
*/

QewTabDialog::~QewTabDialog()
{
}

/*!
    \brief Sets the qew-dialog for use.
    
    Reads property an sets the corresponding object: a container widget, from QTabWidget class, and tries to make a valid qew-dialog.
    
    @return True if the qew-dialog could be set as a valid qew-dialog.
*/

bool QewTabDialog::setUpDialog(void)
{
    bool ret = false;
    ret = setContainerWidget();
    if(ret) {
        setButtonsWidget();
        setSetUpOk(true);
        removeLayoutMargin();
        containerTabWidget->setCurrentIndex(0);
    }
    return ret;
}

/*!
    \brief Sets the container widget.

    This function looks for a QTabWidget with name set by QewExtensibleDialog::containerName property and sets it. It can send error message to users.
    
    @return True if container widget was found and it inherits QTabWidget, false otherwise.
*/ 

bool QewTabDialog::setContainerWidget(void)
{
    bool ret = findWidget(containerName(), "");
    if(!ret)
        errMsg(QewContainerMissing);
    else {
        if((ret = findWidget(containerName(), "QTabWidget"))) {
            //       containerTabWidget = (QTabWidget*) child(containerName(), "QTabWidget");
            const QString name = QString(containerName());
            containerTabWidget = findChild<QTabWidget*>(name);
            setBaseContainerWidget(containerTabWidget);
        }
        else
            errMsg(QewWrongContainer);
    }
    return ret;
}

/*!
    \brief Number of top-level pages.
    
    Returns the number of top-level pages, i.e., the number of tabbed widgets from container widget.
    
    @return The number of top-level pages.
*/ 

uint QewTabDialog::pagesNumber(void)
{
    uint cnt = 0;
    if(containerTabWidget)
        cnt = containerTabWidget->count();
    return cnt;
}

/*!
    \brief Gets container widget pages.
    
    Gets container widget tabs corresponding to dialog pages.

    @return A list of container pages.
*/

QList<QWidget*> QewTabDialog::getContainerPages(void)
{
    QList<QWidget*> cont_pages;
    if(containerTabWidget) {
        for( uint i = 0; i < pagesNumber(); i++)
            cont_pages.append(containerTabWidget->widget(i));
    }
    return cont_pages;
}

/*!
    \brief Add an individual page.
    
    Adds a qew-child, as a tab, to container widget. It is intended to be invoked from addExtensibleChild().
    
    @param extensible_child A pointer to qew-child to add.
    @param caption A caption for \a extensible_child.
    @return A pointer to \a extensible_child or 0 if it could not be added.
*/

QWidget* QewTabDialog::addPage(QewExtensibleDialog* extensible_child, const QString& caption,
                               QTreeWidgetItem *, int index)
{
    QWidget* page = 0;
    if(containerTabWidget) {
        page = extensible_child;
        if(index > -1)
            containerTabWidget->insertTab(index, page, caption);
        else
            containerTabWidget->addTab(page, caption);
    }
    else
        errMsg(QewContainerMissing);
    return page;
}

/*!
    \brief Adds a list of pages.
    
    Adds a qew-children list, as tabs, to container widget. It creates a new child QewTabDialog, setting its caption, and it inserts qew-children from list as children of this new child. Qew-children from list, as QWidgets, are inserted as tabs in new child's container widget of QTabWidget class. It is intended to be invoked from addExtensibleChildrenList().
    
    @param extensible_children_list The qew-children list to add.
    @param w_page The empty QewTabDialog created with newPage(), child of \b this, that is go to be the real parent of each qew_dialog from \a extensible_children_list.
    @param caption A text for tab asociated to \a extensible_children_list.
    @return A pointer to actual QTabWidget where qew_dialogs from \a extensible_children_list was added or 0 if list could not be added.
*/

QWidget* QewTabDialog::addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                     QewExtensibleDialog* w_page, const QString& caption,
                     QTreeWidgetItem * , int index)
{
    QTabWidget* page = 0;
    if(containerTabWidget) {
        if(w_page->containerWidget()) {
            if(w_page->containerWidget()->inherits("QTabWidget")) {
                page = (QTabWidget*)w_page->containerWidget();
                if(index > -1)
                    containerTabWidget->insertTab(index, page, caption);
                else
                    containerTabWidget->addTab(page, caption);

                QListIterator<QewExtensibleDialog*> it(extensible_children_list);
                while (it.hasNext()) {
                    QewExtensibleDialog* qew = it.next();
                    qew->setModal(false);
                    page->addTab(qew, qew->windowTitle());
                }
            }
        }
    }
    return (QWidget*)page;
}

/*!
    \brief Creates an empty page where you can add qew_dialogs lists.
    
    Creates a qew-dialog of QewTabDialogEmpty class, with only an empty QTabWidget as container widget. It is intended to be invoked from addExtensibleChildrenList().
    
    @param caption A caption for the new qew-dialog.
    @return A pointer to created qew-dialog or 0 if it could not be set up.
*/

QewExtensibleDialog* QewTabDialog::newPage(const QString& caption, const QByteArray &cont_name)
{
    QewExtensibleDialog* w_page = 0;
    if(containerTabWidget) {
        QewTabDialog* new_page = new QewTabDialog(this);
        /*    QTabWidget* page = new QTabWidget(new_page, "QewContainerTabWidget"); */
        QTabWidget* tab = new QTabWidget(new_page);        
        if(cont_name.trimmed().isEmpty()) {
            tab->setObjectName("QewContainerTabWidget");
            new_page->setContainerName("QewContainerTabWidget");
        }
        else {
            tab->setObjectName(cont_name);
            new_page->setContainerName(cont_name);
        }
        if(new_page->setUpDialog()) {
            w_page = new_page;
            w_page->setWindowTitle(caption);
        }
        else
            delete new_page;
    }
    else
        errMsg(QewContainerMissing);
    return w_page;
}

void QewTabDialog::removePage(QewExtensibleDialog *extensible_child, QTreeWidgetItem *)
{
    if(containerTabWidget) {
        containerTabWidget->removeTab(containerTabWidget->indexOf(extensible_child));
        delete extensible_child;
    }
}

/*!
    \class QewTabDialogEmpty qewtabdialog.h
    \brief This class doesn't add functionality to its base class, but it is a concrete class.
    
    This class is intended for internal purposes only, it is instantiate from QewTabDialog::newPage().
    
    \author Jose M. Cuadra
*/

/*!
    \fn QewTabDialogEmpty::QewTabDialogEmpty(QWidget*, const char*)
    \brief The constructor.

    Calls base class constructor QewTabDialog::QewTabDialog().
*/
