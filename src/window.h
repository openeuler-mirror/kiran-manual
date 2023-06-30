#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Window;
}
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void switchArticlePage(QString key); // TODO: key -> JSON Object
    void switchHomePage(QString key);
private:
    Ui::Window *ui;
    void initView();
    void initEvent();
};
#endif  // MAINWINDOW_H
