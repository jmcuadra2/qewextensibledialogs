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

//#include <QtGui/qlayout.h>
#include <QFrame>
//#include <Qt/qobject.h>

#include "qewsimpledialog.h"
#include <QVBoxLayout>

/*!
    \class QewSimpleDialog qewsimpledialog.h
    \brief This is the base class for the QewSimpleDialog branch.
    
    This class show its qew-children in a QFrame. If if was created empty you can add until two children, if not only one. For more information see \ref ui_templat "qew-templates".
    
    \author Jose M. Cuadra
*/


/*!
    \brief The constructor.

    Calls base class constructor QewExtensibleDialog::QewExtensibleDialog().
*/

QewSimpleDialog::QewSimpleDialog(QWidget *w_parent, const char *name)
    : QewExtensibleDialog(w_parent, name)
{
    containerFrame = 0;
    init_num_pages = 0;
    countInitPages = true;
    setSetUpOk(false);
}

/*!
    \brief The destructor.

    Calls base class destructor QewExtensibleDialog::~QewExtensibleDialog().
*/

QewSimpleDialog::~QewSimpleDialog()
{
}
/*!
    \brief Sets the qew-dialog for use.
    
    Reads property an sets the corresponding object: a container widget, from QFrame class, and tries to make a valid qew-dialog.
    
    @return True if the qew-dialog could be set as a valid qew-dialog.
*/

bool QewSimpleDialog::setUpDialog(void)
{
    bool ret = false;
    ret = setContainerWidget();
    if(ret) {
        setButtonsWidget();
        const QList<QObject *> o_list = containerFrame->children();
        if(!o_list.isEmpty()) {
            QListIterator<QObject *> it_o(o_list);
            while(it_o.hasNext()) {
                if(it_o.next()->isWidgetType()){
                    init_num_pages = 1;
                    break;
                }
            }
        }
        setSetUpOk(true);
        removeLayoutMargin();
    }
    return ret;
}

/*!
    \brief erases children margins to save screen space.
    
    erases children margins and container widget margin leaving only the top level layout margin.

*/

void QewSimpleDialog::removeLayoutMargin(void)
{
    if(QewParent()) {
        if(layout()) {
            if(container_widget) {
                if(container_widget->layout())
                    container_widget->layout()->setContentsMargins(0, 0, 0, 0);
                layout()->setContentsMargins(0, 0, 0, 0);
            }
        }
        else {
            if(container_widget->layout())
                container_widget->layout()->setContentsMargins(0, 0, 0, 0);
        }
    }
}

void QewSimpleDialog::setCountInitPages(bool countInitPages)
{
    this->countInitPages = countInitPages;
}

/*!
    \brief Sets the container widget.

    This function looks for a QFrame with name set by QewExtensibleDialog::containerName property and sets it. It can send error message to users.
    
    @return True if container widget was found and it inherits QFrame, false otherwise.
*/

bool QewSimpleDialog::setContainerWidget(void)
{
    bool ret = findWidget(containerName(), "");
    if(!ret)
        errMsg(QewContainerMissing);
    else {
        if((ret = findWidget(containerName(), "QFrame"))) {
            //       containerFrame = (QFrame*) child(containerName(), "QFrame");
            containerFrame = findChild<QFrame*>(containerName());
            setBaseContainerWidget(containerFrame);
        }
        else
            errMsg(QewWrongContainer);
    }
    return ret;
}

/*!
    \brief Number of qew-children.
    
    Gives the number of qew-children, or this number plus one if dialog was not created empty.
    
    @return The number of qew-children, or this number plus one if dialog was not created empty.
*/ 

uint QewSimpleDialog::pagesNumber(void)
{
    int ret = extensibleChildren().count() + countInitPages ? init_num_pages : 0;
    return ret;
}

/*!
    \brief Add an individual page.
    
    Adds a qew-child and inserts it in container widget. It is intended to be invoked from addExtensibleChild().
    
    @param extensible_child A pointer to qew-child to add.
    @param caption A caption for \a extensible_child.
    @return A pointer to \a extensible_child or 0 if it could not be added.
*/

