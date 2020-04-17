#include <QtGui>
#include <iostream>

#include <string>

// #include "finddialog.h"
#include "addCDdialog.h"
#include "mainwindow.h"
#include "addCDtrack.h"

extern QApplication *app;

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    this->resize(800,600);

    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainWidget->setMouseTracking(true);

    label_search = new QLabel(tr("Search Text:"));
    lineEdit_Search = new QLineEdit;
    label_search->setBuddy(lineEdit_Search);

    searchButton = new QPushButton(tr("&Search"));
    searchButton->setDefault(true);
    searchButton->setEnabled(false);
    connect(lineEdit_Search, SIGNAL(textChanged(const QString &)), this, SLOT(enableSearchButton(const QString &)));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(search_slot()));

    topHorizontalLayout = new QHBoxLayout();
    topHorizontalLayout->addWidget(label_search);
    topHorizontalLayout->addWidget(lineEdit_Search);
    topHorizontalLayout->addWidget(searchButton);

    showCdTree();

    // createStatusBar();
    findDialog = 0;

    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle(tr("CD Database"));
}

void MainWindow::showCdTree()
{
    cdWindowLabelList << tr("Tag") << tr("Title") << tr("Artist") << tr("Type");

    cdTreeLayout = new QGridLayout;

    //添加树形列表
    treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(4);
    treeWidget->setSortingEnabled(true);
    treeWidget->setHeaderLabels(cdWindowLabelList);
    treeWidget->setGeometry(0,0, this->width(), this->height());
    treeWidget->setColumnWidth(0, 100);
    treeWidget->setColumnWidth(1, 200);
    treeWidget->setColumnWidth(2, 150);

    goThroughDatabase();

    treeWidget->insertTopLevelItems(0, catalog_WidgetItem_List);

    // connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(catalogClickedOperation(QTreeWidgetItem *, int)));

    cdTreeLayout->addWidget(treeWidget, 0, 0);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topHorizontalLayout);
    mainLayout->addLayout(cdTreeLayout);

    mainWidget->setLayout(mainLayout);
}

void MainWindow::goThroughDatabase()
{
    treeWidget->clear();
    /*在启动过程中可以读取数据库，如果数据库非空则将已保存的CD信息显示出来*/
    //是否有函数可以直接遍历DBM数据库？怎样判断dbm数据库为空？
    // 两层循环，第一层遍历目录，第二层遍历目录下的track
    cd_catalog_entry catalog_Item_In_DBM;
    cd_track_entry   existing_track_In_DBM;
    
    //dbm数据库的第一个键应该是指向最后一个加入的对象，或者数据库初始化时有很多key_datum.dptr == NULL的对象
    // dbm_nextkey()遍历到的键值包含的指针为空后，继续往后获取的键值包含的指针也会依然为空
    for(catalog_Item_In_DBM = cd_catalog_DBM_FirstItem();catalog_Item_In_DBM.catalog_entry[0] != '\0';catalog_Item_In_DBM = cd_catalog_DBM_NextItem())
    {
        //先把目录条目的成员转换成QString，然后添加到treeWidget中
        text_Catalog_entry = QString(QLatin1String(catalog_Item_In_DBM.catalog_entry));
        text_Title         = QString(QLatin1String(catalog_Item_In_DBM.title));
        text_Type          = QString(QLatin1String(catalog_Item_In_DBM.type));
        text_Artist        = QString(QLatin1String(catalog_Item_In_DBM.artist));

        catalog_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< text_Catalog_entry
                            << text_Title << text_Artist << text_Type);
        //设置QTreeWidgetItem的各列成员的颜色，字体等
         QBrush brush_catalog = QBrush(QColor(199, 237, 225));
         catalog_WidgetItem->setBackground(0, brush_catalog);
         catalog_WidgetItem->setBackground(1, brush_catalog);
         catalog_WidgetItem->setBackground(2, brush_catalog);
         catalog_WidgetItem->setBackground(3, brush_catalog);

        //再遍历目录下的所有track，并添加到catalog_WidgetItem中
        int track_no = 1;
        while(1)
        {
            // std::cout << "run to catalog " << catalog_Item_In_DBM.catalog_entry[0] << " item "  << track_no << std::endl;
            existing_track_In_DBM = get_cd_track_entry((const char *)(&catalog_Item_In_DBM.catalog_entry), track_no);

            if(existing_track_In_DBM.catalog_entry[0] != '\0')//如果条目号存在且非0
            {

                track_WidgetItem = new QTreeWidgetItem(catalog_WidgetItem, QStringList() << tr("Track")+QString::number(existing_track_In_DBM.track_no) 
                    << QString(QLatin1String(existing_track_In_DBM.track_txt)));

            }
            else
            {
                // std::cout << "catalog " << catalog_Item_In_DBM.catalog_entry[0] << " item " 
                //             << track_no  << " didn't not exist" << std::endl;
                // std:cout << std::endl;
                break;
            }

            track_no++;
        }
        
        treeWidget->expandItem(catalog_WidgetItem);//把这个目录下的子项设为展开
        catalog_WidgetItem_List << catalog_WidgetItem;
    }
}


