#ifndef ADDCDTRACK_H
#define ADDCDTRACK_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class AddCdTrack : public QDialog
{
    Q_OBJECT

public:
    AddCdTrack(QWidget *parent);
    AddCdTrack(QWidget *parent, QString track_name);
    /*提供公有函数来返回获取到的CD目录信息*/
    QString getTrackName();
   
signals:
    void _add_clicked_signal();
    void _ok_clicked_signal();

private slots:
    void addButtonClicked();
    void okButtonClicked();
    void enableAddButton(const QString &text);
    void enableOkButton(const QString &text);

private:
    QLabel *label_Track_Name; 
    
    QLineEdit *lineEdit_Track_Name;

    // QCheckBox *caseCheckBox;
    // QCheckBox *backwardCheckBox;
    QPushButton *addButton;
    QPushButton *okButton;
    QPushButton *closeButton;

    
    QString text_track_name;
    // QString text_track_num;
    /*QString text_lineEdit_Title;
    QString text_lineEdit_Type;
    QString text_lineEdit_Artist;*/
    
};

#endif
