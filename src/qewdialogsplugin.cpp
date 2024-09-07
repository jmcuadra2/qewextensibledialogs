//
// C++ Implementation: qewdialogsplugin
//
// Description:
//
//
// Author: Jose M. Cuadra <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "qewsimpledialog.h"
#include "qewtabdialog.h"
#include "qewtoolboxdialog.h"
#include "qewtreeviewdialog.h"
#include "qewtreepage.h"
#include "qewdialogsplugin.h"

#include <QPixmap>
#include <QtPlugin>

/*!
    \class QewDialogsPlugin qewdialogsplugin.h
    \brief Qt Designer plugins.
    
    There are one plugin for each hierarchy branch. Qew-dialogs created with these plugins are totally empty you have to insert container widget, buttons, etc. Reason for this is that you cannot access widgets inside plugins in Qt Designer in a normal way.
    Read \ref ui_templat "qew-templates" and Qt documentation on Qt Designer base-class templates.
    
    If you create a new plugin you have to change functions of this class or can create a subclass. Read Qt documentation on QWidgetPlugin for members of this class.
    
    \author Jose M. Cuadra
*/

QewDialogsPlugin::QewDialogsPlugin(QObject *parent)
    : QObject(parent)
{
    widgets.append(new QewExtensibleDialogPlugin(this));
    widgets.append(new QewSimpleDialogPlugin(this));
    widgets.append(new QewTabDialogPlugin(this));
    //  widgets.append(new QewToolBoxDialogPlugin(this));
    widgets.append(new QewTreeViewDialogPlugin(this));
    widgets.append(new QewTreePagePlugin(this));
}

QList<QDesignerCustomWidgetInterface*> QewDialogsPlugin::customWidgets() const
{
    return widgets;
}

// Q_EXPORT_PLUGIN2(QewDialogsPlugin, QewDialogsPlugin)

////////////

QewExtensibleDialogPlugin::QewExtensibleDialogPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewExtensibleDialogPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewExtensibleDialogPlugin::isInitialized() const
{
    return initialized;
}

QWidget* QewExtensibleDialogPlugin::createWidget(QWidget *parent)
{
    return new QewExtensibleDialog(parent);
}

QString QewExtensibleDialogPlugin::name() const
{
    return "QewExtensibleDialog";
}

QString QewExtensibleDialogPlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewExtensibleDialogPlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewExtensibleDialogPlugin::toolTip() const
{
    return "QewExtensibleDialog";
}

QString QewExtensibleDialogPlugin::whatsThis() const
{
    return tr("This dialog can embebed other dialogs.\nIt can be inherited for subclassing.");
}

bool QewExtensibleDialogPlugin::isContainer() const
{
    return true;
}

QString QewExtensibleDialogPlugin::includeFile() const
{
    return "qewextensibledialog.h";
}

QString QewExtensibleDialogPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewExtensibleDialog\" name=\"QewExtensibleDialog\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>This dialog can embebed other dialogs.\nIt can be inherited for subclassing.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

/////////////////



QewSimpleDialogPlugin::QewSimpleDialogPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewSimpleDialogPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewSimpleDialogPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QewSimpleDialogPlugin::createWidget(QWidget *parent)
{
    return new QewSimpleDialog(parent);
}

QString QewSimpleDialogPlugin::name() const
{
    return "QewSimpleDialog";
}

QString QewSimpleDialogPlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewSimpleDialogPlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewSimpleDialogPlugin::toolTip() const
{
    return "QewSimpleDialog";
}

QString QewSimpleDialogPlugin::whatsThis() const
{
    return "This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.";
}

bool QewSimpleDialogPlugin::isContainer() const
{
    return true;
}

QString QewSimpleDialogPlugin::includeFile() const
{
    return "qewsimpledialog.h";
}

QString QewSimpleDialogPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewSimpleDialog\" name=\"QewSimpleDialog\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

/////////////////

