#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_connectButton_clicked(bool checked);

    void on_sendDataButton_clicked();

    void readData();
private:
    Ui::MainWindow *ui;
    QSerialPort *_serialPort;
    bool loadPorts();
    bool availablePorts = false;
};
#endif // MAINWINDOW_H
