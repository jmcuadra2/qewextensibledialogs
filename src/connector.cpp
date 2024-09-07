#include "connector.h"
#include "qewextensibledialog.h"

#include <QObject>

Connector* Connector::connector = 0;

Connector* Connector::instance(void)
{

    if(connector == 0)
        connector = new Connector;
    return connector;

}

Connector::Connector()
{

}

Connector::~Connector()
{

}

void Connector::addDialog(QewExtensibleDialog *dialog)
{
    if(dialogs.contains(dialog))
        return;

    dialogs.append(dialog);
    for(int i = 0; i < dialogs.size(); i++) {
        QObject::connect(dialog, SIGNAL(emitExternVal(const QString& , const QVariant& )),
                     dialogs.at(i), SLOT(getExternVal(const QString& , const QVariant& )));
        if(dialogs.at(i) != dialog)
            QObject::connect(dialogs.at(i), SIGNAL(emitExternVal(const QString& , const QVariant& )),
                     dialog, SLOT(getExternVal(const QString& , const QVariant& )));
    }
}

void Connector::removeDialog(QewExtensibleDialog *dialog)
{
    for(int i = 0; i < dialogs.size(); i++) {
        QObject::disconnect(dialog, SIGNAL(emitExternVal(const QString& , const QVariant& )),
                     dialogs.at(i), SLOT(getExternVal(const QString& , const QVariant& )));
        QObject::disconnect(dialogs.at(i), SIGNAL(emitExternVal(const QString& , const QVariant& )),
                     dialog, SLOT(getExternVal(const QString& , const QVariant& )));
    }
    dialogs.removeOne(dialog);
}

void Connector::clearList()
{
    dialogs.clear();
}