void MainWindow::addCdInfoToTree()
{
    text_Catalog_entry = addCdDialog->getCatalogEntry();
    text_Title         = addCdDialog->getCdTitle();
    text_Type          = addCdDialog->getCdType();
    text_Artist        = addCdDialog->getCdArtist();

    catalog_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< text_Catalog_entry
    << text_Title << text_Artist << text_Type);

    //设置QTreeWidgetItem的各列成员的颜色，字体等
    QBrush brush_catalog = QBrush(QColor(199, 237, 225));//199, 237, 204
    catalog_WidgetItem->setBackground(0, brush_catalog);
    catalog_WidgetItem->setBackground(1, brush_catalog);
    catalog_WidgetItem->setBackground(2, brush_catalog);
    catalog_WidgetItem->setBackground(3, brush_catalog);

    catalog_WidgetItem_List << catalog_WidgetItem;

    // connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(catalogClickedOperation(QTreeWidgetItem *, int)));

    treeWidget->insertTopLevelItems(0, catalog_WidgetItem_List);
   
    /*把目录信息保存到数据库中*/
    convertQstringtochar(cdCatalogEntry.catalog_entry, &text_Catalog_entry);
    convertQstringtochar(cdCatalogEntry.title, &text_Title);
    convertQstringtochar(cdCatalogEntry.type, &text_Type);
    convertQstringtochar(cdCatalogEntry.artist, &text_Artist);

    // std::cerr << "run to 4!" << std::endl;
    if(!add_cd_catalog_entry(cdCatalogEntry))
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }
    
}


void MainWindow::changeCdCatalogItem()
{
    text_Catalog_entry = addCdDialog->getCatalogEntry();
    text_Title         = addCdDialog->getCdTitle();
    text_Artist        = addCdDialog->getCdArtist();
    text_Type          = addCdDialog->getCdType();

    current_Item = treeWidget->currentItem();

    current_Item->setText(1, text_Title);
    current_Item->setText(2, text_Artist);
    current_Item->setText(3, text_Type);

     memset(&cdCatalogEntry, '\0', sizeof(cdCatalogEntry));
    /*把目录信息保存到数据库中*/
    convertQstringtochar(cdCatalogEntry.catalog_entry, &text_Catalog_entry);
    convertQstringtochar(cdCatalogEntry.title, &text_Title);
    convertQstringtochar(cdCatalogEntry.type, &text_Type);
    convertQstringtochar(cdCatalogEntry.artist, &text_Artist);

    // std::cerr << "run to 4!" << std::endl;
    if(!add_cd_catalog_entry(cdCatalogEntry))
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }
    
}


void MainWindow::convertQstringtochar(char * dest, QString *srcData)
{
    //srcData->toStdString()返回的是一个string对象，并存放在栈中，不能直接把该对象转换成char *的指针赋值给别的指针变量
    strcpy(dest, srcData->toStdString().c_str());
    // dest = (char *)(srcData->toStdString().c_str());
    //或者先将QString用toStdString()转为标准库中的string类型，然后将string用c_str()转为char *
}

