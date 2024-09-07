#ifndef QEWTREEPAGE_HXX
#define QEWTREEPAGE_HXX

#include <QWidget>

class QewTreePage : public QWidget
{
    Q_OBJECT
public:
    explicit QewTreePage(QWidget *parent = nullptr);
    ~QewTreePage();

signals:
    void emitActivated(bool, const QString&);
public slots:

protected:
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
};

#endif // QEWTREEPAGE_H
