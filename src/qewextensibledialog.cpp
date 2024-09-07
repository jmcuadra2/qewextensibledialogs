
//
// C++ Implementation: qewextensibledialog
//
// Description:
//
//
// Author: Jose M. Cuadra <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "qewextensibledialog.h"
#include "connector.h"

#include <QLayout>
#include <QMessageBox>
#include <QByteArray>
#include <QCloseEvent>
#include <QPushButton>

/*!
    \class QewExtensibleDialog qewextensibledialog.h
    \brief This is the base class for the hierarchy of extensible dialogs (qew-dialogs).
    
    This is an abstract class, it offers the public interface for the whole hierarchy of extensible dialogs (qew-dialogs), except for reading and writing those properties added by subclasses. 
    
    Let's explain the public interface. Supose you have designed your qew-dialog with Qt Designer using a \ref ui_templat "qew-template", you have set properties and implemented the corresponding pure virtual functions and your custom slots. If you have a hand-coded qew-dialogs you have to set properties in code properly. If properties are set the first call you have to do is to setUpDialog() function, it returns true if the qew-dialog is valid and ready to use. Then, if you want, you can add valid qew-dialogs with addExtensibleChild() or valid qew-dialogs list with addExtensibleChildrenList() to your qew-dialog, you can nest, theoretically, qew-dialogs with no limit. Now you can use your qew-dialog as a normal QDialog. This base class provides the main property for the hierarchy, QewExtensibleDialog::containerName, this is the name for dailog's container widget, if this widget exists. The container widget is the widget where children qew-dialogs are inserted. See \ref qt_impl section.
    
    \author Jose M. Cuadra
*/

/*!
    \enum QewExtensibleDialog::QewContainerType
    \brief Container widget class.
    
    Enumeration of container widget classes, values correspond to branches in qew-dialogs hierarchy. You use them when ask to the factory for some empty dialog to wrap buttonless qew-dialogs.
 */
 
/*!
    \var QewExtensibleDialog::QewContainerType QewExtensibleDialog::QewBase
    The qew-dialog has no container widget, it is a one page buttonless qew-dialog from raw QewExtensibleDialog branch.
*/   
/*!
    \var QewExtensibleDialog::QewContainerType QewExtensibleDialog::QewSimpleWidget
    Container widget is a QFrame, the dialog is a one page qew-dialog from QewSimpleDialog branch.
*/ 
/*!
    \var QewExtensibleDialog::QewContainerType QewExtensibleDialog::QewTabWidget
    Container widget is a QTabWidget, the dialog is a tabbed pages qew-dialog from QewTabDialog branch.
*/ 
/*!
    \var QewExtensibleDialog::QewContainerType QewExtensibleDialog::QewToolBox
    Container widget is a QTabWidget, the dialog is a tabbed columns qew-dialog from QewToolBoxDialog branch.
*/ 
/*!
    \var QewExtensibleDialog::QewContainerType QewExtensibleDialog::QewWidgetStack
        Container widget is a QWidgetStack, the dialog is a stacked pages qew-dialog.  It is a top level qew-dialog it has to have a navigator for the pages, in this implementation this is a QTreeWidget, it is is a non top level qew-dialog it doesn't need it. These qew-dialogs are from QewTreeViewDialog branch.
*/

/*!
    \property QewExtensibleDialog::containerName
    This property has the container widget's name. Set this property's value with setContainerName() and get this property's value with containerName(). Normally you set this property at designing time with Qt Designer.
*/

/*!
    \brief The constructor.
        
    The constructor takes the same parameters as QWidget basic constructor, that is the only  constructor writed by uic when generates source from ui files in Qt-3.
    
    @param w_parent The qew-dialog's parent widget, it should be 0, a qew-dialog, for non top level qew-dialogs, or any widget for top level qew-ones.
    @param name The qew-dialog's name.
    @return The qew-dialog.  
   
*/

QewExtensibleDialog::QewExtensibleDialog(QWidget *w_parent, const char *name) : QDialog(w_parent)
{
    setObjectName(name);
    container_widget = 0;
    container_name = "";
    setup_Ok = true;
    buttonsWidget = 0;
    acceptMeansClose = true;
    dataChanged = false;
    itemTree = 0;
    saveButtonShowsUpdate = false;
    sameDialogForAllChildren = false;

//    connect(this, SIGNAL(emitExternVal(const QString& , const QVariant& )),
//                  this, SLOT(getExternVal(const QString& , const QVariant& )));
}

