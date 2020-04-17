#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cd_dbm.h"

#define TMP_STRING_LEN   125

class QAction;
class QLabel;
class QLineEdit;
class FindDialog;
class QPushButton;
class AddCdDialog;
class AddCdTrack;
class QTreeWidget;
class QTreeWidgetItem;
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QStringList;
// class QList<QTreeWidgetItem *>;
class QTreeWidgetItem;


class CdDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    CdDatabaseWidget();

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    
protected:
    // void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);//继承自QWidget的虚函数，在子类中重新实现此函数即可用于处理上下文菜单事件

public slots:
    void catalogClickedOperation();//(QTreeWidgetItem *, int);

private slots:
    void AddCD();
    void about();
    void goThroughDatabase();
    void enableSearchButton(const QString &text);
    void search_slot();

    // void openRecentFile();
    // void updateStatusBar();
    
    void addCdInfoToTree();
    void changeCdCatalogItem();
    void changeCdTrackItem();
    
    void AddTracktoCd();
    void addTrackItem();

    void changeCdItemInfo_slot();
    void deleteCdItem();

private:
    void createActions();
    void createMenus();
    // void createContextMenu();
    void createToolBars();
    // void createStatusBar();

    void showCdTree();
    void updateSearchedCatalogToTree(cd_catalog_entry *catalog_item_found, int found_num);
    void convertQstringtochar(char * dest, QString *srcDataa);

    /*Spreadsheet *spreadsheet;*/
    QWidget *mainWidget;
    QTreeWidget *treeWidget;

    FindDialog *findDialog;
    AddCdDialog *addCdDialog;
    AddCdDialog *changeCdCatalogDialog;
    AddCdTrack *addCdTrackDialog;
    AddCdTrack *changeCdTrackDialog;

    QLabel *label_search;
    QLineEdit *lineEdit_Search;
    QStringList recentFiles;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu * contextMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QPushButton *searchButton;

    QAction *addCd_Action;
    QAction *quit_Action;
    QAction *deleteAction;
    QAction *update_Action;
    QAction *aboutAction;
    QAction *aboutQtAction;


    QStringList cdWindowLabelList;

    QHBoxLayout *topHorizontalLayout;
    QGridLayout *cdTreeLayout;
    QVBoxLayout *mainLayout;

    QList<QTreeWidgetItem *> catalog_WidgetItem_List;
    // QList<QTreeWidgetItem *> cd_Item_List1;
    QTreeWidgetItem *catalog_WidgetItem;

    QTreeWidgetItem *track_WidgetItem;

    QTreeWidgetItem *current_Item;

    /*用于保存从对话框中获取到的CD目录信息*/
    QString text_Catalog_entry;
    QString text_Title;
    QString text_Type;
    QString text_Artist;

    int text_track_num;
    QString text_track_name;

    cd_catalog_entry cdCatalogEntry;
    cd_track_entry   cdTrackEntry;
};

#endif
