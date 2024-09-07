#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QList>

class QewExtensibleDialog;
class Connector
{
protected:
    Connector();

public:
    virtual ~Connector();
    static Connector* instance(void);

    void addDialog(QewExtensibleDialog* dialog);
    void removeDialog(QewExtensibleDialog* dialog);
    void clearList();

protected:
    static Connector* connector;
    QList<QewExtensibleDialog*> dialogs;
};

#endif // CONNECTOR_H