void MainWindow::catalogClickedOperation()//(QTreeWidgetItem *, int)
{
    current_Item = treeWidget->currentItem();

    if(current_Item == NULL)  //如果没有选中树上的任何项目，则直接返回
    {
        return;
    }

// 此种方式不可行，也许需要重写类QTreeWidget
    //判断鼠标位置是否在item的范围内
    /*QPoint point_topleft, point_bottomright, current_pos, treeWidget_pos;
    QRect rect_current_Item;
    //获取treeWidget的全局坐标
    treeWidget_pos = treeWidget->mapToGlobal(QPoint(0,0));
    //获取当前项目相对treeWidget的矩形
    rect_current_Item = treeWidget->visualItemRect(current_Item);

    point_topleft = treeWidget->visualItemRect(current_Item).topLeft();
    rect_current_Item.setTopLeft(treeWidget_pos + point_topleft);

    point_bottomright = treeWidget->visualItemRect(current_Item).bottomRight();
    rect_current_Item.setBottomRight(treeWidget_pos + point_bottomright);

    current_pos = QCursor::pos();
    
    std::cout << "rectangle point_topleft.x = " << (treeWidget_pos + point_topleft).x() << " point_topleft.y = " 
    << (treeWidget_pos + point_topleft).y() << std::endl;
    std::cout << "rectangle point_bottomright.x = " << (treeWidget_pos + point_bottomright).x() << " point_bottomright.y = " 
    << (treeWidget_pos + point_bottomright).y() << std::endl;
    std::cout << "cursor.x = " << current_pos.x() << " cursor.y = " << current_pos.y() << std::endl;
    // if(!(point_topleft.x() <= current_pos.x() && current_pos.x() <= point_bottomright.x() &&
    //      point_topleft.y() <= current_pos.y() && current_pos.y() <= point_bottomright.y()))
    if(!rect_current_Item.contains(current_pos))
    {
        return;
    } */

    // std::cout << "app->mouseButtons() = " << app->mouseButtons() << std::endl;
    //判断右击有问题，需要解决
   
    QAction *addTrack_Action = new QAction(tr("Add &New track"), this);
    QAction *renameCdItem_Action = new QAction(tr("Rename"), this);
    QAction *delCdItem_Action = new QAction(tr("Delete"), this);

    connect(addTrack_Action, SIGNAL(triggered()), this, SLOT(AddTracktoCd()));
    connect(renameCdItem_Action, SIGNAL(triggered()), this, SLOT(changeCdItemInfo_slot()));//这个槽函数待实现
    connect(delCdItem_Action, SIGNAL(triggered()), this, SLOT(deleteCdItem()));//这个槽函数待实现
    
    QPoint pos;
    contextMenu = new QMenu(treeWidget);
    if(current_Item->parent() != NULL) //目录项才允许添加track信息，如果是track项则只能重命名和删除
    {
        // menu.addAction(addTrack_Action);
        contextMenu->addAction(renameCdItem_Action);
        contextMenu->addAction(delCdItem_Action);
        contextMenu->exec(QCursor::pos());  //在当前鼠标位置显示
    }
    else
    {
        contextMenu->addAction(addTrack_Action);
        contextMenu->addAction(renameCdItem_Action);
        contextMenu->addAction(delCdItem_Action);
        contextMenu->exec(QCursor::pos());  //在当前鼠标位置显示
    }
 
     // }
    //else if(qApp->mouseButtons() == Qt::)处理双击事件
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //让菜单移动到鼠标位置并显示.
    // contextMenu->exec(event->globalPos());
    catalogClickedOperation();
}

void MainWindow::AddTracktoCd()
{
    QTreeWidgetItem *current_Item = treeWidget->currentItem();

    if(current_Item == NULL)  //如果没有选中树上的任何项目，则直接返回
    {
        return;
    }

    addCdTrackDialog = new AddCdTrack(this);

    connect(addCdTrackDialog, SIGNAL(_add_clicked_signal()),this, SLOT(addTrackItem()));

    addCdTrackDialog->show();
    
    addCdTrackDialog->raise();
   
    addCdTrackDialog->activateWindow();
}

