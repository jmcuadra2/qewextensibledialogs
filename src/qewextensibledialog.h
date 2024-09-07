//
// C++ Interface: qewextensibledialog
//
// Description:
//
//
// Author: Jose M. Cuadra <jmcuadra@dia.uned.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QewEXTENSIBLEDIALOG_H
#define QewEXTENSIBLEDIALOG_H

#include <QObject>
#include <QStringList>
#include <QDialog>
#include <QByteArray>

#include <QCloseEvent>
#include <QtUiPlugin/QDesignerExportWidget>

class QTreeWidgetItem;

class QDESIGNER_WIDGET_EXPORT QewExtensibleDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QByteArray containerName READ containerName WRITE setContainerName DESIGNABLE true SCRIPTABLE true STORED true)
    Q_PROPERTY(QByteArray buttonsWidgetName READ buttonsWidgetName WRITE setButtonsWidgetName DESIGNABLE true SCRIPTABLE true STORED true)
    Q_ENUMS(QewContainerType)
    Q_ENUMS(QewErrorMsg)

    friend class QewDialogFactory;
    friend class QewSimpleDialog;
    friend class QewTabDialog;
    friend class QewToolBoxDialog;
    friend class QewTreeViewDialog;
    friend class QewExtensibleDialogPlugin;

public:
    enum QewErrorMsg {QewUndefinedError, QewContainerMissing, QewWrongContainer, QewNoContainer, QewContainerFull, QewEmptyChild, QewEmptyChildrenList, QewNoAddChildrenList, QewNoSetExtension, QewNoSetUp, QewNoTreeNavigator};
    enum QewContainerType {QewBase, QewSimpleWidget, QewTabWidget, QewToolBox, QewWidgetStack};
    enum QewDialogInactivate {FromOutside, FromSave, FromRestore, FromDeleting};

public:
    QewExtensibleDialog(QWidget *w_parent = 0, const char *name = 0);
    
public:
    ~QewExtensibleDialog();

    virtual bool setUpDialog(void);
    virtual void addExtensibleChild(QewExtensibleDialog* extensible_child, const QString& caption,
                                    QTreeWidgetItem* parentItem = 0, int index = -1);
    virtual void addExtensibleChildrenList(QList<QewExtensibleDialog*> extensible_children_list,
                     const QString& caption, const bool oneAsList = true,
                     const QByteArray& cont_name = "", QTreeWidgetItem* parentItem = 0,
                     int index = -1);
    void setContainerName(const QByteArray& cont_name);
    const QByteArray& containerName(void) const { return container_name ; }
    void setExtension(QWidget* widget);
    const QByteArray& buttonsWidgetName() const;
    void setButtonsWidgetName(const QByteArray &value);

    virtual void removeExtensibleChild(QewExtensibleDialog* extensible_child, QTreeWidgetItem* parentItem = 0);
    bool getAcceptMeansClose() const;
    void setAcceptMeansClose(bool acceptMeansClose);
    virtual bool dialogActivated(bool activated, QewDialogInactivate from = FromOutside, bool noSaveChildren = false);

    bool getDataChanged() const;
    void setDataChanged(bool dataChanged);
    virtual bool getDataChangedThis() const;
    virtual void setDataChangedThis(bool dataChanged);

    QTreeWidgetItem *getItemTree() const;
    void setItemTree(QTreeWidgetItem *itemTree);

    void setSaveButtonShowsUpdate(bool showUpdate);        

public slots:
    void reject(void);
    void accept(void);
    virtual void getExternVal(const QString& var_name, const QVariant& var_val);
    
signals:
    void emitExternVal(const QString& var_name, const QVariant& var_val); // identificador de
    //  variable y valor
    void emitActivated(const QString& );
    void emitDialogSaved();

protected:
    void closeEvent(QCloseEvent* e);
    bool findWidget(QByteArray w_name, const char* clname);
    virtual void errMsg(QewErrorMsg id = QewUndefinedError);
    virtual uint pagesNumber(void);
    virtual QList<QWidget*> getContainerPages(void);
    virtual QWidget* addPage(QewExtensibleDialog* extensible_child, const QString& caption,
                             QTreeWidgetItem* parentItem = 0, int index = -1);
    virtual QWidget* addPageList(QList<QewExtensibleDialog*> extensible_children_list,
                                 QewExtensibleDialog* w_page, const QString& caption,
                                 QTreeWidgetItem * parentItem = 0, int index = -1);
    virtual QewExtensibleDialog* newPage(const QString& caption, const QByteArray& cont_name);
    virtual void removePage(QewExtensibleDialog* extensible_child, QTreeWidgetItem* parentItem = 0);

    void clean(void);
    void cleanChildren(void);
    virtual void cleanThis(void) {}
    void save(void);
    void saveChildren(void);
    virtual void saveThis(void) {}
    bool validate(void);
    bool validateChildren(void);
    virtual bool validateThis(void) { return true ; }
//    QList<QewExtensibleDialog*> & extensibleChildren(void);
    QWidget* containerWidget(void);
    virtual bool setContainerWidget(void);
    void setBaseContainerWidget(QWidget * w_container) { container_widget = w_container ; }
    void connectDialog(QewExtensibleDialog* dlg_child);
    void disconnectDialog(QewExtensibleDialog* dlg_child);

    QList<QewExtensibleDialog *>& extensibleChildren();

//    bool connectChild(QewExtensibleDialog* dlg_child, QewExtensibleDialog *other) ;
    virtual void removeLayoutMargin(void);
    bool isSetUpOk(void) { return setup_Ok ; }
    void setSetUpOk(bool ok) { setup_Ok = ok ; }
    QewExtensibleDialog* QewParent(void);
    QWidget *getButtonsWidget() const;
    bool setButtonsWidget(void);
    void enterEvent(QEvent * event);

    virtual void restoreThis();

    static QewExtensibleDialog* extensibleParent(QWidget* widget);
    QewExtensibleDialog* superTopParent();

    void setSameDialogForAllChildren(bool newSameDialogForAllChildren);
    bool getSameDialogForAllChildren() const;

private:
    QList<QewExtensibleDialog*> l_extensible_children;
    QWidget* container_widget;
    QByteArray container_name;
    QByteArray buttons_WidgetName;
    QWidget* buttonsWidget;
    bool setup_Ok;
    bool acceptMeansClose;
    bool dataChanged;
    bool saveButtonShowsUpdate;
    bool sameDialogForAllChildren;

    QTreeWidgetItem* itemTree;
};

#endif

