#ifndef ADDCDDIALOG_H
#define ADDCDDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class AddCdDialog : public QDialog
{
    Q_OBJECT

public:
    AddCdDialog(QWidget *parent);
    AddCdDialog(QWidget *parent, QString catalog_entry, QString catalog_title, QString catalog_artist, QString catalog_type);
    /*提供公有函数来返回获取到的CD目录信息*/
    QString getCatalogEntry();
    QString getCdTitle();
    QString getCdType();
    QString getCdArtist();
    QPushButton *getAddButton();

signals:
    void _add_clicked_signal();
    void _ok_clicked_signal();

private slots:
    void addButtonClicked();
    void okButtonClicked();
    void enableAddButton(const QString &text);
    void enableOkButton(const QString &text);

private:
    QLabel *label_Catalog_entry;
    QLabel *label_Title;
    QLabel *label_Type;
    QLabel *label_Artist;

    QLineEdit *lineEdit_Catalog_entry;
    QLineEdit *lineEdit_Title;
    QLineEdit *lineEdit_Type;
    QLineEdit *lineEdit_Artist;

    // QCheckBox *caseCheckBox;
    // QCheckBox *backwardCheckBox;
    QPushButton *addButton;
    QPushButton *okButton;
    QPushButton *closeButton;

    
    QString text_Catalog_entry;
    QString text_lineEdit_Title;
    QString text_lineEdit_Type;
    QString text_lineEdit_Artist;
    
};

#endif