/*!
    \brief The destructor.
        
    The destructor disconnects signal emitExternVal() and deletes all its children, even if they aren't valid qew-dialogs and finally destroys \b this, like normal QWidget destructor. 
*/

QewExtensibleDialog::~QewExtensibleDialog()
{
    disconnectDialog(this);
}


/*!
    \brief User accepts edition.
        
    Calls save(), clean() and QDialog::accept(). 
    \sa reject(). 
*/

void QewExtensibleDialog::accept(void)
{
  if(validate())
  {
    save();
    clean();
    if(acceptMeansClose)
        QDialog::accept();
  }
}

/*!
    \brief User rejects edition.
        
    Calls clean() and QDialog::reject(). 
    \sa accept(). 
*/
 
void QewExtensibleDialog::reject(void)
{    
    clean();
    if(acceptMeansClose)
        QDialog::reject();
}

/*!
    \brief Clean resources recursively.
        
    Disconnects the container widget, if exists, calls cleanChildren() and cleanThis(). 
*/

void QewExtensibleDialog::clean(void)
{
  if(container_widget)
    disconnect(container_widget, 0, 0, 0);
  cleanChildren();
  cleanThis();
  setDataChangedThis(false);
}

/*!
    \brief Clean resources used by qew-children.
        
    Calls clean() for all its qew-children. 
*/

void QewExtensibleDialog::cleanChildren(void)
{
  QListIterator<QewExtensibleDialog*> it(l_extensible_children);
   while (it.hasNext())
     it.next()->clean();
}

/*!
    \fn QewExtensibleDialog::cleanThis(void)
    \brief Clean resources used by \b this
    
    This function should be implemented by subclasses in order to clean resources used by this qew-dialog, if it is necessary. Default implementation does nothing.
 */

/*!
    \brief Updates edited values recursively.
        
    Calls saveChildren() and saveThis(). 
*/

void QewExtensibleDialog::save(void)
{
    saveChildren();
	saveThis();
    setDataChangedThis(false);
}

/*!
    \brief Updates edited values in all its qew-children.
        
    Calls save() for all its qew-children. 
*/

void QewExtensibleDialog::saveChildren(void)
{
  QListIterator<QewExtensibleDialog*> it(l_extensible_children);
  while (it.hasNext())
    it.next()->save();
}

/*!
    \fn QewExtensibleDialog::saveThis(void)
    \brief Updates edited values in \b this
    
    This function must be implemented by subclasses in order to update values edited in this qew-dialog. Implementations in this library do nothing, they are to allow instantiation of branchs base classes.
 */

/*!
    \brief Validates edited values recursively.
        
    Calls validateChildren() and validateThis().
      
    @return True if every children and \b this have valid values for edited fields, false otherwise, in this case an information message is sent to screen.
*/


bool QewExtensibleDialog::validate(void)
{
  if(!validateChildren())
    return false;

	if(!validateThis()) {
//		QMessageBox::information(0, windowTitle(),
//								 tr("Some parameters were invalid") + "\n" +
//								 tr("and has been restored to previous values.") + "\n" +
//                 tr("Returning to edition."),  tr("&Return")) ;
    QMessageBox::information(0, windowTitle(),
                             tr("Some parameters were invalid") + "\n" +
                                 tr("and has been restored to previous values.") + "\n" +
                                 tr("Returning to edition."),  QMessageBox::Ok) ;
		return false;
	}
  return true;
}

/*!
    \brief Validates edited values in all its qew-children.
        
    Calls validate() for all its qew-children. 
    
    @return True if every children have valid values for edited fields, false otherwise.    
*/

bool QewExtensibleDialog::validateChildren(void)
{
  QListIterator<QewExtensibleDialog*> it(l_extensible_children);
  while (it.hasNext()) {
    if(!it.next()->validate())
      return false;
  }
	
  return true;
}

/*!
    \fn QewExtensibleDialog::validateThis(void)
    \brief Validates edited values in \b this
    
    This function should be implemented by subclasses in order to validate values edited in this qew-dialog, if it is necessary. Default implementation does nothing and returns true.
    
    @return True if \b this has valid values for edited fields, false otherwise.    
 */


