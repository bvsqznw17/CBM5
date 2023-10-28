#ifndef FORMMAINREC_H
#define FORMMAINREC_H

#include <QWidget>
#include "FormA.h"
#include "RecOperate.h"

namespace Ui {
class FormMainRec;
}

class FormMainRec : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainRec(QWidget *parent = 0);
    ~FormMainRec();
    void init(FormA *f);
    void pageTop();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onQueryFns(QVariant v);
    void onQueryFns_wd(QVariant v);
    void onQueryFns_bar(QVariant v);
    void onRecUpdate();

    void on_lineup_clicked();

    void on_linedown_clicked();

    void on_pageup_clicked();

    void on_pagedown_clicked();

    void on_jt_up_clicked();

    void on_jt_down_clicked();

    void on_ClearRec_clicked();

    void on_ExportRec_clicked();

    void on_WeightDeviation_clicked();

    void on_barGraph_clicked();

private:
    void queryRec();
    void queryRec_wd();
    void updateRec();

private:
    Ui::FormMainRec *ui;
    FormA *form;
    MPTRWidget *mptrw;
    RecOperate *reco;
    RecQ_Handy rq;
    RecQR_Handy rqr;
    int isQuerying;
    bool queryWait;

    class DialogWeightDeviation *dlg_wd;
    RecOperate *reco_wd;
    RecQ_Handy rq_wd;
    class DialogGraphBar *dlg_bar;
};

#endif // FORMMAINREC_H
