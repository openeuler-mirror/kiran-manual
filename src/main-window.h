#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switchArticlePage(QString key); // TODO: key -> JSON Object
    void switchHomePage(QString key);
private:
    Ui::MainWindow *ui;
    void initView();
    void initEvent();
};
#endif  // MAINWINDOW_H