void MainWindow::addTrackItem()
{
    int childNum = 0;

    QTreeWidgetItem *current_Item = treeWidget->currentItem();

    treeWidget->expandItem(current_Item);
    // std::cout << "current catalog items' child num :  " <<  << std::endl;
    childNum = current_Item->childCount();
    //下一条语句执行以后current_Item的child数目就变成1了，后面再使用current_Item->childCount()获取到的值是1！！！！！
    track_WidgetItem = new QTreeWidgetItem(current_Item, 
        QStringList() << tr("Track")+QString::number(childNum + 1) << addCdTrackDialog->getTrackName());

    /*把音乐信息保存到数据库*/
    QString current_Item_catalog_entry;
    text_track_num = childNum + 1;//addCdTrackDialog->getTrackNum();
    text_track_name = addCdTrackDialog->getTrackName();
    current_Item_catalog_entry = current_Item->text(0);

    convertQstringtochar(cdTrackEntry.catalog_entry, &current_Item_catalog_entry);
   
    convertQstringtochar(cdTrackEntry.track_txt, &text_track_name);

    cdTrackEntry.track_no = text_track_num;//std::stoi(text_track_num.toStdString());
    // std::cout << "add track item " << text_track_num << std::endl;
    if(add_cd_track_entry(cdTrackEntry) != 1)
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }
}

void MainWindow::changeCdItemInfo_slot()
{
    QTreeWidgetItem *current_Item = treeWidget->currentItem();
    
    /*char cur_catalog_entry_num[31];
    int deleted_track_no, to_be_done_num;
    cd_track_entry temp_track_In_DBM;*/

    //修改目录项
    if(current_Item->parent() == NULL)
    {
        //修改目录信息
        changeCdCatalogDialog = new AddCdDialog(this, current_Item->text(0), current_Item->text(1),
            current_Item->text(2), current_Item->text(3));
        connect(changeCdCatalogDialog, SIGNAL(_ok_clicked_signal()),this, SLOT(changeCdCatalogItem()));
        
        changeCdCatalogDialog->show();
        changeCdCatalogDialog->raise(); 
        changeCdCatalogDialog->activateWindow(); 
    }
    else// 修改track项
    {
        // QTreeWidgetItem *current_Item_Parent = current_Item->parent();

        changeCdTrackDialog = new AddCdTrack(this, /*current_Item->text(0).section('k', 1, -1) +*/ current_Item->text(1));

        connect(changeCdTrackDialog, SIGNAL(_ok_clicked_signal()),this, SLOT(changeCdTrackItem()));

        changeCdTrackDialog->show();
        changeCdTrackDialog->raise();
        changeCdTrackDialog->activateWindow();
    }
}

void MainWindow::deleteCdItem()
{
    QTreeWidgetItem *current_Item = treeWidget->currentItem();
    QString qstring_Current_Item;
    char cur_catalog_entry_num[31];
    int deleted_track_no, to_be_done_num;
    cd_track_entry temp_track_In_DBM;

    //删除目录项
    if(current_Item->parent() == NULL)
    {
        if(current_Item->childCount() > 0)//有子节点
        {
            for(int i = 0; i < current_Item->childCount(); i++)
            {
                //删除各个track项
                qstring_Current_Item = current_Item->text(0);
                convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取column 0的内容

                deleted_track_no = std::stoi(current_Item->child(i)->text(0).section('k', 1, -1).toStdString());

                del_cd_track_entry(cur_catalog_entry_num, deleted_track_no);

                delete (current_Item->child(i));
            }
        }

        //删除目录
        qstring_Current_Item = current_Item->text(0);
        convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取目录项column 0的内容
        
        if(del_cd_catalog_entry(cur_catalog_entry_num) != 1)
        {
            std::cout << "Error, delete catlog failed" << std::endl;
        }
        // delete treeWidget->takeTopLevelItem(treeWidget->currentIndex().row());
        delete current_Item;
    }
    else// 删除track项
    {
        QTreeWidgetItem *current_Item_Parent = current_Item->parent();

        //从树上删除该track项
        current_Item_Parent->removeChild(current_Item); 

        //从数据库中删除该track项
        qstring_Current_Item = current_Item_Parent->text(0);
        convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取目录项column 0的内容
        deleted_track_no = std::stoi(current_Item->text(0).section('k', 1, -1).toStdString());   //只能处理个位数的track_no，扩展性需要进一步完善

        del_cd_track_entry(cur_catalog_entry_num, deleted_track_no);

        //检测并更新树的子项和数据库中的track项的track_no值
        // 遍历剩余子项，更新编号大于当前被删除子项编号的项，并写到数据库中
        int temp_track_no;
        int new_temp_track_no;
        int accumulation = 1;
        int adjust_finished_num = 0;
        to_be_done_num = current_Item_Parent->childCount() - (deleted_track_no - 1);//删除一个后目前剩余的子项目数，减去不需要更新的项目数
        while(1)//从被删除后的下一个项开始按顺序更新
        {
            
            for(int i = 0; i < current_Item_Parent->childCount(); i++)
            {
                temp_track_no = std::stoi(current_Item_Parent->child(i)->text(0).section('k', 1, -1).toStdString());
                
                if(temp_track_no == deleted_track_no + accumulation)
                {
                    new_temp_track_no = temp_track_no - 1;
                    current_Item_Parent->child(i)->setText(0, tr("Track")+QString::number(new_temp_track_no));
                    //由于键值更新了，要先在数据库中找出原来的track项，更新track_no，再删除原来的项，并利用新的键值重新存储
                    temp_track_In_DBM = get_cd_track_entry((const char *)cur_catalog_entry_num, temp_track_no);
                    temp_track_In_DBM.track_no = new_temp_track_no;
                    del_cd_track_entry(cur_catalog_entry_num, temp_track_no);

                    add_cd_track_entry(temp_track_In_DBM);

                    adjust_finished_num++;
                }
            }

            if(adjust_finished_num == to_be_done_num)
            {
                break;
            }

            accumulation++;

        }
        
        delete current_Item;
    }
}

