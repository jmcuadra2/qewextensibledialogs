//
// C++ Implementation: qewtoolboxdialog
//
// Description: 
//
//
// Author: José Manuel Cuadra Troncoso <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

//#include <QtGui/qtoolbox.h>
//#include <QtGui/qlayout.h>
#include "qewtoolboxdialog.h"
#include <QVBoxLayout>

/*!
    \class QewToolBoxDialog qewtoolboxdialog.h
    \brief This is the base class for the QewToolBoxDialog branch.
    
    This class show its qew-children in individual pages within nested vertically tabbed widgets (QToolBox). It is the less developed class in the library. For more information see \ref ui_templat "qew-templates".
    \author Jose M. Cuadra
*/


/*!
    \brief The constructor.

    Calls base class constructor QewExtensibleDialog::QewExtensibleDialog().
*/

QewToolBoxDialog::QewToolBoxDialog(QWidget *w_parent, const char *name)
    : QewExtensibleDialog(w_parent, name)
{
    containerToolBox = nullptr;
    setSetUpOk(false);
}

/*!
    \brief The destructor.

    Calls base class destructor QewExtensibleDialog::~QewExtensibleDialog().
*/

QewToolBoxDialog::~QewToolBoxDialog()
{
}

/*!
    \brief Sets the qew-dialog for use.
    
    Read property an sets the corresponding object: a container widget, from QToolBox class, and tries to make a valid qew-dialog.
    
    @return True if the qew-dialog could be set as a valid qew-dialog.
*/

bool QewToolBoxDialog::setUpDialog(void)
{
    bool ret = false;
    ret = setContainerWidget();
    if(ret) {
        setButtonsWidget();
        setSetUpOk(true);
        removeLayoutMargin();
        containerToolBox->setCurrentIndex(0);
    }
    return ret;
}

/*!
    \brief Sets the container widget.

    This function looks for a QToolBox with name set by QewExtensibleDialog::containerName property and sets it. It can send error message to users.
    
    @return True if container widget was found and it inherits QToolBox, false otherwise.
*/

bool QewToolBoxDialog::setContainerWidget(void)
{
    bool ret = findWidget(containerName(), "");
    if(!ret)
        errMsg(QewContainerMissing);
    else {
        if((ret = findWidget(containerName(), "QToolBox"))) {
            //       containerToolBox = (QToolBox*) findChild(containerName(), "QToolBox");
            QString name = QString(containerName());
            containerToolBox = findChild<QToolBox*>(name);
            setBaseContainerWidget(containerToolBox);
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

uint QewToolBoxDialog::pagesNumber(void)
{
    uint cnt = 0;
    if(containerToolBox)
        cnt = uint(containerToolBox->count());
    return cnt;
}

/*!
    \brief Gets container widget pages.
    
    Gets container widget tabs corresponding to dialog pages.

    @return A list of container pages.
*/

QList<QWidget*> QewToolBoxDialog::getContainerPages(void)
{
    QList<QWidget*> cont_pages;
    if(containerToolBox) {
        for(int i = 0; i < int(pagesNumber()); i++)
            cont_pages.append(containerToolBox->widget(i));
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

QWidget* QewToolBoxDialog::addPage(QewExtensibleDialog* extensible_child,
                  const QString& caption, QTreeWidgetItem *, int index)
{
    QWidget* page = nullptr;
    if(containerToolBox) {
        page = extensible_child;
        if(index > -1)
            containerToolBox->insertItem(index, page, caption);
        else
            containerToolBox->addItem(page, caption);
    }
    else
        errMsg(QewContainerMissing);
    return page;
}

/*!
    \brief Adds a list of pages.
    
    Adds a qew-children list, as tabs, to container widget. It creates a new child QewTToolBox, setting its caption, and it inserts qew-children from list as children of this new child. Qew-children from list, as QWidgets, are inserted as tabs in new child's container widget of QToolBox class. It is intended to be invoked from addExtensibleChildrenList().
    
    @param extensible_children_list The qew-children list to add.
    @param w_page The empty QewToolBoxDialog created with newPage(), child of \b this, that is go to be the real parent of each qew_dialog from \a extensible_children_list.
    @param caption A text for tab asociated to \a extensible_children_list.
    @return A pointer to actual QToolBox where qew_dialogs from \a extensible_children_list was added or 0 if list could not be added.
*/

QWidget* QewToolBoxDialog::addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                 QewExtensibleDialog* w_page, const QString& caption,
                 QTreeWidgetItem *, int index)
{
    QToolBox* page = nullptr;
    if(containerToolBox) {
        if(w_page->containerWidget()) {
            if(w_page->containerWidget()->inherits("QToolBox")) {
                page = dynamic_cast<QToolBox*>(w_page->containerWidget());
                if(index > -1)
                    containerToolBox->insertItem(index, w_page, caption);
                else
                    containerToolBox->addItem(w_page, caption);

                QListIterator<QewExtensibleDialog*> it(extensible_children_list);
                while (it.hasNext()) {
                    QewExtensibleDialog* qew = it.next();
                    qew->setModal(false);
                    page->addItem(qew, qew->windowTitle());
                }
            }
        }
    }
    return dynamic_cast<QWidget*>(page);
}

/*!
    \brief Creates an empty page where you can add qew_dialogs lists.
    
    Creates a qew-dialog of QewToolBoxDialogEmpty class, with only an empty QToolBox as container widget. It is intended to be invoked from addExtensibleChildrenList().
    
    @param caption A caption for the new qew-dialog.
    @return A pointer to created qew-dialog or 0 if it could not be set up.
*/

QewExtensibleDialog* QewToolBoxDialog::newPage(const QString& caption, const QByteArray &cont_name)
{ 
    QewExtensibleDialog* w_page = nullptr;
    if(containerToolBox) {
        QewToolBoxDialog* new_page = new QewToolBoxDialog(this);
        //     QToolBox* page = new QToolBox(w_page, "QewContainerToolBox");
        QToolBox* page = new QToolBox();
        page->setObjectName("QewContainerToolBox");
        if(cont_name.trimmed().isEmpty()) {
            page->setObjectName("QewContainerToolBox");
            new_page->setContainerName("QewContainerToolBox");
        }
        else {
            page->setObjectName(cont_name);
            new_page->setContainerName(cont_name);
        }
        if(new_page->setUpDialog()) {
            w_page = new_page;
            w_page->setWindowTitle(caption);
            new QVBoxLayout(w_page);
            /*      new QVBoxLayout(w_page, 0, -1);       */
        }
        else
            delete new_page;
    }
    else
        errMsg(QewContainerMissing);

    return w_page;
}

void QewToolBoxDialog::removePage(QewExtensibleDialog *extensible_child, QTreeWidgetItem */*parentItem*/)
{
    if(containerToolBox) {
        int index = containerToolBox->indexOf(extensible_child);
        containerToolBox->removeItem(index);
        delete extensible_child;
    }
}


/*!
    \class QewToolBoxDialogEmpty qewtoolboxdialog.h
    \brief This class doesn't add functionality to its base class, but it is a concrete class.
    
    This class is intended for internal purposes only, it is instantiate from QewToolBoxDialog::newPage().
    
    \author Jose M. Cuadra
*/

/*!
    \fn QewToolBoxDialogEmpty::QewToolBoxDialogEmpty(QWidget*, const char*)
    \brief The constructor.

    Calls base class constructor QewToolBoxDialog::QewToolBoxDialog().
*/

