#include "dequalizerdialog.h"

#include <QPainter>
#include <QProxyStyle>
#include <DLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <DSlider>
#include <DBlurEffectWidget>
#include <DTabWidget>
#include <DSwitchButton>
#include <DComboBox>
#include <DTitlebar>
#include <DFrame>
#include <DPushButton>
#include <QDebug>

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 144; // 设置每个tabBar中item的大小
            s.rheight() = 30;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(tab->rect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

DWIDGET_USE_NAMESPACE
class DequalizerDialogPrivate
{
public:
    DequalizerDialogPrivate(DequalizerDialog *parent) : q_ptr(parent) {}

    void initUI();
    void initConnection();
    void setCurrentEffect(QString currentText);

    DTitlebar   *mtitlebar            = nullptr;
    DLabel      *mtitleLabel          = nullptr;
    DLabel      *mswitchLabel         = nullptr;
    DSwitchButton *mswitchBtn         = nullptr;
    DComboBox      *mcombox           = nullptr;
    DPushButton    *saveBtn           = nullptr;
    QStringList    effect_type        = {"自定义","单调","古典","俱乐部","舞曲","全低音",
                                         "全低音和高音","全高音","耳机","大厅","实况","聚会",
                                         "流行","雷盖","摇滚","斯卡","柔和","慢摇","电子乐"};
    bool          switch_flag         = false;
    QMap <DSlider*,int> Slider_map;

    DSlider     *slider_preamplifier  = nullptr;
    DSlider     *slider_60            = nullptr;
    DSlider     *slider_170           = nullptr;
    DSlider     *slider_310           = nullptr;
    DSlider     *slider_600           = nullptr;
    DSlider     *slider_1k            = nullptr;
    DSlider     *slider_3k            = nullptr;
    DSlider     *slider_6k            = nullptr;
    DSlider     *slider_12k           = nullptr;
    DSlider     *slider_14k           = nullptr;
    DSlider     *slider_16k           = nullptr;

    MusicSettings     *settings       = nullptr;

    //slidergroup
    DBlurEffectWidget *slWidget  = nullptr;
    DequalizerDialog *q_ptr;

    Q_DECLARE_PUBLIC(DequalizerDialog)
};

void DequalizerDialogPrivate::initUI()
{
    Q_Q(DequalizerDialog);

    q->resize(720,463);

    QFont font;
    font.setFamily("SourceHanSansSC");
    font.setWeight(QFont::Normal);
    font.setPixelSize(17);
    mtitleLabel  = new DLabel("均衡器");
    mtitleLabel->resize(51,25);
    mtitleLabel->setFont(font);

    mswitchLabel = new DLabel("关");
    mswitchLabel->resize(14,20);

    mswitchBtn = new DSwitchButton(q);
    mswitchBtn->resize(50,20);
//    //设置底色
//    DPalette sw;
//    sw.setColor(DPalette::Button,QColor("#000000"));
//    mswitchBtn->setPalette(sw);

    mcombox = new DComboBox(q);
    mcombox->setMinimumSize(240,36);
    mcombox->addItems(effect_type);


    saveBtn = new DPushButton(q);
    saveBtn->setMinimumSize(110,36);
    saveBtn->setText("保存");

    slider_preamplifier = new DSlider(Qt::Vertical);
    auto lb_preamplifier = new DLabel("前置放大");

//    Slider_map.insert(slider_60,"60");
    slider_60 = new DSlider(Qt::Vertical);
    auto lb1     = new DLabel("60");

    slider_170 = new DSlider(Qt::Vertical);
    auto lb2     = new DLabel("170");

    slider_310 = new DSlider(Qt::Vertical);
    auto lb3     = new DLabel("310");

    slider_600 = new DSlider(Qt::Vertical);
    auto lb4     = new DLabel("600");

    slider_1k = new DSlider(Qt::Vertical);
    auto lb5     = new DLabel("1K");

    slider_3k = new DSlider(Qt::Vertical);
    auto lb6     = new DLabel("3K");

    slider_6k = new DSlider(Qt::Vertical);
    auto lb7    = new DLabel("6K");

    slider_12k = new DSlider(Qt::Vertical);
    auto lb8     = new DLabel("12K");

    slider_14k = new DSlider(Qt::Vertical);
    auto lb9     = new DLabel("14K");

    slider_16k = new DSlider(Qt::Vertical);
    auto lb10     = new DLabel("16K");

    auto vlayout0  = new QVBoxLayout;
    auto vlayout1  = new QVBoxLayout;
    auto vlayout2  = new QVBoxLayout;
    auto vlayout3  = new QVBoxLayout;
    auto vlayout4  = new QVBoxLayout;
    auto vlayout5  = new QVBoxLayout;
    auto vlayout6  = new QVBoxLayout;
    auto vlayout7  = new QVBoxLayout;
    auto vlayout8  = new QVBoxLayout;
    auto vlayout9  = new QVBoxLayout;
    auto vlayout10 = new QVBoxLayout;

    vlayout0->addWidget(slider_preamplifier,Qt::AlignCenter);
    vlayout0->addWidget(lb_preamplifier);

    vlayout1->addWidget(slider_60);
    vlayout1->addWidget(lb1);

    vlayout2->addWidget(slider_170);
    vlayout2->addWidget(lb2);

    vlayout3->addWidget(slider_310);
    vlayout3->addWidget(lb3);

    vlayout4->addWidget(slider_600);
    vlayout4->addWidget(lb4);

    vlayout5->addWidget(slider_1k);
    vlayout5->addWidget(lb5);

    vlayout6->addWidget(slider_3k);
    vlayout6->addWidget(lb6);

    vlayout7->addWidget(slider_6k);
    vlayout7->addWidget(lb7);

    vlayout8->addWidget(slider_12k);
    vlayout8->addWidget(lb8);

    vlayout9->addWidget(slider_14k);
    vlayout9->addWidget(lb9);

    vlayout10->addWidget(slider_16k);
    vlayout10->addWidget(lb10);

    auto hlayout  = new QHBoxLayout;
    hlayout->addLayout(vlayout0);
    hlayout->addLayout(vlayout1);
    hlayout->addLayout(vlayout2);
    hlayout->addLayout(vlayout3);
    hlayout->addLayout(vlayout4);
    hlayout->addLayout(vlayout5);
    hlayout->addLayout(vlayout6);
    hlayout->addLayout(vlayout7);
    hlayout->addLayout(vlayout8);
    hlayout->addLayout(vlayout9);
    hlayout->addLayout(vlayout10);
    hlayout->setSpacing(5);

    slWidget = new DBlurEffectWidget;
    slWidget->setBlurRectXRadius(8);
    slWidget->setBlurRectYRadius(8);
    slWidget->setRadius(8);
    slWidget->setBlurEnabled(true);
    slWidget->setMode(DBlurEffectWidget::GaussianBlur);
    slWidget->setMinimumWidth(517);
    slWidget->setMinimumHeight(264);
    slWidget->setLayout(hlayout);
    slWidget->setContentsMargins(10,10,9,20);

    auto vlay  = new QVBoxLayout;
    //第一行
    vlay->addWidget(mtitleLabel);
    //第二行
    auto hlay  = new QHBoxLayout;
    hlay->addWidget(mswitchLabel,Qt::AlignLeft);
    hlay->addWidget(mswitchBtn,Qt::AlignLeft);
    hlay->addWidget(mcombox,20,Qt::AlignCenter);
    hlay->addWidget(saveBtn,Qt::AlignRight);

    //第三行
    vlay->addLayout(hlay);
    vlay->addStretch(2);
    vlay->addWidget(slWidget);
    vlay->setContentsMargins(20,10,20,20);

    //均衡器页面
    auto mequalizer = new DWidget(q);
    mequalizer->resize(537,393);
    mequalizer->setLayout(vlay);

    auto mtabwidget = new DTabWidget;
    mtabwidget->setTabPosition(QTabWidget::West);
    mtabwidget->setAutoFillBackground(true);
    mtabwidget->setDocumentMode(true);
    mtabwidget->resize(720,463);
    mtabwidget->addTab(mequalizer,"均衡器");
    mtabwidget->tabBar()->setStyle(new CustomTabStyle);
    mtabwidget->tabBar()->setContentsMargins(10,0,10,0);

    auto mTitlebar = new DTitlebar(q);
    mTitlebar->setTitle("");
    mTitlebar->resize(720,50);

    //垂直布局TabWidget和TitleBar
    auto  mlayout = new QVBoxLayout;
    mlayout->addWidget(mTitlebar);
    mlayout->addWidget(mtabwidget);
    mlayout->setSpacing(0);
    mlayout->setMargin(0);
    q->setLayout(mlayout);


    //开机后默认参数设置
    switch_flag = settings->value("equalizer.all.switch").toBool();
    mswitchBtn->setChecked(switch_flag);
    mcombox->setEnabled(switch_flag);
    mcombox->setCurrentText(settings->value("equalizer.all.combox").toString());
    saveBtn->setEnabled(false);

    slider_preamplifier->setValue(0);
    slider_preamplifier->setMinimum(-20);
    slider_preamplifier->setMaximum(20);
    slider_preamplifier->setPageStep(1);
    slider_preamplifier->setEnabled(false);

    slider_60->setValue(0);
    slider_60->setMinimum(-20);
    slider_60->setMaximum(20);
    slider_60->setPageStep(1);
    slider_60->setEnabled(false);

    slider_170->setValue(0);
    slider_170->setMinimum(-20);
    slider_170->setMaximum(20);
    slider_170->setPageStep(1);
    slider_170->setEnabled(false);

    slider_310->setValue(0);
    slider_310->setMinimum(-20);
    slider_310->setMaximum(20);
    slider_310->setPageStep(1);
    slider_310->setEnabled(false);

    slider_600->setValue(0);
    slider_600->setMinimum(-20);
    slider_600->setMaximum(20);
    slider_600->setPageStep(1);
    slider_600->setEnabled(false);

    slider_1k->setValue(0);
    slider_1k->setMinimum(-20);
    slider_1k->setMaximum(20);
    slider_1k->setPageStep(1);
    slider_1k->setEnabled(false);

    slider_3k->setValue(0);
    slider_3k->setMinimum(-20);
    slider_3k->setMaximum(20);
    slider_3k->setPageStep(1);
    slider_3k->setEnabled(false);

    slider_6k->setValue(0);
    slider_6k->setMinimum(-20);
    slider_6k->setMaximum(20);
    slider_6k->setPageStep(1);
    slider_6k->setEnabled(false);

    slider_12k->setValue(0);
    slider_12k->setMinimum(-20);
    slider_12k->setMaximum(20);
    slider_12k->setPageStep(1);
    slider_12k->setEnabled(false);

    slider_14k->setValue(0);
    slider_14k->setMinimum(-20);
    slider_14k->setMaximum(20);
    slider_14k->setPageStep(1);
    slider_14k->setEnabled(false);

    slider_16k->setValue(0);
    slider_16k->setMinimum(-20);
    slider_16k->setMaximum(20);
    slider_16k->setPageStep(1);
    slider_16k->setEnabled(false);


}

void DequalizerDialogPrivate::initConnection()
{
    Q_Q(DequalizerDialog);
    q->connect(mswitchBtn, &DSwitchButton::checkedChanged,
    q, [ = ](bool flag) {
        mcombox->setEnabled(flag);
        slider_preamplifier->setEnabled(flag);
        slider_60->setEnabled(flag);
        slider_170->setEnabled(flag);
        slider_310->setEnabled(flag);
        slider_600->setEnabled(flag);
        slider_1k->setEnabled(flag);
        slider_3k->setEnabled(flag);
        slider_6k->setEnabled(flag);
        slider_12k->setEnabled(flag);
        slider_14k->setEnabled(flag);
        slider_16k->setEnabled(flag);
            qDebug()<<"均衡器已启用:"<<flag;
            settings->setOption("equalizer.all.switch",flag);
    });

    q->connect(mcombox, &DComboBox::currentTextChanged,
    q, [ = ](const QString &currentText) {
            setCurrentEffect(currentText);
            saveBtn->setEnabled(false);
            settings->setOption("equalizer.all.combox",currentText);
    });

    q->connect(saveBtn, &DPushButton::clicked,
    q, [ = ]() {
            qDebug()<<"保存";
    });

    q->connect(slider_60, &DSlider::sliderPressed,
    q, [ = ]() {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_170, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_310, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_600, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_1k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_3k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_6k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_12k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_14k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
    q->connect(slider_16k, &DSlider::sliderPressed,
    q, [ = ]( ) {
            mcombox->setCurrentText("自定义");
            saveBtn->setEnabled(true);
    });
}
//设置模式
 void DequalizerDialogPrivate::setCurrentEffect(QString currentText)
 {




 }

DequalizerDialog::DequalizerDialog(QWidget *parent):
    DAbstractDialog(parent), d_ptr(new DequalizerDialogPrivate(this))
{
    Q_D(DequalizerDialog);
    d->initUI();
    d->initConnection();
}

DequalizerDialog::~DequalizerDialog()
{
//    Q_D(DequalizerDialog);
}
