#include "assistassistant.h"

#include <QApplication>
#include <DToolButton>
#include <DPushButton>
#include <QVBoxLayout>
#include <DLabel>
#include <QString>
#include <DStackedWidget>
#include <DWidget>
#include <DSpinner>
#include <DLineEdit>
#include <QTimer>
#include <QDebug>
#include <QClipboard>
#include <QString>
#include <QRandomGenerator>
#include <QRegExp>
#include <QElapsedTimer>

DWIDGET_USE_NAMESPACE

AssistAssistant::AssistAssistant(DWidget *parent)
: DMainWindow (parent)
{
    n_help = nullptr;
    help_one = nullptr;
    help_two = nullptr;
    help_three = nullptr;

    g_help = nullptr;
    server_one = nullptr;
    server_two = nullptr;
    server_three = nullptr;
    server_four = nullptr;

    m_assist();
    m_connect();

    tit = this->titlebar();
}

//----------------标题－－－－－－－－－－－//
void AssistAssistant::head_tit(const QString &path,const QString &name)
{
    if(!head)
    {
        head = new DWidget(this);

        QImage *ima=new QImage;
//        ima->load(":/images/theme.svg");
        left=new DLabel(this);
        left->setPixmap(QPixmap::fromImage(*ima));
        left->setScaledContents(true);
        left->setFixedSize(16,16);
        right=new DLabel(this);

        QHBoxLayout* myLayout = new QHBoxLayout();
        myLayout->addSpacing(100);
        myLayout->addWidget(left);
        myLayout->addSpacing(5);
        myLayout->addWidget(right);
        myLayout->addStretch(2);
        head->setLayout(myLayout);
        tit->setCustomWidget(head);
    }
    left->setPixmap(QPixmap(path));
    right->setText(name);
}


//----------------主页面－－－－－－－－－－－//
void AssistAssistant::m_assist()
{
    tSeek =new QTimer(this);
    tGo = new QTimer(this);
    DLabel  *head_picture = new DLabel;
    QImage *image = new QImage;
    image->load(":/images/hand.png");
    head_picture->setPixmap(QPixmap::fromImage(*image));
    head_picture->setScaledContents(true);
    head_picture->setFixedSize(80,80);

    helpbutton = new DToolButton;
    helpbutton->setText(QObject::tr("我要求助"));
    helpbutton->setIcon(QIcon(QPixmap(":/images/help.png")));
    helpbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon );

    givebutton = new DToolButton;
    givebutton->setText(QObject::tr("帮助别人"));
    givebutton->setIcon(QIcon(QPixmap("/home/archermind/RemoteAssistance/images/give.png")));
    givebutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon );

    label = new DLabel;
    QString str = QObject::tr("这是由deepin团队开发的远程协助应用，"
                              "如果您在\n使用的过程中遇到困难，或者友人求助于你，"
                              "请点\n击下方的我要求助或帮助别人。");
    label->setAlignment(Qt::AlignHCenter);
    QFont ft;
    ft.setPointSize(8);
    label->setFont(ft);
    DPalette pe;
    pe.setColor(DPalette::WindowText, Qt::gray);
    label->setPalette(pe);
    label->setText(str);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(5);
    layout->addWidget(head_picture,0,Qt::AlignHCenter);
    layout->addSpacing(5);
    layout->addWidget(label,0,Qt::AlignHCenter);
    layout->addSpacing(5);
    layout->addWidget(helpbutton,0,Qt::AlignHCenter);
    layout->addSpacing(5);
    layout->addWidget(givebutton,0,Qt::AlignHCenter);
    layout->addSpacing(30);

    wid=new DWidget(this);
    wid->setLayout(layout);

    stack=new DStackedWidget(this);
    stack->addWidget(wid);

    setCentralWidget(stack);
    setFixedSize(360,320);
}

//----------------主页面按钮连接信号槽－－－－－－－--//
void AssistAssistant::m_connect()
{
    connect(helpbutton,&DToolButton::pressed,this,&AssistAssistant::HelpSlot);
    connect(givebutton,&DToolButton::pressed,this,&AssistAssistant::GiveSlot);
    connect(tSeek,&QTimer::timeout,[=](){
        tSeek->stop();
        need_one();
    });
    connect(tGo,&QTimer::timeout,[=](){
        tGo->stop();
        if(str_code == str_num1){
            connection_succeeded();
        }
        else{
            connection_failed();
        }
    });
}


