//
// C++ Implementation: qewdialogfactory
//
// Description: 
//
//
// Author: Jose M. Cuadra <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QPushButton>
#include <QLayout>
#include <QScrollBar>
//#include <Qt/qobject.h>
#include <QTabWidget>
#include <QToolBox>
#include <QStackedWidget>

#include "qewsimpledialogform.h"
#include "qewtabdialogform.h"
#include "qewtoolboxdialogform.h"
#include "qewtreeviewdialogform.h"
#include "qewdialogfactory.h"

/*!
    \class QewDialogFactory qewdialogfactory.h
    \brief This class serves qew-dialogs ready to use.
    
    Dialogs has only properties asociated widgets and "Ok" and "Cancel" buttons. They are intended to wrap buttonless qew-dialogs.
    
    \author Jose M. Cuadra
*/  

/*!
    \brief The constructor.

    Create a qew-dialogs factory.
*/

QewDialogFactory::QewDialogFactory()
{
}

/*!
    \brief The destructor.

    Destroys factory.
*/

QewDialogFactory::~QewDialogFactory()
{
}

/*!
    \brief Serves qew-dialogs ready to use.

    This dialog are constructed from \ref ui_templat "qew-templates". The dialog is returned as a pointer to base class.

    @param cont_type Container widget type from QewExtensibleDialog::QewContainerType.
    @param w_parent The qew-dialog's parent widget, it should be 0, a qew-dialog, for non top level qew-dialogs, or any widget for top level qew-ones.
    @param name A name for dialog.
    @return
*/

QewExtensibleDialog* QewDialogFactory::createDialog(QewExtensibleDialog::QewContainerType cont_type,
                             QWidget *w_parent, bool connect, bool withButtons,
                             const char *name, const QByteArray &cont_name)
{
    QewExtensibleDialog* dialog = 0;
    //   QVBoxLayout* wLayout = 0;

    if(cont_type == QewExtensibleDialog::QewBase) {
        QewSimpleDialog* base_dialog = new QewSimpleDialog();
        if(base_dialog) {
            if(!base_dialog->setUpDialog())
                delete base_dialog;
            else
                dialog = (QewExtensibleDialog*)base_dialog;
        }
    }
    else if(cont_type == QewExtensibleDialog::QewSimpleWidget) {
        QewSimpleDialogForm* simple_dialog = new QewSimpleDialogForm(w_parent, name);
        if(simple_dialog) {
            if(cont_name.trimmed().isEmpty())
                simple_dialog->setContainerName("QewContainerFrame");
            else {
                QObject* o = simple_dialog->findChild<QObject*>("QewContainerFrame");
                if(o) {
                    o->setObjectName(QString(cont_name));
                    simple_dialog->setContainerName(cont_name);
                }
                else
                    simple_dialog->setContainerName("QewContainerFrame");
            }
            if(simple_dialog->setUpDialog()) {
                dialog = (QewExtensibleDialog*)simple_dialog;
            }
            else
                delete simple_dialog;
        }
        else
            delete simple_dialog;
    }
    else if(cont_type == QewExtensibleDialog::QewTabWidget) {
        QewTabDialogForm* tab_dialog = new QewTabDialogForm(w_parent, name);
        if(tab_dialog) {
            if(cont_name.trimmed().isEmpty())
                tab_dialog->setContainerName("QewContainerTabWidget");
            else {
                QObject* o = tab_dialog->findChild<QObject*>("QewContainerTabWidget");
                if(o) {
                    o->setObjectName(QString(cont_name));
                    tab_dialog->setContainerName(cont_name);
                }
                else
                    tab_dialog->setContainerName("QewContainerTabWidget");
            }
            if(tab_dialog->setUpDialog()) {
                QTabWidget* baseTab = (QTabWidget*) tab_dialog->containerWidget();
                if(baseTab) {
                    QWidget* page_tab1 = baseTab->widget(0);
                    while(page_tab1) {
                        baseTab->removeTab(baseTab->indexOf(page_tab1));
                        delete page_tab1;
                        page_tab1 = baseTab->widget(0);
                    }
                }
                dialog = (QewExtensibleDialog*)tab_dialog;
            }
            else
                delete tab_dialog;
        }
        else
            delete tab_dialog;
    }
    else if(cont_type == QewExtensibleDialog::QewToolBox) {
        QewToolBoxDialogForm* tool_dialog = new QewToolBoxDialogForm(w_parent, name);
        if(tool_dialog) {
            if(cont_name.trimmed().isEmpty())
                tool_dialog->setContainerName("QewContainerToolBox");
            else {
                QObject* o = tool_dialog->findChild<QObject*>("QewContainerToolBox");
                if(o) {
                    o->setObjectName(QString(cont_name));
                    tool_dialog->setContainerName(cont_name);
                }
                else
                    tool_dialog->setContainerName("QewContainerToolBox");
            }
            if(tool_dialog->setUpDialog()) {
                QToolBox* baseTool = (QToolBox*) tool_dialog->containerWidget();
                if(baseTool) {
                    QWidget* item_tool1 = baseTool->widget(0);
                    while(item_tool1) {
                        baseTool->removeItem(baseTool->indexOf(item_tool1));
                        delete item_tool1;
                        item_tool1 = baseTool->widget(0);
                    }
                }
                dialog = (QewExtensibleDialog*)tool_dialog;
            }
            else
                delete tool_dialog;
        }
        else
            delete tool_dialog;

    }
    else if(cont_type == QewExtensibleDialog::QewWidgetStack) {
        QewTreeViewDialogForm* stack_dialog = new QewTreeViewDialogForm(w_parent, name);
        if(stack_dialog) {
            stack_dialog->setContainerName("QewContainerWidgetStack");
            if(cont_name.trimmed().isEmpty())
                stack_dialog->setContainerName("QewContainerWidgetStack");
            else {
                QObject* o = stack_dialog->findChild<QObject*>("QewContainerWidgetStack");
                if(o) {
                    o->setObjectName(QString(cont_name));
                    stack_dialog->setContainerName(cont_name);
                }
                else
                    stack_dialog->setContainerName("QewContainerWidgetStack");
            }
            stack_dialog->setContainerWidget();
            QStackedWidget* baseStack = (QStackedWidget*) stack_dialog->containerWidget();
            if(baseStack) {
                QWidget* widget1 = baseStack->widget(0);
                while(widget1) {
                    baseStack->removeWidget(widget1);
                    delete widget1;
                    widget1 = baseStack->widget(0);
                }
                stack_dialog->setTreeNavigatorName("QewListView");
                stack_dialog->setTreeNavigator();
                stack_dialog->setSetUpOk(true);
                dialog = (QewExtensibleDialog*)stack_dialog;
            }
            else
                delete stack_dialog;
        }
        else
            delete stack_dialog;
    }
    if(dialog && !withButtons) {
        dialog->setButtonsWidgetName("buttonsWidget");
        dialog->setButtonsWidget();
        if(dialog->getButtonsWidget())
            dialog->getButtonsWidget()->hide();
    }
    if(dialog && connect)
        dialog->connectDialog(dialog);
    return dialog;
}
