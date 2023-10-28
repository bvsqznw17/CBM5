QT       += core gui serialport sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TRANSLATIONS = \
lang_Chinese.ts \
lang_Chinese_Traditional.ts \
lang_English.ts \
lang_German.ts \
lang_French.ts \
lang_Italian.ts \
lang_Portuguese.ts \
lang_Spanish.ts

CODECFORTR = UTF-8
CODECFORSRC = UTF-8

TARGET = MainProgram

#If the compiler for the ARM version, use MY_ARM
#If the compiler for the X11 version, use MY_X11

contains(QT_ARCH, arm|arm64) {
DEFINES += MY_ARM
DEFINES += __ARM_EABI__
#DEFINES += MYOPT_TEST
} else {
unix {
DEFINES += MY_X11
DEFINES += MYOPT_TEST
DEFINES += MYOPT_DEBUG
greaterThan(QT_MAJOR_VERSION, 4): QMAKE_LFLAGS += -no-pie
} else {
DEFINES += MY_WIN
DEFINES += MYOPT_WINDOW
#DEFINES += MYOPT_MCTCP
#DEFINES += MYOPT_MCTCP_S
#DEFINES += MYOPT_OnlineManager
}
}

DEFINES += MYOPT_LocalTime

contains( DEFINES, MY_ARM ) {
LIBS += $$PWD/lib/usbh/libusb-1.0.so
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
}
contains( DEFINES, MY_X11 ) {
LIBS += $$PWD/lib/usbh/x86_libusb-1.0.so.0
INCLUDEPATH += /usr/OpenCv-X86/include/
LIBS += /usr/OpenCv-X86/lib/libopencv_core.so.2.4.9 \
        /usr/OpenCv-X86/lib/libopencv_highgui.so.2.4.9 \
        /usr/OpenCv-X86/lib/libopencv_imgproc.so.2.4.9\
        /usr/OpenCv-X86/lib/libopencv_ml.so.2.4.9 \
        /usr/OpenCv-X86/lib/libopencv_video.so.2.4.9
}
contains( DEFINES, MY_WIN ) {
LIBS += -lwsock32 -lws2_32
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += marm/
INCLUDEPATH += common/
INCLUDEPATH += Printer/

SOURCES += \
    CameraJoint.cpp \
    CameraThread.cpp \
    CameraWidget.cpp \
    CheckBox.cpp \
    Config.cpp \
    DialogDebugMM.cpp \
    DialogDouStateAdjust.cpp \
    DialogGraphBar.cpp \
    DialogSetSpecify.cpp \
    DialogWeightDeviation.cpp \
    DlgArm.cpp \
    DlgArmHelp.cpp \
    DlgInfo.cpp \
    DlgList.cpp \
    DlgOKCancel.cpp \
    DlgProgress.cpp \
    DlgWaiting.cpp \
    Doum.cpp \
    Form.cpp \
    FormA.cpp \
    FormCamera.cpp \
    FormCameraCalib.cpp \
    FormHelp.cpp \
    FormHelpDouState.cpp \
    FormHelpIOState.cpp \
    FormMain.cpp \
    FormMainAlarm.cpp \
    FormMainComb.cpp \
    FormMainModifyParam.cpp \
    FormMainModifyTime.cpp \
    FormMainRec.cpp \
    FormMainZero.cpp \
    FormMenu.cpp \
    FormMenuCalib.cpp \
    FormMenuDebug.cpp \
    FormMenuHSet.cpp \
    FormMenuHSetMM.cpp \
    FormMenuHSetMM1.cpp \
    FormMenuHSetMM2.cpp \
    FormMenuHSetMMA.cpp \
    FormMenuHSetMMA1.cpp \
    FormMenuHSetMainboard.cpp \
    FormMenuHSetPBak.cpp \
    FormMenuHSetPL.cpp \
    FormMenuHSetSortor.cpp \
    FormMenuHSetSysParam.cpp \
    FormMenuIO.cpp \
    FormMenuIO1.cpp \
    FormMenuIO2.cpp \
    FormMenuMac.cpp \
    FormMenuMac1.cpp \
    FormMenuMac2.cpp \
    FormMenuPList.cpp \
    FormMenuPSet.cpp \
    FormMenuPSetBase.cpp \
    FormMenuPSetMM.cpp \
    FormMenuPSetOther.cpp \
    FormMenuPSetSpecify.cpp \
    FormMenuPSetTime.cpp \
    FormMenuPSetVib.cpp \
    FormSamping.cpp \
    FormTestDouKaiMen.cpp \
    FormTestZhenDong.cpp \
    GraphBar.cpp \
    GraphDraw.cpp \
    GraphMotorMode.cpp \
    GraphMotorMode2.cpp \
    GraphSamping.cpp \
    GraphWeightDeviation.cpp \
    GraphZhenDong.cpp \
    JServer_Local.cpp \
    JServer_Mac.cpp \
    JServer_NetUp.cpp \
    JServer_Rec.cpp \
    JsonTcpServer.cpp \
    JsonTcpTool.cpp \
    Keyboard.cpp \
    KeyboardInputInfo.cpp \
    KeyboardManager.cpp \
    Language.cpp \
    MPRWOperation.cpp \
    MachineParamList0.cpp \
    Printer/PrintThread.cpp \
    Printer/Printer.cpp \
    Printer/SPrinter.cpp \
    Printer/USBDevice.cpp \
    QLabel2.cpp \
    RecDB.cpp \
    RecOperate.cpp \
    RecSave.cpp \
    TcpComm.cpp \
    Thread.cpp \
    Tool.cpp \
    UDiskOperation.cpp \
    WiFiCheckThread.cpp \
    common/DialogStartup.cpp \
    common/MTools.cpp \
    common/MsgHandlerWrapper.cpp \
    common/UIDebug.cpp \
    main.cpp \
    marm/CRC.cpp \
    marm/MPTReader.cpp \
    marm/MWidget.cpp \
    marm/Machine.cpp \
    marm/MachineComm.cpp \
    marm/MachineParamList.cpp \
    marm/ModbusTcpServer.cpp \
    marm/MoudbusVirtualSlave.cpp \
    marm/SignalEcho.cpp \
    sampingcurve.cpp

HEADERS += \
    CameraJoint.h \
    CameraThread.h \
    CameraWidget.h \
    CheckBox.h \
    Config.h \
    DialogDebugMM.h \
    DialogDouStateAdjust.h \
    DialogGraphBar.h \
    DialogSetSpecify.h \
    DialogWeightDeviation.h \
    DlgArm.h \
    DlgArmHelp.h \
    DlgInfo.h \
    DlgList.h \
    DlgOKCancel.h \
    DlgProgress.h \
    DlgWaiting.h \
    Doum.h \
    Form.h \
    FormA.h \
    FormCamera.h \
    FormCameraCalib.h \
    FormHelp.h \
    FormHelpDouState.h \
    FormHelpIOState.h \
    FormMain.h \
    FormMainAlarm.h \
    FormMainComb.h \
    FormMainModifyParam.h \
    FormMainModifyTime.h \
    FormMainRec.h \
    FormMainZero.h \
    FormMenu.h \
    FormMenuCalib.h \
    FormMenuDebug.h \
    FormMenuHSet.h \
    FormMenuHSetMM.h \
    FormMenuHSetMM1.h \
    FormMenuHSetMM2.h \
    FormMenuHSetMMA.h \
    FormMenuHSetMMA1.h \
    FormMenuHSetMainboard.h \
    FormMenuHSetPBak.h \
    FormMenuHSetPL.h \
    FormMenuHSetSortor.h \
    FormMenuHSetSysParam.h \
    FormMenuIO.h \
    FormMenuIO1.h \
    FormMenuIO2.h \
    FormMenuMac.h \
    FormMenuMac1.h \
    FormMenuMac2.h \
    FormMenuPList.h \
    FormMenuPSet.h \
    FormMenuPSetBase.h \
    FormMenuPSetMM.h \
    FormMenuPSetOther.h \
    FormMenuPSetSpecify.h \
    FormMenuPSetTime.h \
    FormMenuPSetVib.h \
    FormSamping.h \
    FormTestDouKaiMen.h \
    FormTestZhenDong.h \
    GraphBar.h \
    GraphDraw.h \
    GraphMotorMode.h \
    GraphMotorMode2.h \
    GraphSamping.h \
    GraphWeightDeviation.h \
    GraphZhenDong.h \
    JServer_Local.h \
    JServer_Mac.h \
    JServer_NetUp.h \
    JServer_Rec.h \
    JsonTcpServer.h \
    JsonTcpTool.h \
    Keyboard.h \
    KeyboardInputInfo.h \
    KeyboardManager.h \
    Language.h \
    MPRWOperation.h \
    MachineParamList0.h \
    Printer/PrintThread.h \
    Printer/Printer.h \
    Printer/SPrinter.h \
    Printer/USBDevice.h \
    QLabel2.h \
    RecDB.h \
    RecOperate.h \
    RecSave.h \
    RingBuffer.h \
    TcpComm.h \
    Thread.h \
    Tool.h \
    UDiskOperation.h \
    WiFiCheckThread.h \
    cbDef.h \
    common/DialogStartup.h \
    common/MTools.h \
    common/MsgHandlerWrapper.h \
    common/UIDebug.h \
    marm/CRC.h \
    marm/MPTReader.h \
    marm/MWidget.h \
    marm/Machine.h \
    marm/MachineComm.h \
    marm/MachineParamList.h \
    marm/ModbusTcpServer.h \
    marm/MoudbusVirtualSlave.h \
    marm/SignalEcho.h \
    sampingcurve.h

FORMS += \
    DialogDebugMM.ui \
    DialogDouStateAdjust.ui \
    DialogGraphBar.ui \
    DialogSetSpecify.ui \
    DialogWeightDeviation.ui \
    DlgArmHelp.ui \
    DlgInfo.ui \
    DlgList.ui \
    DlgOKCancel.ui \
    DlgProgress.ui \
    DlgWaiting.ui \
    Form.ui \
    FormA.ui \
    FormCamera.ui \
    FormCamera2.ui \
    FormCameraCalib.ui \
    FormHelp.ui \
    FormHelpDouState.ui \
    FormHelpIOState.ui \
    FormMain.ui \
    FormMainAlarm.ui \
    FormMainComb.ui \
    FormMainModifyParam.ui \
    FormMainModifyTime.ui \
    FormMainRec.ui \
    FormMainZero.ui \
    FormMenu.ui \
    FormMenuCalib.ui \
    FormMenuDebug.ui \
    FormMenuHSet.ui \
    FormMenuHSetMM.ui \
    FormMenuHSetMM1.ui \
    FormMenuHSetMM2.ui \
    FormMenuHSetMMA.ui \
    FormMenuHSetMMA1.ui \
    FormMenuHSetMainboard.ui \
    FormMenuHSetPBak.ui \
    FormMenuHSetPL.ui \
    FormMenuHSetSortor.ui \
    FormMenuHSetSysParam.ui \
    FormMenuIO.ui \
    FormMenuIO1.ui \
    FormMenuIO2.ui \
    FormMenuMac.ui \
    FormMenuMac1.ui \
    FormMenuMac2.ui \
    FormMenuPList.ui \
    FormMenuPSet.ui \
    FormMenuPSetBase.ui \
    FormMenuPSetMM.ui \
    FormMenuPSetOther.ui \
    FormMenuPSetSpecify.ui \
    FormMenuPSetTime.ui \
    FormMenuPSetVib.ui \
    FormSamping.ui \
    FormTestDouKaiMen.ui \
    FormTestZhenDong.ui \
    Keyboard.ui \
    common/DialogStartup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc

unix {
MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
}

#contains( DEFINES, MY_ARM ) {
#INCLUDEPATH += /usr/local/opencv_arm/include/opencv2 \
#               /usr/local/opencv_arm/include/
#LIBS += /usr/local/opencv_arm/lib/libopencv_core.so \
#        /usr/local/opencv_arm/lib/libopencv_highgui.so \
#        /usr/local/opencv_arm/lib/libopencv_imgproc.so \
#        /usr/local/opencv_arm/lib/libopencv_ml.so \
#        /usr/local/opencv_arm/lib/libopencv_video.so
#}

#contains( DEFINES, MY_X11 ) {
#INCLUDEPATH += /usr/OpenCv-X86/include/opencv2 \
#                /usr/OpenCv-X86/include/
#LIBS += /usr/OpenCv-X86/lib/libopencv_core.so.2.4.9 \
#        /usr/OpenCv-X86/lib/libopencv_highgui.so.2.4.9 \
#        /usr/OpenCv-X86/lib/libopencv_imgproc.so.2.4.9\
#        /usr/OpenCv-X86/lib/libopencv_ml.so.2.4.9 \
#        /usr/OpenCv-X86/lib/libopencv_video.so.2.4.9
#}