//----------------我要帮助　界面１－－－－－－－－//
void AssistAssistant::need_help()
{
    DSpinner *dsp = new DSpinner(this);
    dsp->setFixedSize(50,50);
    dsp->start();

    DLabel *n_label = new DLabel(this);
    QString n_str = QObject::tr("正在生成请求验证码，请稍后……");
    n_label->setAlignment(Qt::AlignHCenter);
    QFont n_ft;
    n_ft.setPointSize(11);
    n_label->setFont(n_ft);
    DPalette n_pe;
    n_pe.setColor(DPalette::WindowText, Qt::black);
    n_label->setPalette(n_pe);
    n_label->setText(n_str);

    DPushButton *n_button = new DPushButton(this);
    n_button->setText(QObject::tr("取消"));
    connect(n_button,&DPushButton::clicked,
            [=](){
        tSeek->stop();
        stack->setCurrentWidget(wid);
    });

    QVBoxLayout *n_layout = new QVBoxLayout(this);
    n_layout->addSpacing(20);
    n_layout->addWidget(dsp,0,Qt::AlignHCenter);
    n_layout->addSpacing(20);
    n_layout->addWidget(n_label,0,Qt::AlignHCenter);
    n_layout->addSpacing(10);
    n_layout->addWidget(n_button,0,Qt::AlignHCenter);
    n_layout->addSpacing(30);

    help_one=new DWidget(this);
    help_one->setLayout(n_layout);
    n_help->addWidget(help_one);
}

//----------------我要帮助　界面２－－－－－－－－//
void AssistAssistant::need_one()
{
    digital_label = new DLabel(this);
    QString str = QString::number(QRandomGenerator::global()->bounded(900000)+100000);
    QFont ft;
    ft.setPointSize(25);
    ft.setLetterSpacing(QFont::AbsoluteSpacing,18);
    digital_label->setFont(ft);
    DPalette pe;
    pe.setColor(DPalette::WindowText, Qt::black);
    digital_label->setPalette(pe);
    digital_label->setText(str);

    DLabel *one_label = new DLabel(this);
    one_label->setAlignment(Qt::AlignHCenter);
    QString one_str = QObject::tr("要开始共享你的桌面，请将上面的验证码提供给协助\n您的人员，"
                                  "他们输入验证码后，您的共享会话会立即开始");
    QFont one_ft;
    one_ft.setPointSize(8);
    one_label->setFont(one_ft);
    DPalette one_pe;
    one_pe.setColor(DPalette::WindowText, Qt::gray);
    one_label->setPalette(one_pe);
    one_label->setText(one_str);

    DPushButton *one_button = new DPushButton;
    one_button->setText(QObject::tr("复制"));
    connect(one_button,&DPushButton::clicked,this,
            [=](){
        QClipboard *one_board = QApplication::clipboard();
        one_board->setText(digital_label->text());
        str_code = one_board->text();
       // qDebug()<<"jianteeeee"<<one_board->text();
        need_two();
    });

    QVBoxLayout *one_layout = new QVBoxLayout(this);
    one_layout->addSpacing(20);
    one_layout->addWidget(digital_label,0,Qt::AlignHCenter);
    one_layout->addSpacing(20);
    one_layout->addWidget(one_label,0,Qt::AlignHCenter);
    one_layout->addSpacing(10);
    one_layout->addWidget(one_button,0,Qt::AlignHCenter);
    one_layout->addSpacing(30);

    help_two=new DWidget(this);
    help_two->setLayout(one_layout);
    n_help->addWidget(help_two);
    n_help->setCurrentWidget(help_two);
    //setFixedSize(360,320);
}

//----------------我要帮助　界面３－－－－－－－－//
void AssistAssistant::need_two()
{
    DLabel *two_label = new DLabel(this);
    two_label->setAlignment(Qt::AlignHCenter);
    QString str = QObject::tr("成功复制到粘贴版...");
    QFont ft;
    ft.setPointSize(20);
    two_label->setFont(ft);
    DPalette pe;
    pe.setColor(DPalette::WindowText, Qt::black);
    two_label->setPalette(pe);
    two_label->setText(str);

    DLabel *label = new DLabel(this);
    label->setAlignment(Qt::AlignHCenter);
    QString str1 = QObject::tr("正在等待连接，请稍后...\n"
                               "连接成功后，此界面会自动隐藏到任务栏");
    QFont ft1;
    ft1.setPointSize(8);
    label->setFont(ft1);
    DPalette pe1;
    pe1.setColor(DPalette::WindowText, Qt::gray);
    label->setPalette(pe1);
    label->setText(str1);

    DPushButton *button = new DPushButton;
    button->setText(QObject::tr("返回"));

    connect(button,&DPushButton::clicked,
            [=](){
        stack->setCurrentWidget(wid);
    }
            );

    QVBoxLayout *one_layout = new QVBoxLayout(this);
    one_layout->addSpacing(20);
    one_layout->addWidget(two_label,0,Qt::AlignHCenter);
    one_layout->addSpacing(20);
    one_layout->addWidget(label,0,Qt::AlignHCenter);
    one_layout->addSpacing(10);
    one_layout->addWidget(button,0,Qt::AlignHCenter);
    one_layout->addSpacing(30);

    help_three=new DWidget(this);
    help_three->setLayout(one_layout);
    n_help->addWidget(help_three);
    n_help->setCurrentWidget(help_three);

}

