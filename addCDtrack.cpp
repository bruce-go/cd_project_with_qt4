#include <QtGui>

#include "addCDtrack.h"

AddCdTrack::AddCdTrack(QWidget *parent) : QDialog(parent)
{
    label_Track_Name = new QLabel(tr("Track name"));
    lineEdit_Track_Name = new QLineEdit;
    label_Track_Name->setBuddy(lineEdit_Track_Name);

    addButton = new QPushButton(tr("&Add"));
    addButton->setDefault(true);
    addButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit_Track_Name, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableAddButton(const QString &)));
    connect(addButton, SIGNAL(clicked()),this, SLOT(addButtonClicked()));

    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
    topLeftLayout_2->addWidget(label_Track_Name);
    topLeftLayout_2->addWidget(lineEdit_Track_Name);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    // leftLayout->addLayout(topLeftLayout_1);
    leftLayout->addLayout(topLeftLayout_2);
    // leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Add track"));
    setFixedHeight(sizeHint().height());

}

AddCdTrack::AddCdTrack(QWidget *parent, QString track_name) : QDialog(parent)
{
    label_Track_Name = new QLabel(tr("Track name"));
    lineEdit_Track_Name = new QLineEdit;
    lineEdit_Track_Name->setText(track_name);
    label_Track_Name->setBuddy(lineEdit_Track_Name);

    okButton = new QPushButton(tr("&ok"));
    okButton->setDefault(true);
    okButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit_Track_Name, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton(const QString &)));
    connect(okButton, SIGNAL(clicked()),this, SLOT(okButtonClicked()));

    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *topLeftLayout_2 = new QHBoxLayout;
    topLeftLayout_2->addWidget(label_Track_Name);
    topLeftLayout_2->addWidget(lineEdit_Track_Name);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    // leftLayout->addLayout(topLeftLayout_1);
    leftLayout->addLayout(topLeftLayout_2);
    // leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(okButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Change track name"));
    setFixedHeight(sizeHint().height());
}


QString AddCdTrack::getTrackName()
{
    return text_track_name;
}

/*QString AddCdTrack::getTrackNum()
{
    return text_track_num;
}*/

void AddCdTrack::addButtonClicked()
{
    //先保存对话框的输入内容
    text_track_name = lineEdit_Track_Name->text();
    //然后清除掉，以免多次按下add会导致重复添加相同的条目
    lineEdit_Track_Name->clear();
    //在这里再次发送信号，自定义一个
    emit _add_clicked_signal();  
}

void AddCdTrack::okButtonClicked()
{
    //先保存对话框的输入内容
    text_track_name = lineEdit_Track_Name->text();
    //然后清除掉，以免多次按下add会导致重复添加相同的条目
    lineEdit_Track_Name->clear();
    //在这里再次发送信号，自定义一个
    emit _ok_clicked_signal();
}

void AddCdTrack::enableAddButton(const QString &text)
{
    addButton->setEnabled(!text.isEmpty());
}

void AddCdTrack::enableOkButton(const QString &text)
{
    okButton->setEnabled(!text.isEmpty());
}