void QewExtensibleDialog::restoreThis()
{
    setDataChangedThis(false);
}
 
/*!
    \brief Sets the container widget.
        
    If you are developing a new branch, based in a different container widget class, you have to reimplement this function to read the QewExtensibleDialog::containerName property a looks if the widget really exits and if it belongs to the proper class. Basic implementation sends an error message to screen and returns false.
    
    @return Basic implementation returns always false.
*/ 
 
bool QewExtensibleDialog::setContainerWidget(void) 
{
  errMsg(QewNoContainer);
  return false ; 
}

/*!
    \brief Gets the container widget.
    
    @return A pointer to the container widget, or 0 if it doesn't exist.
*/ 

QWidget* QewExtensibleDialog::containerWidget(void)
{
  return container_widget;
}

/*!
    \brief Manages close events.
    
    Calls clean() and hides the qew-dialog like a normal QDialog.
*/

void QewExtensibleDialog::closeEvent(QCloseEvent* e)
{
  clean();
  e->accept();
}

/*!
    \brief A slot to receive signals from qew-children.
    
    This slot connected to signal emitExternVal() of every qew-children and viceversa. It is intended for passing messages between qew-dialogs being edited. It gets a string identificating the emited value and a variant with the value, if it finds identification it executes code with value. Default implementation does nothing.
    
    @param var_name A identification string for the received value.
    @param var_val A QVariant with the received value.    
*/

void QewExtensibleDialog::getExternVal(const QString& , const QVariant& )
{ }

/*!
    \brief Sets container widget name.
    
    Sets the QewExtensibleDialog::containerName property, normally you set this property designing the form, in other case you have to set this name after construction, before calling any other function for this object.
    
    @param cont_name Name for container widget.
    
*/

void QewExtensibleDialog::setContainerName(const QByteArray& cont_name)
{
  container_name = cont_name;
}

/*!
    \fn QewExtensibleDialog::containerName(void) const
    \brief Gets container's name property.
    
    Container widget name is necessary to make valid dialog, except for those no having container.
    
    @return The name of the container widget.    
*/

/*!
    \brief Sets the qew-dialog for use.
    
    Reads properties an sets the corresponding objects. Trying to make a valid qew-dialog. .
    
    @return Basic implementation returns always true.
    
*/

bool QewExtensibleDialog::setUpDialog(void)
{  
    return true;
}

/*!
    \brief erases children margins to save screen space.
    
    erases children margins leaving only the top level layout margin. This saves screen space that can be lost nesting windows and can give a more consistent look.
  
*/

void QewExtensibleDialog::removeLayoutMargin(void)
{
  if(QewParent()) {
    if(layout())
      layout()->setContentsMargins(0, 0, 0, 0);
  }
}

/*!
    \brief Gets container widget pages.
    
    Gets container widget children widgets corresponding to dialog pages, these pages show top level qew-children.
  
    @return A list of container pages.
*/

QList<QWidget*> QewExtensibleDialog::getContainerPages(void)
{
  QList<QWidget*> cont_pages;
  return cont_pages;
}

/*!
    \brief Adds a qew-child in a new page.
    
    Adds a qew-child to its children, if it is valid, and put it into a new extension of the container widget removing its layout margin. Subclasses have to define the new extension in addPage(). 
    Qew-child should have been constructed with \b this as parent
    
    @param extensible_child A pointer to valid qew-child to be added.
    @param caption A caption for \a extensible_child.
*/

void QewExtensibleDialog::addExtensibleChild(QewExtensibleDialog* extensible_child,
                             const QString& caption, QTreeWidgetItem *parentItem, int index)
{
  if(!isSetUpOk()){
    errMsg(QewNoSetUp);
    return;
  }  
  if(!extensible_child) {
    errMsg(QewEmptyChild);
    return;
  }
  if(!extensible_child->isSetUpOk()){
    extensible_child->errMsg(QewNoSetUp);
    return;
  }
  extensible_child->setModal(false);
  QString cap = caption.isEmpty() ? extensible_child->windowTitle() : caption;
  cap = cap.isEmpty() ? QString(extensible_child->objectName()) : cap;   
  QWidget* page = addPage(extensible_child, cap, parentItem, index);
  if(page) {
    extensibleChildren().append(extensible_child);
    connectDialog(extensible_child);
    if(page->layout()) {
      if(page != extensible_child)
        page->layout()->setContentsMargins(0, 0, 0, 0);
    }
//     adjustSize();
  }
  return;
}