//----------------帮助别人　界面１－－－－－－－－//
void AssistAssistant::give_help()
{
    if(!server_one){
        g_edit = new QLineEdit(this);
        g_edit->setAlignment(Qt::AlignHCenter);
        QFont font;
        font.setLetterSpacing(QFont::AbsoluteSpacing,18);
        font.setPointSize(25);
        g_edit->setFont(font);
        QRegExp rx("[0-9]{6}");
        g_edit->setValidator(new QRegExpValidator(rx, this));

        connect(g_edit,&QLineEdit::textChanged,[=](const QString str){
            str_num1 = str;
            if(str.size()!=6){
                g_button->setText(QObject::tr("取消"));
            }
            else {
               g_button->setText(QObject::tr("连接"));
            }
        });

        DLabel *g_label = new DLabel(this);
        g_label->setAlignment(Qt::AlignHCenter);
        QString g_str = QObject::tr("请在上方输入你想帮助的人提供给你的验\n证码，完成后点击链接开始协助对方。");
        QFont g_ft;
        g_ft.setPointSize(10);
        g_label->setFont(g_ft);
        DPalette g_pe;
        g_pe.setColor(DPalette::WindowText, Qt::gray);
        g_label->setPalette(g_pe);
        g_label->setText(g_str);

        g_button = new DPushButton(this);
        g_button->setText(QObject::tr("取消"));
        connect(g_button,&DPushButton::clicked,
                [=](){
            if(g_button->text()=="取消"){
                stack->setCurrentWidget(wid);
            }
            else {
                give_one();
            }
        });


        QVBoxLayout *g_layout = new QVBoxLayout(this);
        g_layout->addSpacing(20);
        g_layout->addWidget(g_edit,0,Qt::AlignHCenter);
        g_layout->addSpacing(20);
        g_layout->addWidget(g_label,0,Qt::AlignHCenter);
        g_layout->addSpacing(20);
        g_layout->addWidget(g_button,0,Qt::AlignHCenter);
        g_layout->addSpacing(30);

        server_one=new DWidget(this);
        server_one->setLayout(g_layout);
        g_help->addWidget(server_one);
    }
}

//----------------帮助别人　界面３－－－－－－－－//
void AssistAssistant::give_one()
{
    give_stop();
}

void AssistAssistant::give_stop()
{
    DSpinner *dsp1 = new DSpinner(this);
    dsp1->setFixedSize(50,50);
    dsp1->start();

    DLabel *label1 = new DLabel(this);
    label1->setAlignment(Qt::AlignHCenter);
    QString str = QObject::tr("正在建立连接，请稍后……");
    QFont ft;
    ft.setPointSize(11);
    label1->setFont(ft);
    DPalette pe;
    pe.setColor(DPalette::WindowText, Qt::black);
    label1->setPalette(pe);
    label1->setText(str);

    DPushButton *button = new DPushButton(this);
    button->setText(QObject::tr("取消"));
    connect(button,&DPushButton::clicked, [=](){
        g_button->setText(QObject::tr("取消"));
        g_edit->clear();
        tGo->stop();
        stack->setCurrentWidget(wid);
    });

    QVBoxLayout *o_layout = new QVBoxLayout(this);
    o_layout->addSpacing(20);
    o_layout->addWidget(dsp1,0,Qt::AlignHCenter);
    o_layout->addSpacing(20);
    o_layout->addWidget(label1,0,Qt::AlignHCenter);
    o_layout->addSpacing(10);
    o_layout->addWidget(button,0,Qt::AlignHCenter);
    o_layout->addSpacing(30);

    server_two=new DWidget(this);
    server_two->setLayout(o_layout);

    g_help->addWidget(server_two);
    g_help->setCurrentWidget(server_two);
    tGo->start(5000);
}

//----------------延时　判断验证码　－－－－－－－－//
void AssistAssistant::give_link()
{


    qDebug()<<"1111"<<str_num1;

}

