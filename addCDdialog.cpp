#include <QtGui>

#include "addCDdialog.h"

class MainWindow;

AddCdDialog::AddCdDialog(QWidget *parent) : QDialog(parent)
{
	label_Catalog_entry = new QLabel(tr("Catalog entry"));
    lineEdit_Catalog_entry = new QLineEdit;
    label_Catalog_entry->setBuddy(lineEdit_Catalog_entry);

    label_Title = new QLabel(tr("Title"));
    lineEdit_Title = new QLineEdit;
    label_Title->setBuddy(lineEdit_Title);

    label_Type = new QLabel(tr("Type"));
    lineEdit_Type = new QLineEdit;
    label_Type->setBuddy(lineEdit_Type);

    label_Artist = new QLabel(tr("Artist"));
    lineEdit_Artist = new QLineEdit;
    label_Artist->setBuddy(lineEdit_Artist);

    // caseCheckBox = new QCheckBox(tr("Match &case"));
    // backwardCheckBox = new QCheckBox(tr("Search &backward"));

    addButton = new QPushButton(tr("&Add"));
    addButton->setDefault(true);
    addButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit_Catalog_entry, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableAddButton(const QString &)));

    connect(addButton, SIGNAL(clicked()),this, SLOT(addButtonClicked()));

    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
    topLeftLayout_1->addWidget(label_Catalog_entry);
    topLeftLayout_1->addWidget(lineEdit_Catalog_entry);

    QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
    topLeftLayout_2->addWidget(label_Title);
    topLeftLayout_2->addWidget(lineEdit_Title);

    QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
    topLeftLayout_3->addWidget(label_Type);
    topLeftLayout_3->addWidget(lineEdit_Type);

    QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
    topLeftLayout_4->addWidget(label_Artist);
    topLeftLayout_4->addWidget(lineEdit_Artist);


    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout_1);
    leftLayout->addLayout(topLeftLayout_2);
    leftLayout->addLayout(topLeftLayout_3);
    leftLayout->addLayout(topLeftLayout_4);
    // leftLayout->addWidget(caseCheckBox);
    // leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Add cd"));
    setFixedHeight(sizeHint().height());

}

AddCdDialog::AddCdDialog(QWidget *parent, QString catalog_entry, QString catalog_title, 
                            QString catalog_artist, QString catalog_type) : QDialog(parent)
{
    label_Catalog_entry = new QLabel(tr("Catalog entry"));
    lineEdit_Catalog_entry = new QLineEdit;
    //设置catalog_entry值为当前选定的目录项的值，并把此QLineEdit对象设为只读
    lineEdit_Catalog_entry->setText(catalog_entry);
    lineEdit_Catalog_entry->setReadOnly(true);
    label_Catalog_entry->setBuddy(lineEdit_Catalog_entry);

    label_Title = new QLabel(tr("Title"));
    lineEdit_Title = new QLineEdit;
    lineEdit_Title->setText(catalog_title);
    label_Title->setBuddy(lineEdit_Title);

    label_Artist = new QLabel(tr("Artist"));
    lineEdit_Artist = new QLineEdit;
    lineEdit_Artist->setText(catalog_artist);
    label_Artist->setBuddy(lineEdit_Artist);

    label_Type = new QLabel(tr("Type"));
    lineEdit_Type = new QLineEdit;
    lineEdit_Type->setText(catalog_type);
    label_Type->setBuddy(lineEdit_Type);

    okButton = new QPushButton(tr("&Ok"));
    okButton->setDefault(true);
    okButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit_Title, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton(const QString &)));
    connect(lineEdit_Type, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton(const QString &)));
    connect(lineEdit_Artist, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton(const QString &)));

    connect(okButton, SIGNAL(clicked()),this, SLOT(okButtonClicked()));

    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *topLeftLayout_1 = new QHBoxLayout;
    topLeftLayout_1->addWidget(label_Catalog_entry);
    topLeftLayout_1->addWidget(lineEdit_Catalog_entry);

    QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
    topLeftLayout_2->addWidget(label_Title);
    topLeftLayout_2->addWidget(lineEdit_Title);

    QHBoxLayout *topLeftLayout_3 = new QHBoxLayout;
    topLeftLayout_3->addWidget(label_Type);
    topLeftLayout_3->addWidget(lineEdit_Type);

    QHBoxLayout *topLeftLayout_4 = new QHBoxLayout;
    topLeftLayout_4->addWidget(label_Artist);
    topLeftLayout_4->addWidget(lineEdit_Artist);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout_1);
    leftLayout->addLayout(topLeftLayout_2);
    leftLayout->addLayout(topLeftLayout_3);
    leftLayout->addLayout(topLeftLayout_4);
    // leftLayout->addWidget(caseCheckBox);
    // leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(okButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Change Cd Info"));
    setFixedHeight(sizeHint().height());

}


QString AddCdDialog::getCatalogEntry()
{
    return text_Catalog_entry;
}

QString AddCdDialog::getCdTitle()
{
    return text_lineEdit_Title;
}

QString AddCdDialog::getCdType()
{
    return text_lineEdit_Type;
}

QString AddCdDialog::getCdArtist()
{
    return text_lineEdit_Artist;
}

QPushButton * AddCdDialog::getAddButton()
{
    return addButton;
}

void AddCdDialog::addButtonClicked()
{
    //先保存对话框的输入内容
    //还可以添加对输入信息长度的检测，如果超过指定长度则在报错（弹出对话框或者在状态栏用红色字体显示错误原因）
    text_Catalog_entry = lineEdit_Catalog_entry->text();
    text_lineEdit_Title = lineEdit_Title->text();
    text_lineEdit_Type = lineEdit_Type->text();
    text_lineEdit_Artist = lineEdit_Artist->text();

    //然后清除掉，以免多次按下add会导致重复添加相同的条目
    lineEdit_Catalog_entry->clear();
    lineEdit_Title->clear();
    lineEdit_Type->clear();
    lineEdit_Artist->clear();

    //在这里再次发送信号，自定义一个
    emit _add_clicked_signal(); 
}

void AddCdDialog::okButtonClicked()
{
    //先保存对话框的输入内容
    //还可以添加对输入信息长度的检测，如果超过指定长度则在报错（弹出对话框或者在状态栏用红色字体显示错误原因）
    text_Catalog_entry = lineEdit_Catalog_entry->text();
    text_lineEdit_Title = lineEdit_Title->text();
    text_lineEdit_Type = lineEdit_Type->text();
    text_lineEdit_Artist = lineEdit_Artist->text();

    //然后清除掉，以免多次按下add会导致重复添加相同的条目
    // lineEdit_Catalog_entry->clear();
    lineEdit_Title->clear();
    lineEdit_Type->clear();
    lineEdit_Artist->clear();

    //在这里再次发送信号，自定义一个
    emit _ok_clicked_signal();
}



void AddCdDialog::enableAddButton(const QString &text)
{
    addButton->setEnabled(!text.isEmpty());
}

void AddCdDialog::enableOkButton(const QString &text)
{
    okButton->setEnabled(!text.isEmpty());
}