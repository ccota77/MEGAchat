#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void buttonPushed();
    void megaMessageSlot(void* msg);
private:
    Ui::MainWindow *ui;
};

class AppDelegate: public QObject
{
    Q_OBJECT
public slots:
    void onAppTerminate();
};

#endif // MAINWINDOW_H