/*!
    \brief Adds a qew-children list in a new top-level page within children pages.
    
    Adds a qew-children list to its children, only valid ones, and put them in individual pages into a new extension of the container widget removing their layout margins. Subclasses have to define the new extension in addPageList(). 
    
    @param extensible_children_list The list of valid qew-children to be added.
    @param caption A caption for \a extensible_child.
*/


void QewExtensibleDialog::addExtensibleChildrenList(QList<QewExtensibleDialog*>
                    extensible_children_list, const QString& caption, const bool oneAsList,
                   const QByteArray &cont_name, QTreeWidgetItem *parentItem, int index)
{
  if(!isSetUpOk()){
    errMsg(QewNoSetUp);
    return;
  }
  
  QMutableListIterator<QewExtensibleDialog*> it(extensible_children_list);
  while (it.hasNext()) {
    QewExtensibleDialog* qew = it.next();

    if(!qew->isSetUpOk()) {
        qew->errMsg(QewNoSetUp);
        it.remove();
    }
  }
//  if(extensible_children_list.isEmpty()) {
//      errMsg(QewEmptyChildrenList);
//      return;
//  }
  if(extensible_children_list.count() == 1 && !oneAsList)
      return addExtensibleChild(extensible_children_list.at(0), caption, parentItem, index);
  
  QString cap = caption.isEmpty() ? QString("Page") + " " + QString::number(pagesNumber() + 1) : caption;
  QewExtensibleDialog* w_page = newPage(cap, cont_name);
  if(w_page) {
      extensibleChildren().append(w_page);

      QWidget* page = addPageList(extensible_children_list, w_page, cap, parentItem, index);
      if(page) {
          it.toFront();
          while (it.hasNext()) {
            QewExtensibleDialog* qew = it.next();
            w_page->extensibleChildren().append(qew);
            connectDialog(qew);
          }
      }
  }    
 
  return;  
}

/*!
    \brief Number of top-level pages.
    
    Returns the number of top-level pages, i.e., the number of top-level qew-children, including empty children containing qew_dialogs lists.
    
    @return The number of top-level pages.
   
*/   

uint QewExtensibleDialog::pagesNumber(void)
{
  return 0;
}

/*!
    \brief Adds an individual page.
    
    Adds a qew-child, as a widget, to container widget. It is intended to be invoked from addExtensibleChild(). It shold be reimplemented in subclasses to do proper inserting depending on container widget type.  Default implementation does nothing.
    
    @param extensible_child A pointer to qew-child to add.
    @param caption A caption for \a extensible_child.
    @return A null pointer, but subclasses return \a extensible_child.  
*/

QWidget* QewExtensibleDialog::addPage(QewExtensibleDialog* , const QString& ,
                                      QTreeWidgetItem * , int)
{
  errMsg(QewNoContainer); 
  return (QWidget*)0; 
}

/*!
    \brief Adds a list of pages.
    
    Adds a qew-children list, as widgets, to container widget. It is intended to be invoked from addExtensibleChildrenList(). It shold be reimplemented in subclasses to do proper inserting depending on container widget type.  Default implementation does nothing.
    
    @param extensible_children_list The qew-children list to add.
    @param w_page The empty qew_dialog of proper type created with newPage(), child of \b this, that is go to be the real parent of each qew_dialog from \a extensible_children_list.
    @param caption A caption for window containing \a extensible_children_list.
    @return A null pointer, but subclasses return the actual widget where qew_dialogs from \a extensible_children_list was added.     
*/

QWidget* QewExtensibleDialog::addPageList(QList<QewExtensibleDialog*>,
              QewExtensibleDialog* , const QString&, QTreeWidgetItem * , int )
{
  errMsg(QewNoContainer);  
  return (QWidget*)0;
}

/*!
    \brief Creates an empty page where you can add qew_dialogs lists.
    
    Creates an empty dialog of the same class of \b this where a qew_dialogs from a list are added, it is intended to be invoked from addExtensibleChildrenList(). It shold be reimplemented in subclasses to creates a new qew-dialog of proper class. Default implementation does nothing.
    
    @param caption A caption for the new qew-dialog.
    @return A null pointer, but subclasses return an empty qew-dialog  of the same class of them.
   
*/

