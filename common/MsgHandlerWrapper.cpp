#include "MsgHandlerWrapper.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QMetaType>
#include <QMutex>

MsgHandlerWrapper *MsgHandlerWrapper::instance()
{
    static MsgHandlerWrapper *_instance = 0;
    static QMutex _mutex;

    if (!_instance) {
        _mutex.lock();
        if (!_instance)
            _instance = new MsgHandlerWrapper;
        _mutex.unlock();
    }

    return _instance;
}

void static msgHandlerFunction(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QMetaObject::invokeMethod(MsgHandlerWrapper::instance(), "message",
                              Q_ARG(QtMsgType, type),
                              Q_ARG(QString, msg));
}

MsgHandlerWrapper::MsgHandlerWrapper() :
    QObject()
{
    qRegisterMetaType<QtMsgType>("QtMsgType");
    qInstallMessageHandler(msgHandlerFunction);
}
