#ifndef UDISKOPERATION_H
#define UDISKOPERATION_H

#include <QThread>
#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QDir>

// 功能：1.检测U盘；2.筛选U盘里的文件；3.读写文件；4.挂载
class UDiskOperation : public QObject
{
    Q_OBJECT
public:
    static UDiskOperation *instance();
    explicit UDiskOperation(QObject *parent = 0);
    ~UDiskOperation();
    
signals:
    void monitorChanged(bool exist);
    void findFilesFinished(const QStringList &files);
    void readFileFinished(const QByteArray &data);
    void writeFileFinished(int err);
    
public:
    static QString mountAny_dev;
    static bool mountAny();
    static void umountAny();

    void startMonitor();
    void stopMonitor();
    void findFiles(const QStringList &nameFilters,
                   QDir::Filters filters = QDir::NoFilter,
                   QDir::SortFlags sort = QDir::NoSort);
    void readFile(const QString &filename);
    void writeFile(const QString &filename, const QByteArray &data);

private:
    Q_INVOKABLE void pri_startMonitor();
    Q_INVOKABLE void pri_stopMonitor();
    Q_INVOKABLE void pri_findFiles(const QStringList &nameFilters,
                                   int filters = QDir::NoFilter,
                                   int sort = QDir::NoSort);
    Q_INVOKABLE void pri_readFile(const QString &filename);
    Q_INVOKABLE void pri_writeFile(const QString &filename, const QByteArray &data);

private slots:
    void onMonitor();

public:
    static QStringList monitor();
    static bool mount(const QString &dev) ;
    static void umount(const QString &dev) ;

private:
    QStringList m_devs;
    QTimer m_timer_monitor_sd;
    int m_timeri_monitor_sd;
};

#include <QWidget>

class UDiskOperationWidget : public QWidget
{
    Q_OBJECT
public:
    UDiskOperationWidget(QWidget *parent);
    ~UDiskOperationWidget();

    void findFiles(const QString &str = "*.dat");

    bool monitorResult;
    QStringList findFilesResult;

signals:
    void monitorChanged(bool exist);
    void findFilesFinished(const QStringList &files);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void onMonitorChanged(bool exist);
    void onFindFilesFinished(const QStringList &files);
};

#endif // UDISKOPERATION_H