QewTabDialogPlugin::QewTabDialogPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewTabDialogPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewTabDialogPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QewTabDialogPlugin::createWidget(QWidget *parent)
{
    return new QewTabDialog(parent);
}

QString QewTabDialogPlugin::name() const
{
    return "QewTabDialog";
}

QString QewTabDialogPlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewTabDialogPlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewTabDialogPlugin::toolTip() const
{
    return "QewTabDialog";
}

QString QewTabDialogPlugin::whatsThis() const
{
    return "This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.";
}

bool QewTabDialogPlugin::isContainer() const
{
    return true;
}

QString QewTabDialogPlugin::includeFile() const
{
    return "qewtabdialog.h";
}

QString QewTabDialogPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewTabDialog\" name=\"QewTabDialog\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

//////////////////

QewToolBoxDialogPlugin::QewToolBoxDialogPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewToolBoxDialogPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewToolBoxDialogPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QewToolBoxDialogPlugin::createWidget(QWidget *parent)
{
    return new QewToolBoxDialog(parent);
}

QString QewToolBoxDialogPlugin::name() const
{
    return "QewToolBoxDialog";
}

QString QewToolBoxDialogPlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewToolBoxDialogPlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewToolBoxDialogPlugin::toolTip() const
{
    return "QewToolBoxDialog";
}

QString QewToolBoxDialogPlugin::whatsThis() const
{
    return "This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.";
}

bool QewToolBoxDialogPlugin::isContainer() const
{
    return true;
}

QString QewToolBoxDialogPlugin::includeFile() const
{
    return "qewtoolboxdialog.h";
}

QString QewToolBoxDialogPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewToolBoxDialog\" name=\"QewToolBoxDialog\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

// ////////////////////

QewTreeViewDialogPlugin::QewTreeViewDialogPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewTreeViewDialogPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewTreeViewDialogPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QewTreeViewDialogPlugin::createWidget(QWidget *parent)
{
    return new QewTreeViewDialog(parent);
}

QString QewTreeViewDialogPlugin::name() const
{
    return "QewTreeViewDialog";
}

QString QewTreeViewDialogPlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewTreeViewDialogPlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewTreeViewDialogPlugin::toolTip() const
{
    return "QewTreeViewDialog";
}

QString QewTreeViewDialogPlugin::whatsThis() const
{
    return "This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.";
}

bool QewTreeViewDialogPlugin::isContainer() const
{
    return true;
}

QString QewTreeViewDialogPlugin::includeFile() const
{
    return "qewtreeviewdialog.h";
}

QString QewTreeViewDialogPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewTreeViewDialog\" name=\"QewTreeViewDialog\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>This dialog can embebed other dialogs in a frame.\nIt can be inherited for subclassing.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

/////////

QewTreePagePlugin::QewTreePagePlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QewTreePagePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;
    initialized = true;
}

bool QewTreePagePlugin::isInitialized() const
{
    return initialized;
}

QWidget *QewTreePagePlugin::createWidget(QWidget *parent)
{
    return new QewTreePage(parent);
}

QString QewTreePagePlugin::name() const
{
    return "QewTreePage";
}

QString QewTreePagePlugin::group() const
{
    return "Extensible dialogs";
}

QIcon QewTreePagePlugin::icon() const
{
    return QIcon(QPixmap("qewextensibledialog.xpm"));
}

QString QewTreePagePlugin::toolTip() const
{
    return "QewTreePage";
}

QString QewTreePagePlugin::whatsThis() const
{
    return "Widget for QewTreeViewDialog page";
}

bool QewTreePagePlugin::isContainer() const
{
    return true;
}

QString QewTreePagePlugin::includeFile() const
{
    return "qewtreepage.h";
}

QString QewTreePagePlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"QewTreePage\" name=\"QewTreePage\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>250</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Widget for QewTreeViewDialog page.</string>\n"
           " </property>\n"
           "</widget>\n"
           "</ui>\n";
}

//Q_EXPORT_PLUGIN2(qewextensibledialogs, QewDialogsPlugin)