QWidget* QewSimpleDialog::addPage(QewExtensibleDialog* extensible_child,
                                  const QString& caption, QTreeWidgetItem *, int )
{

    QVBoxLayout*  base_lay_out = 0;
    QWidget* page = 0;
    if(containerFrame) {
        if(pagesNumber() == 0) {
            page = extensible_child;
            page->setParent(containerFrame);
            page->move(QPoint(0,0));
            if(containerFrame->layout())
                base_lay_out = (QVBoxLayout*)(containerFrame->layout());
            else
                base_lay_out = new QVBoxLayout(containerFrame);
            base_lay_out->setContentsMargins(0, 0, 0, 0);
            if(!caption.isEmpty())
                setWindowTitle(caption);
            if(page->layout())
                layout()->setContentsMargins(0, 0, 0, 0);
        }
        else if(pagesNumber() == 1) {
            page = extensible_child;
            QFrame* phantom_line = new QFrame(containerFrame);
            phantom_line->setGeometry(QRect(10, containerFrame->height(), containerFrame->width() - 20, 1));

            //       phantom_line->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, phantom_line->sizePolicy().hasHeightForWidth() ) );
            phantom_line->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
            phantom_line->setFrameShape( QFrame::NoFrame );
            phantom_line->setFrameShadow( QFrame::Plain );
            phantom_line->setLineWidth( 0 );
            base_lay_out = (QVBoxLayout*)(containerFrame->layout());
            if(base_lay_out)
                base_lay_out->addWidget(phantom_line);
            containerFrame->resize(containerFrame->width(), containerFrame->height() + phantom_line->height());
            page->setParent(containerFrame);
            page->move(QPoint(0,containerFrame->height() + phantom_line->height()));
            /*      if(page->layout())
        layout()->setMargin(0); */
        }
        else
            errMsg(QewContainerFull);
    }
    else
        errMsg(QewContainerMissing);
    if(page) {
        if(base_lay_out) {
            base_lay_out->addWidget(page);
            base_lay_out->activate();
        }
    }
    return page;
}  

/*!
    \brief Does nothing.
    
    You can not add lists to a QewSimpleDialog.
*/

QWidget* QewSimpleDialog::addPageList(QList<QewExtensibleDialog*> , QewExtensibleDialog* ,
                                const QString& , QTreeWidgetItem */*parentItem*/, int )
{
    errMsg(QewNoAddChildrenList);
    return (QWidget*)0;
}  

/*!
    \brief Creates an empty page where you can add qew_dialogs lists.
    
    Creates a qew-dialog of QewSimpleDialogEmpty class, with only an empty QFrame as container widget. It is not used and exists for completition.
    
    @param caption A caption for the new qew-dialog.
    @return A pointer to created qew-dialog or 0 if it could not be set up.
*/ 

QewExtensibleDialog* QewSimpleDialog::newPage(const QString& caption, const QByteArray &cont_name)
{ 
    QewExtensibleDialog* w_page = 0;
    //   QFrame* page = 0;
    if(containerFrame) {
        QewSimpleDialog* new_page = new QewSimpleDialog(this);
        //     page = new QFrame(w_page, "QewContainerFrame");
        if(cont_name.trimmed().isEmpty())
            new_page->setContainerName("QewContainerFrame");
        else
            new_page->setContainerName(cont_name);
        if(new_page->setUpDialog()) {
            w_page = new_page;
            w_page->setWindowTitle(caption);
            /*      new QVBoxLayout(w_page, 0, -1); */
            new QVBoxLayout(w_page);
        }
        else
            delete new_page;
    }
    else
        errMsg(QewContainerMissing);
    return w_page;
}

void QewSimpleDialog::removePage(QewExtensibleDialog *extensible_child, QTreeWidgetItem *)
{
    QWidget* w = extensible_child->parentWidget();
    QFrame* parentFrame = (QFrame*) w;
    if(parentFrame == containerFrame)
        delete extensible_child;
    else
        delete parentFrame;
}

/*!
    \class QewSimpleDialogEmpty qewsimpledialog.h
    \brief This class doesn't add functionality to its base class, but it is a concrete class.
    
    This class is intended for internal purposes only, it is instantiate from QewDialogFactory::createDialog().
    
    \author Jose M. Cuadra
*/

/*!
    \fn QewSimpleDialogEmpty::QewSimpleDialogEmpty(QWidget*, const char*)
    \brief The constructor.

    Calls base class constructor QewSimpleDialog::QewSimpleDialog().
*/