QewExtensibleDialog* QewExtensibleDialog::newPage(const QString& , const QByteArray&)
{
  errMsg(QewNoContainer);  
  return (QewExtensibleDialog*)0;
}

void QewExtensibleDialog::removePage(QewExtensibleDialog* extensible_child, QTreeWidgetItem *)
{
    QWidget* w = extensible_child->parentWidget();
    if(w && !w->inherits("QewExtensibleDialog"))
        delete w;
    else
        delete extensible_child;
}

/*!
    \brief Overrides QDialog::setExtension(QWidget* widget).
    
    This function overrides QDialog::setExtension() to do nothing, it is because standar version conflicts a little bit with this new dialog extension system.
    
    @param widget Not used.
    
*/    

void QewExtensibleDialog::setExtension(QWidget* )
{
  errMsg(QewNoSetExtension);
}

/*!
    \brief Looks for a widget.
    
    Looks for a widget by its name and its widget base class.
    
    @param w_name The widget name.
    @param clname The widget base class.
    @return True if the widget was found and it is of proper class.    
    
*/    

bool QewExtensibleDialog::findWidget(QByteArray w_name, const char* clname)
{
  QObject* o;
  bool ret = false;
  QString wName = w_name;
  o = findChild<QObject*>(wName);
  if(o) {
    if(QString(clname).isEmpty()) 
      ret = o->isWidgetType() ? true : false;
    else 
      ret = o->inherits(clname) ? true : false;
  }
  return ret;
};

/*!
    \enum QewExtensibleDialog::QewErrorMsg
    \brief Error messages codes
    
    Enumeration of error messages codes that qew-dialogs can send, codes start at 0. Subclasses can create their own extra codes, but without conflict.
 */
 
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewUndefinedError
    Undefined error.
*/ 
 
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewContainerMissing
    The qew-dialog has to contain a container widget but it is missing.
*/   
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewWrongContainer
    The qew-dialog has a container widget of improper type.
*/ 
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewNoContainer
    The qew-dialog cannot have a container widget, it is a single page without buttons.
*/ 
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewContainerFull
    You cannot add more than two pages to a QewSimpleDialog if it was created empty, or more than one page in other case.
*/ 
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewEmptyChild
    You have tried to add a null pointer.
*/
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewEmptyChildrenList
    You have tried to add an empty list.
*/
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewNoAddChildrenList
    QewSimpleDialog cannot add lists.
*/
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewNoSetExtension
    You cannot use QDialog::SetExtension().
*/
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewNoSetUp
    The qew_dialog wasn't set up properly or you forgot to do it.
*/
/*!
    \var QewExtensibleDialog::QewErrorMsg QewExtensibleDialog::QewNoTreeNavigator
    A top level QewTreeViewDialog needs a tree navigator for raising pages.
*/
    
/*!
    \brief Displays error messagges.
    
    Displays a window with one of error messages commented in QewErrorMsg.
    
    @param id The QewErrorMsg identification.
    
 */

void QewExtensibleDialog::errMsg(QewErrorMsg id)
{
  QString msg;
  switch(id) {
    case QewContainerMissing:
      msg =  tr("Widgets container is missing");
      break;
    case QewWrongContainer:
      msg =  tr("Widgets container isn't of a valid class");
      break;
    case QewNoContainer:
      msg =  tr("This dialog cannot have widgets container");
      break;
    case QewContainerFull:
      msg =  tr("Widgets container is full");
      break;
    case QewEmptyChild:
      msg =  tr("No child to add");
      break;
    case QewEmptyChildrenList:
      msg =  tr("No children to add");
      break;                  
    case QewNoAddChildrenList:
      msg =  tr("This dialog cannot to add children lists");
      break;            
    case QewNoSetExtension:
      msg =  tr("Can't employ setExtension() in a extensible dialog");
      break;
    case QewNoSetUp:
      msg =  tr("This dialog wasn't set up or is invalid");
      break; 
    case QewNoTreeNavigator:
      msg =  tr("This dialog hasn't tree navigator");
      break;                      
    default:
      msg = tr("Undefined error");
      break;
  }
//  QMessageBox* mb = new QMessageBox(tr("Extensible dialog error"), msg,
//        QMessageBox::Critical, QMessageBox::Ok, Qt::NoButton,
//        Qt::NoButton);
    QMessageBox* mb = new QMessageBox(QMessageBox::Critical, tr("Extensible dialog error"),msg, QMessageBox::Ok);
  mb->setWindowTitle(windowTitle());
//   mb->adjustSize();
  mb->exec();
  delete mb;

}

