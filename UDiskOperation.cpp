#include "UDiskOperation.h"
#include <QProcess>

UDiskOperation *UDiskOperation::instance()
{
    return 0;
}

UDiskOperation::UDiskOperation(QObject *parent) :
    QObject(parent),
    m_timer_monitor_sd(this),
    m_timeri_monitor_sd(1000)
{
    m_timer_monitor_sd.setInterval(m_timeri_monitor_sd);
    connect(&m_timer_monitor_sd, SIGNAL(timeout()),
            this, SLOT(onMonitor()));
}

UDiskOperation::~UDiskOperation()
{
}

QString UDiskOperation::mountAny_dev;

bool UDiskOperation::mountAny()
{
    umountAny();

    QStringList devs = monitor();
    foreach (const QString &str, devs) {
        if (mount(str)) {
            mountAny_dev = str;
            return true;
        }
    }
    return false;
}

void UDiskOperation::umountAny()
{
    if (!mountAny_dev.isEmpty()) {
        umount(mountAny_dev);
        mountAny_dev.clear();
    }
}

void UDiskOperation::startMonitor()
{
    QMetaObject::invokeMethod(this, "pri_startMonitor");
}

void UDiskOperation::stopMonitor()
{
    QMetaObject::invokeMethod(this, "pri_stopMonitor");
}

void UDiskOperation::findFiles(const QStringList &nameFilters, QDir::Filters filters,
                               QDir::SortFlags sort)
{
    QMetaObject::invokeMethod(this, "pri_findFiles",
                              Q_ARG(QStringList, nameFilters),
                              Q_ARG(int, filters),
                              Q_ARG(int, sort));
}

void UDiskOperation::readFile(const QString &filename)
{
    QMetaObject::invokeMethod(this, "pri_readFile",
                              Q_ARG(QString, filename));
}

void UDiskOperation::writeFile(const QString &filename, const QByteArray &data)
{
    QMetaObject::invokeMethod(this, "pri_writeFile",
                              Q_ARG(QString, filename),
                              Q_ARG(QByteArray, data));
}

void UDiskOperation::pri_startMonitor()
{
    m_devs.clear();
    m_devs.append("");
    m_timer_monitor_sd.start();
    onMonitor();
}

void UDiskOperation::pri_stopMonitor()
{
    m_timer_monitor_sd.stop();
}

void UDiskOperation::pri_findFiles(const QStringList &nameFilters, int filters, int sort)
{
    QStringList list;

#ifdef Q_OS_UNIX
    foreach (const QString &str, m_devs) {
        if (!mount(str))
            continue;

        QDir dir("/mnt/usb/");
        list = dir.entryList(nameFilters, (QDir::Filters)filters, (QDir::SortFlags)sort);
        umount(str);

        break;
    }
#else
    list = QDir::current().entryList(nameFilters, (QDir::Filters)filters, (QDir::SortFlags)sort);
#endif

    emit findFilesFinished(list);
}

void UDiskOperation::pri_readFile(const QString &filename)
{
    QByteArray data;

#ifdef Q_OS_UNIX
    foreach (const QString &str, m_devs) {
        if (!mount(str))
            continue;

        QFile file(QString("/mnt/usb/%1").arg(filename));
        file.open(QFile::ReadOnly);
        data = file.readAll();
        file.close();
        umount(str);

        break;
    }
#else
    QFile file(filename);
    file.open(QFile::ReadOnly);
    data = file.readAll();
    file.close();
#endif

    emit readFileFinished(data);
}

void UDiskOperation::pri_writeFile(const QString &filename, const QByteArray &data)
{
    int err = -1;

#ifdef Q_OS_UNIX
    foreach (const QString &str, m_devs) {
        if (!mount(str))
            continue;

        QFile file(QString("/mnt/usb/%1").arg(filename));
        file.open(QFile::WriteOnly | QFile::Truncate);
        file.write(data);
        file.close();
        err = file.error();
#ifdef __ARM_EABI__
        system("sync");
#endif
        umount(str);

        break;
    }
#else
    QFile file(filename);
    file.open(QFile::WriteOnly | QFile::Truncate);
    file.write(data);
    file.close();
    err = file.error();
#endif

    emit writeFileFinished(err);
}