//----------------帮助别人　连接失败－－－－－－－－//
void AssistAssistant::connection_failed()
{
    DLabel *label1 = new DLabel(this);
    label1->setAlignment(Qt::AlignHCenter);
    QString str1 = QObject::tr("连接失败");
    QFont ft1;
    ft1.setPointSize(30);
    label1->setFont(ft1);
    DPalette pe1;
    pe1.setColor(DPalette::WindowText, Qt::gray);
    label1->setPalette(pe1);
    label1->setText(str1);

    DLabel *f_label = new DLabel(this);
    f_label->setAlignment(Qt::AlignHCenter);
    QString f_str = QObject::tr("网络连接状态不是很稳定，点击重试按钮再次尝试连接。");
    QFont f_ft;
    f_ft.setPointSize(10);
    f_label->setFont(f_ft);
    DPalette f_pe;
    f_pe.setColor(DPalette::WindowText, Qt::gray);
    f_label->setPalette(f_pe);
    f_label->setText(f_str);

    DPushButton *button = new DPushButton(this);
    button->setText(QObject::tr("重试"));
    connect(button,&DPushButton::clicked,
            [=](){
        g_help->setCurrentWidget(server_one);
    }
            );

    QVBoxLayout *n_layout = new QVBoxLayout(this);
    n_layout->addSpacing(20);
    n_layout->addWidget(label1,0,Qt::AlignHCenter);
    n_layout->addSpacing(20);
    n_layout->addWidget(f_label,0,Qt::AlignHCenter);
    n_layout->addSpacing(10);
    n_layout->addWidget(button,0,Qt::AlignHCenter);
    n_layout->addSpacing(30);

    server_three=new DWidget(this);
    server_three->setLayout(n_layout);

    g_help->addWidget(server_three);
    g_help->setCurrentWidget(server_three);
    setFixedSize(360,320);
}

//----------------帮助别人　连接成功－－－－－－－－//
void AssistAssistant::connection_succeeded()
{
    DLabel *c_label = new DLabel(this);
    c_label->setAlignment(Qt::AlignHCenter);
    QString c_str = QObject::tr("您正在进行远程协助");
    QFont c_ft;
    c_ft.setPointSize(20);
    c_label->setFont(c_ft);
    DPalette c_pe;
    c_pe.setColor(DPalette::WindowText, Qt::black);
    c_label->setPalette(c_pe);
    c_label->setText(c_str);

    DLabel *f_label = new DLabel(this);
    f_label->setAlignment(Qt::AlignHCenter);
    QString f_str = QObject::tr("当前已经有一个连接会话，\n如果您想建立新的连接，请先断开");
    QFont f_ft;
    f_ft.setPointSize(8);
    f_label->setFont(f_ft);
    DPalette f_pe;
    f_pe.setColor(DPalette::WindowText, Qt::gray);
    f_label->setPalette(f_pe);
    f_label->setText(f_str);

    DPushButton *button = new DPushButton(this);
    button->setText(QObject::tr("断开"));
    connect(button,&DPushButton::clicked,
            [=](){
        stack->setCurrentWidget(wid);
    }
            );

    QVBoxLayout *n_layout = new QVBoxLayout(this);
    n_layout->addSpacing(20);
    n_layout->addWidget(c_label,0,Qt::AlignHCenter);
    n_layout->addSpacing(20);
    n_layout->addWidget(f_label,0,Qt::AlignHCenter);
    n_layout->addSpacing(10);
    n_layout->addWidget(button,0,Qt::AlignHCenter);
    n_layout->addSpacing(30);

    server_four=new DWidget(this);
    server_four->setLayout(n_layout);

    g_help->addWidget(server_four);
    g_help->setCurrentWidget(server_four);
    setFixedSize(360,320);
}


//----------------我要帮助　槽函数－－－－－－－－－－-//
void AssistAssistant::HelpSlot()
{
    //head_tit(nullptr,"123345555");
    if(!n_help)
    {
        n_help = new DStackedWidget(this);
        stack->addWidget(n_help);
        need_help();
    }
    stack->setCurrentWidget(n_help);
    n_help->setCurrentWidget(help_one);

    tSeek->start(2000);
}

//----------------帮助别人　槽函数－－－－－－－－－－-//
void AssistAssistant::GiveSlot()
{
    if(!g_help)
    {
        g_help = new DStackedWidget(this);
        stack->addWidget(g_help);
    }
    stack->setCurrentWidget(g_help);
    g_help->setCurrentWidget(server_one);
    give_help();
}