/*!
    \brief Makes connections to send and to receive values between qew-dialogs.
    
    Connects \b this signal emitExternVal() to a just added qew-child slot getExternVal() and viceversa.
    
    @param dlg_child The just added qew_child.
 */
 
//bool QewExtensibleDialog::connectChild(QewExtensibleDialog* dlg_child)
//{
//  bool ret = false;

//  ret = connect(this, SIGNAL(emitExternVal(const QString& , const QVariant& )), dlg_child, SLOT(getExternVal(const QString& , const QVariant& )));
//  ret = ret && connect(dlg_child, SIGNAL(emitExternVal(const QString& , const QVariant& )), this, SLOT(getExternVal(const QString& , const QVariant& )));
//  QListIterator<QewExtensibleDialog*> it(extensibleChildren());

//  while (it.hasNext()) {
//    QewExtensibleDialog* qew = it.next();
//    if(qew != dlg_child) {
//      ret = ret && connect(qew, SIGNAL(emitExternVal(const QString& , const QVariant& )), dlg_child, SLOT(getExternVal(const QString& , const QVariant& )));
//      ret = ret && connect(dlg_child, SIGNAL(emitExternVal(const QString& , const QVariant& )), qew, SLOT(getExternVal(const QString& , const QVariant& )));
//    }
//  }

//  return ret;
//}

void QewExtensibleDialog::connectDialog(QewExtensibleDialog* dlg_child)
{
    Connector* connector = Connector::instance();
    connector->addDialog(dlg_child);
}

void QewExtensibleDialog::disconnectDialog(QewExtensibleDialog *dlg_child)
{
    Connector* connector = Connector::instance();
    connector->removeDialog(dlg_child);
}

//bool QewExtensibleDialog::connectChild(QewExtensibleDialog *dlg_child,
//                                       QewExtensibleDialog * other)
//{
//    bool ret = false;

//    if(other != dlg_child) {
//        ret = connect(other, SIGNAL(emitExternVal(const QString& , const QVariant& )),
//                      dlg_child, SLOT(getExternVal(const QString& , const QVariant& )));
//        ret = ret && connect(dlg_child, SIGNAL(emitExternVal(const QString& , const QVariant& )),
//                             other, SLOT(getExternVal(const QString& , const QVariant& )));
//    }
//    QListIterator<QewExtensibleDialog*> it(other->extensibleChildren());

//    while (it.hasNext()) {
//      QewExtensibleDialog* qew = it.next();
//      ret = ret && connectChild(dlg_child, qew);
//    }
//    return ret;
//}

/*!
    \brief The qew-children pointers list.
    
    A qew-dialog keeps its top level qew-children in list of pointers.
    
    @return The qew-children pointers list   
*/    

QList<QewExtensibleDialog *> &QewExtensibleDialog::extensibleChildren(void)
{
  return l_extensible_children;
}

/*!
    \brief The qew-parent.
    
    A top level qew-dialog has no qew-parent, but could has another widget as parent. A child qew-dialog always should has a qew-dialog as parent.
    
    @return The qew-parent, if it exist, or 0.   
*/ 

QewExtensibleDialog* QewExtensibleDialog::QewParent(void)
{
	QewExtensibleDialog* parent_ext = 0;
	QObject* parent_obj = parent();
	if(parent_obj) {
        if(parent_obj->inherits("QewExtensibleDialog")) {
    //       QString n_calss = parent_obj->className();
          parent_ext = (QewExtensibleDialog*) parent_obj;
        }
    }
	return parent_ext;
}

QWidget *QewExtensibleDialog::getButtonsWidget() const
{
    return buttonsWidget;
}

