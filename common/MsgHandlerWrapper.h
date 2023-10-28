#ifndef MSGHANDLERWRAPPER_H
#define MSGHANDLERWRAPPER_H

#include <QObject>

class MsgHandlerWrapper : public QObject
{
    Q_OBJECT
public:
    static MsgHandlerWrapper *instance();

signals:
    void message(QtMsgType type, const QString &msg);

private:
    MsgHandlerWrapper();
};

#endif // MSGHANDLERWRAPPER_H
