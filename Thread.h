#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "MWidget.h"

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    ~Thread();

    void loadLang(Priority = InheritPriority);
    void changeLang(int lid, Priority = InheritPriority);
    Q_SLOT void initWifi(Priority = InheritPriority);
    QHash<QString, QImages> lang_imgs;

protected:
    virtual void run();

private:
    int lang_id;

    int fun;
    enum Fun {
        LoadLang,
        ChangeLang,
        InitWifi
    };
};

#endif // THREAD_H
