#include "DlgArm.h"
#include "DlgArmHelp.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QFontMetrics>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

#ifndef Q_OS_WIN
#define MG 9
#define BD 1
#define SD 19
#define BSD (BD+SD)
#else
#define MG 9
#define BD 1
#define SD 0
#define BSD (BD+SD)
#endif

DlgArm::DlgArm(QWidget *parent, Qt::WindowFlags f) :
    QDialog(0)
{
    tit_height = QFontMetrics(font()).height() + MG+MG;

    dlg = new QDialog(parent, f);
#ifndef Q_OS_WIN
    dlg->setWindowFlags(Qt::FramelessWindowHint | dlg->windowFlags());
    dlg->setAttribute(Qt::WA_TranslucentBackground);
#endif
    tit = new QWidget(dlg);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(BD, BD, BD, BD);
    layout->setSpacing(BD);
    tit_title = new QLabel;
    tit_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tit_title->setAlignment(Qt::AlignCenter);
    layout->addWidget(tit_title);
    tit->setLayout(layout);
    tit->setStyleSheet(".QWidget{ background: #C0C0C0; "
                       "border: 1px solid #C0B0A0; "
                       "border-bottom: 1px solid #C0C0C0; "
                       "border-top-left-radius: 5px; "
                       "border-top-right-radius: 5px; }");
    tit_help = new QPushButton(tit);
    tit_help->setVisible(false);
    tit_help->setText("?");
    tit_help->setShortcut(QKeySequence::WhatsThis);
    tit_help->setFocusPolicy(Qt::NoFocus);
    tit_help->setStyleSheet("border-top-right-radius: 5px;");
    connect(tit_help, SIGNAL(clicked()), this, SLOT(showHelpText()));
    body = new QWidget(dlg);
    body->setStyleSheet(".QWidget{ background: #EFEFEF; "
                        "border: 1px solid #C0B0A0; "
                        "border-top: 0px; "
                        "border-bottom-left-radius: 5px; "
                        "border-bottom-right-radius: 5px; }");
#ifndef Q_OS_WIN
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(dlg);
    effect->setColor(QColor(0, 0, 0, 128));
    effect->setBlurRadius(19);
    effect->setOffset(0, 0);
    dlg->setGraphicsEffect(effect);
#endif

    setParent(dlg);
    connect(dlg, &QDialog::finished, this, &DlgArm::finished);
    connect(dlg, &QDialog::accepted, this, &DlgArm::accepted);
    connect(dlg, &QDialog::rejected, this, &DlgArm::rejected);
}

DlgArm::~DlgArm()
{
    QTimer::singleShot(1000, dlg, SLOT(deleteLater()));
}

void DlgArm::setHelpText(const QString &text)
{
    tit_help->setVisible(!text.isEmpty());
    helptext = text;
}

void DlgArm::showHelpText()
{
    DlgArmHelp dlg(this);
    dlg.setWindowTitle(windowTitle());
    dlg.setText(helptext);
    dlg.exec();
}

void DlgArm::setVisible(bool visible)
{
    dlg->setVisible(visible);
    QWidget::setVisible(visible);
}

void DlgArm::open()
{
    dlg->open();
    QDialog::setResult(dlg->result());
}

int DlgArm::exec()
{
    int r = dlg->exec();
    QDialog::setResult(r);
    return r;
}

void DlgArm::done(int r)
{
    dlg->done(r);
    QDialog::setResult(r);
}

void DlgArm::accept()
{
    done(Accepted);
}

void DlgArm::reject()
{
    done(Rejected);
}

bool DlgArm::event(QEvent *ev)
{
    switch (ev->type()) {
    case QEvent::Show: {
        QTimer::singleShot(100, this, SLOT(update()));
        break;
    }
    case QEvent::FontChange: {
        tit_height = QFontMetrics(font()).height() + MG+MG;
        fixThisSize();
        break;
    }
    case QEvent::WindowTitleChange: {
        tit_title->setText(windowTitle());
        break;
    }
    case QEvent::Move: {
        this->move(BSD, BSD+BD + tit_height);
        break;
    }
    case QEvent::Resize: {
        fixThisSize();
        break;
    }
    case QEvent::Close:
        dlg->close();
        break;
    default:
        break;
    }
    return QDialog::event(ev);
}

void DlgArm::fixThisSize()
{
    QSize s = this->size();
    int dlgw = s.width() + BSD+BSD;
    int dlgh = s.height() + BSD+BSD+BD + tit_height;
    dlg->resize(dlgw, dlgh);
    int w = s.width() + BD+BD;
    int h = BD+BD + tit_height;
    tit->setGeometry(SD, SD, w, h);
    body->setGeometry(SD, SD + h, w, s.height() + BD);
    this->move(BSD, BSD+BD + tit_height);

    tit_help->move(w-BD-tit_height, BD);
    tit_help->setFixedSize(tit_height, tit_height);
}
