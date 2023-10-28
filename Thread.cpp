#include "Thread.h"
#include "MTools.h"
#include "WiFiCheckThread.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
{
}

Thread::~Thread()
{
    wait();
}

void Thread::loadLang(Priority p)
{
    fun = LoadLang;
    start(p);
}

void Thread::changeLang(int lid, Priority p)
{
    fun = ChangeLang;
    lang_id = lid;
    start(p);
}

void Thread::initWifi(QThread::Priority p)
{
    fun = InitWifi;
    start(p);
}

void Thread::run()
{
    switch (fun) {
    case LoadLang:
        MTools::loadLang();
        lang_imgs = QImagesManager::load();
        break;
    case ChangeLang:
        MTools::changeLang(lang_id);
        lang_imgs = QImagesManager::load();
        break;
    case InitWifi:
        // 初始化WiFi
        #ifdef __ARM_EABI__
        ::system("killall wpa_supplicant");
        ::system("killall udhcpc");
        ::system("killall hostapd");
        ::system("killall udhcpd");

        ::system("ifconfig wlan0 down");
        ::system("ifconfig wlan0 up");
        if (MTools::isOpenWifi()) {
            ::system("wpa_supplicant -D wext -B -i wlan0 -c wifi.conf &");
            ::system("udhcpc -b -i wlan0 -R &");
        }

        g_wific.start();
        #endif
        break;
    }
}