void MainWindow::AddCD()
{
    addCdDialog = new AddCdDialog(this);
    connect(addCdDialog, SIGNAL(_add_clicked_signal()),this, SLOT(addCdInfoToTree()));
    
    addCdDialog->show();
    addCdDialog->raise(); 
    addCdDialog->activateWindow(); 
}

void MainWindow::changeCdTrackItem()
{
    QString current_Item_catalog_entry;

    QTreeWidgetItem *current_Item = treeWidget->currentItem();

    memset(&cdTrackEntry, '\0', sizeof(cdTrackEntry));
    
    /*把音乐信息更新到数据库*/
    text_track_num = std::stoi(current_Item->text(0).section('k', 1, -1).toStdString());//addCdTrackDialog->getTrackNum();
    text_track_name = changeCdTrackDialog->getTrackName();
    current_Item_catalog_entry = current_Item->parent()->text(0);

    //修改条目中的track名字
    current_Item->setText(1, text_track_name);

    convertQstringtochar(cdTrackEntry.catalog_entry, &current_Item_catalog_entry);
   
    convertQstringtochar(cdTrackEntry.track_txt, &text_track_name);

    cdTrackEntry.track_no = text_track_num;//std::stoi(text_track_num.toStdString());
    // std::cout << "add track item " << text_track_num << std::endl;
    if(add_cd_track_entry(cdTrackEntry) != 1)
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }
}

void MainWindow::search_slot()
{
    int entry_selected =0;
    char tmp_str[TMP_STRING_LEN +1];
    int any_entry_found =0;
    int first_call = 1;
    cd_catalog_entry item_found;

    QString text_search = lineEdit_Search->text();

    convertQstringtochar(tmp_str, &text_search);

    while(!entry_selected)//需要改成跟启动时遍历一样，才能查找相同名字的多个目录
    {
        item_found = search_cd_catalog_entry(tmp_str, &first_call);
        if(item_found.catalog_entry[0] != '\0')
        {
            ++any_entry_found;//统计查找到的次数

            updateSearchedCatalogToTree(&item_found, any_entry_found);
            //找到了项目，设置标记
            // entry_selected = 1;
            // std::cout << any_entry_found <<" times found " << std::endl;
        }
        else
        {
            if(any_entry_found)
            {
                std::cout << "Sorry, no more matches found" << std::endl;
            }
            else
            {
                std::cout << "Sorry, nothing found" << std::endl;
            }
            break;
        }
    }
}