void UDiskOperation::onMonitor()
{
#ifdef Q_OS_UNIX
    QStringList devs = monitor();
    if (devs.size() != m_devs.size() ||
            (!m_devs.isEmpty() && m_devs.at(0).isEmpty())) {
        m_devs = devs;
        emit monitorChanged(!m_devs.isEmpty());
    }
    m_devs = devs;
#endif
}

QStringList UDiskOperation::monitor()
{
    QStringList devs;
#ifdef Q_OS_UNIX
    // 遍历dev
    QProcess p;
    p.setStandardErrorFile("/dev/null");
    p.start("ls", QStringList() << "/dev/");
    p.waitForFinished(-1);

    QStringList list = QString::fromUtf8(p.readAllStandardOutput()).split('\n');
    QRegExp rx("sd.*[0-9]*");
    foreach (const QString &str, list) {
        if(rx.exactMatch(str))
            devs.append(str);
    }
#endif
    return devs;
}

bool UDiskOperation::mount(const QString &dev)
{
    QDir::current().mkpath("/mnt/usb");
    for (int i = 0; ; i++)  {
        if (!::system(QString("mount /dev/%1 /mnt/usb 1> /dev/null 2> /dev/null")
                      .arg(dev).toLatin1().constData()))
            return true;
        // 如果挂载失败，则卸载之后再尝试，最多2次
        if (i >= 2) return false;
        ::system(QString("umount /dev/%1 1> /dev/null 2> /dev/null")
                 .arg(dev).toLatin1().constData());
        ::system(QString("umount /mnt/usb 1> /dev/null 2> /dev/null")
                 .toLatin1().constData());
    }
    return false;
}

void UDiskOperation::umount(const QString &dev)
{
    for (int i = 0; ; i++)  {
        if (!::system(QString("umount /dev/%1 1> /dev/null 2> /dev/null")
                     .arg(dev).toLatin1().constData()))
            return;
        // 如果卸载失败，则等待500ms再尝试，最多2次
        if (i >= 2) return;
        QThread::msleep(500);
    }
}

UDiskOperationWidget::UDiskOperationWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(0, 0);

    monitorResult = false;
    findFilesResult.clear();

    connect(UDiskOperation::instance(), SIGNAL(monitorChanged(bool)),
            this, SLOT(onMonitorChanged(bool)));
    connect(UDiskOperation::instance(), SIGNAL(findFilesFinished(QStringList)),
            this, SLOT(onFindFilesFinished(QStringList)));
}

UDiskOperationWidget::~UDiskOperationWidget()
{
}

void UDiskOperationWidget::findFiles(const QString &str)
{
    UDiskOperation::instance()->findFiles(QStringList(str));
}

void UDiskOperationWidget::showEvent(QShowEvent *)
{
#ifndef MYOPT_WINDOW
    monitorResult = false;
    emit monitorChanged(monitorResult);
    UDiskOperation::instance()->startMonitor();
#else
    monitorResult = true;
    emit monitorChanged(monitorResult);
#endif
}

void UDiskOperationWidget::hideEvent(QHideEvent *)
{
#ifndef MYOPT_WINDOW
    UDiskOperation::instance()->stopMonitor();
#endif
}

void UDiskOperationWidget::onMonitorChanged(bool exist)
{
#ifndef MYOPT_WINDOW
    monitorResult = exist;
    emit monitorChanged(monitorResult);
#else
    Q_UNUSED(exist);
#endif
}

void UDiskOperationWidget::onFindFilesFinished(const QStringList &files)
{
    findFilesResult = files;
    emit findFilesFinished(findFilesResult);
}
