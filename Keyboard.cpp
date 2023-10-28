#include "Keyboard.h"
#include "ui_Keyboard.h"
#include <QButtonGroup>

Keyboard::Keyboard(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::Keyboard)
{
    ui->setupUi(this);
    uis_title = windowTitle();
    uis_prompt = ui->v_prompt->text();
    ui->w_prompt->hide();

    m_isRange = false;
    m_abcIndex = 0;
    m_abcLast = 0;
    m_abcTimerId = 0;

    QButtonGroup *btg_fun = new QButtonGroup(this);
    btg_fun->addButton(ui->Number, 0);
    btg_fun->addButton(ui->Cap, 1);
    btg_fun->addButton(ui->Lower, 2);
    connect(btg_fun, SIGNAL(buttonClicked(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));
    ui->Cap->click();

    connect(ui->n_0, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_1, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_2, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_3, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_4, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_5, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_6, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_7, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_8, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->n_9, &QPushButton::clicked, this, &Keyboard::onKey);

    connect(ui->ABC, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->DEF, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->GHI, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->JKL, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->MNO, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->PQRS, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->TUV, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->WXYZ, &QPushButton::clicked, this, &Keyboard::onKey);

    connect(ui->abc, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->def, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->ghi, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->jkl, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->mno, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->pqrs, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->tuv, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->wxyz, &QPushButton::clicked, this, &Keyboard::onKey);

    connect(ui->dot, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->dot_C, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->dot_l, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->minus, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->minus_C, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->minus_l, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->space_C, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->space_l, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->at_C, &QPushButton::clicked, this, &Keyboard::onKey);
    connect(ui->at_l, &QPushButton::clicked, this, &Keyboard::onKey);
}

Keyboard::~Keyboard()
{
    delete ui;
}

QString Keyboard::value()
{
    return ui->lineEdit->text();
}

void Keyboard::setOnlyNumberMode()
{
    ui->Number->click();
    ui->Number->hide();
    ui->Cap->hide();
    ui->Lower->hide();
    resize(450, 480-73-6);
}

void Keyboard::setPasswordMode()
{
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

void Keyboard::reset(const KbInputInfo &ii, const QString &title, const QString &help)
{
    m_ii = ii;
    setWindowTitle(!title.isEmpty() ? title : uis_title);
    setHelpText(help);

    abcCancel();
    ui->v_prompt->setText(uis_prompt + ii.prompt());
    ui->w_prompt->setVisible(!ii.prompt().isEmpty());
    ui->lineEdit->blockSignals(true);
    ui->lineEdit->setMaxLength(ii.maxlen());
    ui->lineEdit->setText(ii.value());
    ui->lineEdit->clearFocus();
    ui->lineEdit->blockSignals(false);
    afterKey();
}

void Keyboard::timerEvent(QTimerEvent *)
{
    abcCancel();
}

void Keyboard::on_Enter_clicked()
{
    abcCancel();
    accept();
}

void Keyboard::on_Cancel_clicked()
{
    abcCancel();
    reject();
}

void Keyboard::on_Clear_clicked()
{
    abcCancel();
    ui->lineEdit->clear();
}

void Keyboard::on_Backspace_clicked()
{
    abcCancel();
    ui->lineEdit->backspace();
}

void Keyboard::onKey()
{
    QPushButton *btn = (QPushButton *)sender();
    QString text = btn->text();

    if (text.size() == 1) {
        abcCancel();
        if (m_ii.type() == '0' && text.at(0) == '-') {
            QString value = ui->lineEdit->text();
            if (value.isEmpty() || value.at(0) != '-') {
                value = text + value;
                ui->lineEdit->setText(value);
            }
            else {
                value = value.right(value.size() - 1);
                ui->lineEdit->setText(value);
            }
        }
        else
            ui->lineEdit->insert(text);
    }
    else if (text.size() == 0) {
        abcCancel();
        ui->lineEdit->insert(" ");
    }
    else {
        if (m_abcLast == btn)
            ui->lineEdit->backspace();
        else if (ui->lineEdit->text().size() >=
                 ui->lineEdit->maxLength()) {
            abcCancel();
            return;
        }
        else if (m_abcLast) {
            ui->lineEdit->deselect();
            m_abcIndex = 0;
        }

        int texti = m_abcIndex % text.size();

        if (m_abcTimerId)
            killTimer(m_abcTimerId);
        m_abcTimerId = startTimer(750);
        m_abcLast = btn;
        m_abcIndex++;

        ui->lineEdit->insert(text.at(texti));
        int cpos = ui->lineEdit->cursorPosition();
        ui->lineEdit->setSelection(cpos-1, 1);
    }
}

void Keyboard::on_lineEdit_textChanged(const QString &)
{
    afterKey();
}

void Keyboard::abcCancel()
{
    if (m_abcLast == 0)
        return;

    if (m_abcTimerId)
        killTimer(m_abcTimerId);
    m_abcTimerId = 0;
    m_abcLast = 0;
    m_abcIndex = 0;
    ui->lineEdit->deselect();
}

void Keyboard::afterKey()
{
    QString text = ui->lineEdit->text();
    m_isRange = m_ii.isRange(text);
    ui->Enter->setEnabled(m_isRange);
    if (m_isRange) {
        ui->v_prompt->setStyleSheet("color: green");
    } else {
        ui->v_prompt->setStyleSheet("color: red");
    }
}