void MainWindow::updateSearchedCatalogToTree(cd_catalog_entry *catalog_item_found, int found_num)
{
    cd_track_entry track_found;
    //如果找到了第一个项目，则先清除当前树，再把项目添加到树中；如果是第2次及以后，则不用清除树上的项目
    if(found_num == 1)
    {
        treeWidget->clear();
    }

    text_Catalog_entry = QString(QLatin1String(catalog_item_found->catalog_entry));
    text_Title         = QString(QLatin1String(catalog_item_found->title));
    text_Type          = QString(QLatin1String(catalog_item_found->type));
    text_Artist        = QString(QLatin1String(catalog_item_found->artist));

    catalog_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< text_Catalog_entry
                        << text_Title << text_Artist << text_Type);
    //设置QTreeWidgetItem的各列成员的颜色，字体等
     QBrush brush_catalog = QBrush(QColor(199, 237, 225));
     catalog_WidgetItem->setBackground(0, brush_catalog);
     catalog_WidgetItem->setBackground(1, brush_catalog);
     catalog_WidgetItem->setBackground(2, brush_catalog);
     catalog_WidgetItem->setBackground(3, brush_catalog);

    //再遍历目录下的所有track，并添加到catalog_WidgetItem中
    int track_no = 1;
    while(1)
    {
        // std::cout << "run to catalog " << catalog_Item_In_DBM.catalog_entry[0] << " item "  << track_no << std::endl;
        track_found = get_cd_track_entry((const char *)(&catalog_item_found->catalog_entry), track_no);

        if(track_found.catalog_entry[0] != '\0')//如果条目号存在且非0
        {

            track_WidgetItem = new QTreeWidgetItem(catalog_WidgetItem, QStringList() << tr("Track")+QString::number(track_found.track_no) 
                << QString(QLatin1String(track_found.track_txt)));

        }
        else
        {
            break;
        }

        track_no++;
    }
    
    treeWidget->expandItem(catalog_WidgetItem);//把这个目录下的子项设为展开
    catalog_WidgetItem_List << catalog_WidgetItem;
}


/*void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &,
                                            Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &,
                                           Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &,
                                                Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &,
                                               Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}*/

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spreadsheet"),
            tr("<h2>CD Database 0.1</h2>"
               "<p>Copyright &copy; 2020 Software Inc."
               "<p>CD Database is a small application that "
               "demonstrates QAction, QMainWindow, QMenuBar, "
               "QStatusBar, QToolBar, QTreeWidget, and many other "
               "Qt classes."));
}

void MainWindow::enableSearchButton(const QString &text)
{
    searchButton->setEnabled(!text.isEmpty());
}

/*void MainWindow::updateStatusBar()
{
    
}*/

void MainWindow::createActions()
{
    addCd_Action = new QAction(tr("Add &CD"), this);
    addCd_Action->setIcon(QIcon(":/images/new.png"));
    addCd_Action->setShortcut(QKeySequence::New);
    addCd_Action->setStatusTip(tr("Add a new CD to database"));
    connect(addCd_Action, SIGNAL(triggered()), this, SLOT(AddCD()));

    quit_Action = new QAction(tr("Q&uit"), this);
    quit_Action->setIcon(QIcon(":/images/close.png"));
    quit_Action->setShortcut(tr("Ctrl+Q"));
    quit_Action->setStatusTip(tr("Exit the application"));
    connect(quit_Action, SIGNAL(triggered()), this, SLOT(close()));

    update_Action = new QAction(tr("&Update..."), this);
    update_Action->setIcon(QIcon(":/images/refresh.png"));
    // update_Action->setShortcut(QKeySequence::Find);
    update_Action->setStatusTip(tr("Find a matching cell"));
    connect(update_Action, SIGNAL(triggered()), this, SLOT(goThroughDatabase()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));//qApp是一个全局指针，指向QApplication对象
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addCd_Action);
    fileMenu->addAction(update_Action);
    fileMenu->addAction(quit_Action);
    /*separatorAction = fileMenu->addSeparator();*/
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

/*void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}*/

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(addCd_Action);
    fileToolBar->addAction(update_Action);
    fileToolBar->addAction(quit_Action);
}

/*void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()),
            this, SLOT(spreadsheetModified()));

    updateStatusBar();
}*/