bool QewExtensibleDialog::setButtonsWidget(void)
{
    bool ret;
    if((ret = findWidget(buttonsWidgetName(), "QWidget"))) {
        buttonsWidget = findChild<QWidget*>(buttonsWidgetName());
    }
    return ret;
}

void QewExtensibleDialog::enterEvent(QEvent */*event*/)
{
    emit emitActivated(windowTitle());
}

QewExtensibleDialog* QewExtensibleDialog::extensibleParent(QWidget *widget)
{
    QewExtensibleDialog* extensibleParent = 0;
    while (!extensibleParent && widget) {
        if(widget->inherits("QewExtensibleDialog"))
            extensibleParent = (QewExtensibleDialog*)widget;
        else
            widget = widget->parentWidget();
    }
    return extensibleParent;
}

QewExtensibleDialog *QewExtensibleDialog::superTopParent()
{
//   QewExtensibleDialog* dialog = this;
//   QewExtensibleDialog* topParent = QewExtensibleDialog::extensibleParent(dialog->parentWidget());
//   while(topParent->parentWidget()) {
//       dialog = topParent;
//       topParent = QewExtensibleDialog::extensibleParent(dialog);
//   }
//   return topParent;
    QewExtensibleDialog* topParent = this;
    QWidget* widget = this->parentWidget();
    while (widget) {
        if(widget->inherits("QewExtensibleDialog"))
            topParent = (QewExtensibleDialog*)widget;
        widget = widget->parentWidget();
    }
    return topParent;
}

bool QewExtensibleDialog::getSameDialogForAllChildren() const
{
    return sameDialogForAllChildren;
}

void QewExtensibleDialog::setSameDialogForAllChildren(bool newSameDialogForAllChildren)
{
    sameDialogForAllChildren = newSameDialogForAllChildren;
}

void QewExtensibleDialog::setSaveButtonShowsUpdate(bool showUpdate)
{
    saveButtonShowsUpdate = showUpdate;
}

QTreeWidgetItem *QewExtensibleDialog::getItemTree() const
{
    return itemTree;
}

void QewExtensibleDialog::setItemTree(QTreeWidgetItem *itemTree)
{
    this->itemTree = itemTree;
}

bool QewExtensibleDialog::getDataChangedThis() const
{
    return dataChanged;
}

void QewExtensibleDialog::setDataChangedThis(bool dataChanged)
{
    this->dataChanged = dataChanged;
}

bool QewExtensibleDialog::getAcceptMeansClose() const
{
    return acceptMeansClose;
}

void QewExtensibleDialog::setAcceptMeansClose(bool acceptMeansClose)
{
    this->acceptMeansClose = acceptMeansClose;
    for(int i = 0; l_extensible_children.size(); i++)
        l_extensible_children.at(i)->setAcceptMeansClose(acceptMeansClose);
}

bool QewExtensibleDialog::dialogActivated(bool activated, QewDialogInactivate from,
                                          bool noSaveChildren)
{
    bool dataSaved = true;
    if(activated || from == FromDeleting)
        return dataSaved;

    if(getDataChangedThis()) {
        QMessageBox msgBox;
        QString name = windowTitle();

        QPushButton saveButton;
        if(from == FromOutside || from == FromSave) {
            if(!saveButtonShowsUpdate) {
                saveButton.setText(tr("Save"));
                msgBox.setText(name + tr(" changed.\nDo you want to save?"));
            }
            else {
                saveButton.setText(tr("Update"));
                msgBox.setText(name + tr(" changed.\nDo you want to update?"));
            }
            msgBox.addButton((QAbstractButton*)(&saveButton), QMessageBox::AcceptRole);
        }

        QPushButton returnButton(tr("Return"));
        msgBox.addButton((QAbstractButton*)(&returnButton), QMessageBox::RejectRole);

        QPushButton restoreButton(tr("Restore"));
        if(from == FromOutside || from == FromRestore)
            msgBox.addButton((QAbstractButton*)(&restoreButton), QMessageBox::NoRole);

        if(from == FromOutside)
            msgBox.setDefaultButton(&returnButton);
        else if(from == FromSave)
            msgBox.setDefaultButton(&saveButton);
        else if(from == FromRestore) {
            msgBox.setDefaultButton(&restoreButton);
            msgBox.setText(name + tr(" changed.\nDo you want to restore?"));
        }

        msgBox.exec();
        if(msgBox.clickedButton() == (QAbstractButton*)(&saveButton)) {
            if(noSaveChildren) {
                if(validateThis()) {
                    saveThis();
                }
//                setDataChangedThis(false);
                dataChanged = false;
            }
            else {
                if(validate()) {
                    save();
                }
                setDataChangedThis(false);
            }
            emit emitDialogSaved();
        }
        else if(msgBox.clickedButton() == (QAbstractButton*)(&restoreButton))
            restoreThis();
        else
            dataSaved = false;
    }
    return dataSaved;
}

