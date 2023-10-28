#ifndef DIALOGSTARTUP_H
#define DIALOGSTARTUP_H

#include <QDialog>

namespace Ui {
class DialogStartup;
}

class DialogStartup : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogStartup(int processCount, QWidget *parent = 0);
    ~DialogStartup();

    inline void initInterval(int interval_) { interval = interval_; }
    inline void initMaximum(int maximum_) { maximum = maximum_; }

public slots:
    void progress();

signals:
    void processStarted();
    void processFinished();
    void processFinished0();

protected:
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);

private:
    Q_INVOKABLE void startProcess();
    bool finishProcess();
    void killTimer();
    
private:
    Ui::DialogStartup *ui;
    int timerid;
    int interval;
    int maximum;
    int curvalue;
    int countf;
    int countt;
    bool isProcessStarted;
    bool isProcessFinished;
    bool isProcessFinished0;
};

#endif // DIALOGSTARTUP_H
