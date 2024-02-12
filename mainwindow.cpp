#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,_serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!loadPorts())
    {
        qDebug() << "No serial port found";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadPorts()
{
    if(!QSerialPortInfo::availablePorts().empty())
    {
        availablePorts = true;
        ui->connectButton->setCheckable(true);
        foreach (auto &port, QSerialPortInfo::availablePorts()) {
            ui->portComBox->addItem(port.portName());
        }
        return true;
    } else {
        ui->connectButton->setText("Connect device");
    }
    return false;
}

void MainWindow::on_connectButton_clicked(bool checked)
{
    if(availablePorts)
    {
        if(ui->connectButton->isChecked())
        {
            _serialPort = new QSerialPort();
            _serialPort->setPortName(ui->portComBox->currentText());
            _serialPort->setBaudRate(ui->baudRateComBox->currentText().toInt());
            _serialPort->setParity(QSerialPort::Parity::NoParity);
            _serialPort->setDataBits(QSerialPort::DataBits::Data8);
            _serialPort->setStopBits(QSerialPort::StopBits::OneStop);
            _serialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

            if(_serialPort->open(QIODevice::ReadWrite))
            {
                qDebug() << "Serial port "<< _serialPort->portName() << "is connected";
                qDebug() << "Baud rate:" << _serialPort->baudRate();
                connect(_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
                ui->connectButton->setText("Disconnect");
            } else {
                qDebug() << "Could not connect to serial port";
                qDebug() << _serialPort->error();
                ui->portComBox->clear();
                availablePorts = false;
            }
        } else {
            qDebug() << "Serial port" << _serialPort->portName() << "is disconnected";
            if(_serialPort != nullptr)
            {
                _serialPort->close();
                delete _serialPort;
            }
            ui->connectButton->setText("Connect");
        }
    } else {
        if(!loadPorts())
        {
            ui->connectButton->setText("Connect device");
            ui->connectButton->setCheckable(true);
            qDebug() << "No serial port found";
        } else {
            availablePorts = true;
            ui->connectButton->setCheckable(true);
            ui->connectButton->setText("Connect");
        }
    }



}


void MainWindow::on_sendDataButton_clicked()
{
    if(_serialPort->isOpen())
    {
        if(!ui->sendInput->text().isEmpty())
        {
            _serialPort->write(ui->sendInput->text().toStdString().c_str());
            ui->sendInput->clear();
        } else {
            qDebug() << "Enter some data first";
        }
    } else {
        qDebug() << "Open serial port";
    }
}

void MainWindow::readData()
{
    if(!_serialPort->isOpen())
    {
        qDebug() << "Serial Port in not open";
        return;
    }
    auto data = _serialPort->readAll();
    ui->listWidget->addItem(data);
}