bool QewExtensibleDialog::getDataChanged() const
{
    bool ret = getDataChangedThis();
    if(ret)
        return ret;
    QListIterator<QewExtensibleDialog*> it(l_extensible_children);
    while (it.hasNext()) {
      if(it.next()->getDataChangedThis())
        return true;
    }
    return false;
}

void QewExtensibleDialog::setDataChanged(bool dataChanged)
{
    setDataChangedThis(dataChanged);
    QListIterator<QewExtensibleDialog*> it(l_extensible_children);
    while (it.hasNext())
      it.next()->setDataChanged(dataChanged);
}

const QByteArray &QewExtensibleDialog::buttonsWidgetName() const
{
    return buttons_WidgetName;
}

void QewExtensibleDialog::setButtonsWidgetName(const QByteArray &value)
{
    buttons_WidgetName = value;
}

//void QewExtensibleDialog::removeExtensibleChild(QewExtensibleDialog *extensible_child, QTreeWidgetItem *parentItem)
//{
//   int index = extensibleChildren().indexOf(extensible_child);
//   if(index < 0)
//       return;
//   QewExtensibleDialog *child = extensibleChildren().takeAt(index);
//   for(int i = 0; i < child->extensibleChildren().size(); i++) {
////       QewExtensibleDialog *sub_child = child->extensibleChildren().takeFirst();
//       QewExtensibleDialog *sub_child = child->extensibleChildren().at(i);
//       child->removeExtensibleChild(sub_child);
//   }
//   delete child;
//}

void QewExtensibleDialog::removeExtensibleChild(QewExtensibleDialog *extensible_child, QTreeWidgetItem *parentItem)
{
   int index = extensibleChildren().indexOf(extensible_child);
   if(index < 0)
       return;
   removePage(extensible_child, parentItem);
   extensibleChildren().removeAt(index);
}

/*!
    \fn QewExtensibleDialog::emitExternVal(const QString& var_name, const QVariant& var_val)
    
    Container widget name is necessary to make valid dialog, except for those no having container. It sends a string as a value  identification and the actual value as a variant type, that need to be cast to proper type. It is intended to be connected to getExternVal() slot.
 
    @param var_name A identification string for the emited value.
    @param var_val A QVariant with the emited value.    
*/

/*!
    \fn QewExtensibleDialog::setBaseContainerWidget(QWidget * w_container)
    \brief Sets a widget base class pointer to container widget.
    
    Subclasses use a private pointer to their proper type of container widget, but the whole hierarchy use a pointer to container widget of simple QWidget type. This function sets this last type of pointer that is the accesible one.
        
    \sa setContainerWidget(), containerWidget().
*/

/*!
    \fn QewExtensibleDialog::isSetUpOk(void)
    \brief Set up state.
    
    This function informs on setUpDialog() return value.
    
    @return True is \b this is a valid qew-dialog.
*/

/*!
    \fn QewExtensibleDialog::setSetUpOk(bool ok)
    \brief Set up state.
    
    If you are doing a non standar qew-dialog setup, like factory does, this function keeps the result of this setup for further use.
 
     @param ok Set to true if you have gotten a valid qew-dialog in your non standar setup, set to false otherwise.
*/

/*!
    \class QewExtensibleDialogEmpty qewextensibledialog.h
    \brief This class doesn't add functionality to its base class, but it is a concrete class.
    
    This class is intended for internal purposes only, it is instantiate from subclasses newPage().
    
    \author Jose M. Cuadra
*/

/*!
    \fn QewExtensibleDialogEmpty::QewExtensibleDialogEmpty(QWidget*, const char*)
    \brief The constructor.
        
    Calls base class constructor QewExtensibleDialog::QewExtensibleDialog().     
*/
