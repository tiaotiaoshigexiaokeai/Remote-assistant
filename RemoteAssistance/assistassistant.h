#ifndef ASSISTASSISTANT_H
#define ASSISTASSISTANT_H
#include <DMainWindow>
#include <DWidget>
#include <QPainter>
#include <DToolButton>
#include <QVBoxLayout>
#include <DLabel>
#include <DTitlebar>

DWIDGET_USE_NAMESPACE

class AssistAssistant : public DMainWindow
{
    Q_OBJECT
public:
    AssistAssistant(DWidget *parent = nullptr);

private:
    void head_tit(const QString &path,const QString &name);
    void m_assist();
    void m_connect();
    void need_help();
    void give_help();
    void need_one();
    void need_two();
    void connection_failed();
    void connection_succeeded();
    void give_link();
    void give_stop();

private slots:
    void HelpSlot();
    void GiveSlot();
//    void HelpOne();
    //void judgeCord();
    void give_one();

private:
     DStackedWidget *stack;
     DToolButton *helpbutton;
     DToolButton *givebutton;
     DLabel *label;
     DWidget *wid;
     DLabel *head_picture;
     DLabel *digital_label;
     //DWidget *m_widget;
     QImage *image;

     DStackedWidget *n_help;
     DWidget *help_one;
     DWidget *help_two;
     DWidget *help_three;


     DStackedWidget *g_help;
     DWidget *server_one;
     DWidget *server_two;
     DWidget *server_three;
     DWidget *server_four;

     QString str_num1;
     QString str_code;
     QLineEdit *g_edit = nullptr;
     DPushButton *g_button = nullptr;

     DTitlebar *tit;
     DWidget *head;
     DLabel *left;
     DLabel *right;
};

#endif // ASSISTASSISTANT_H
