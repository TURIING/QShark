//
// Created by TURIING on 2023/6/5.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow), m_isCapture(false) {
    ui->setupUi(this);
    Engine *engine = new Engine;
    /* UI初始化 */
    ui->actionStop->setEnabled(false);


    m_findNetworkCard();

    /* 信号槽 */
    connect(ui->combo_box_devices, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxCurrentIndexChanged);
    connect(ui->actionStart, &QAction::triggered, this, [=]() {
        if(m_isCapture = !m_isCapture; m_isCapture) {
            if(m_capture() != -1) {
                engine->setSession(m_session);
                engine->setIsDone();
                engine->start();

                ui->actionStart->setEnabled(false);
                ui->actionStop->setEnabled(true);
                ui->combo_box_devices->setEnabled(false);
                statusBar()->showMessage(m_current_device->description);
            }else {
                statusBar()->showMessage("Failed to listen on network interface");
            }
        }
    });
    connect(ui->actionStop, &QAction::triggered, this, [=]() {
        if(m_isCapture) {
            m_isCapture = false;
            engine->resetIsDone();
            engine->quit();
            engine->wait();
            ui->actionStop->setEnabled(false);
            ui->actionStart->setEnabled(true);
            ui->combo_box_devices->setEnabled(true);
            pcap_close(m_session);
            m_session = nullptr;
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

/*
 * 列出所有网卡设备
 */
void MainWindow::m_findNetworkCard() {
    int n = pcap_findalldevs(&m_all_device, m_err_buf);
    if(n == -1) {
        ui->statusbar->showMessage("error:" + QString(m_err_buf));
    }else {
        ui->combo_box_devices->clear();
        QString device_name, device_description, item;
        pcap_if_t *device;
        for(device = m_all_device; device != nullptr; device = device->next) {
            device_name = device->name;
            device_name.replace("\\Device\\", "");
            device_description = device->description;
            item = device_name + device_description;
            ui->combo_box_devices->addItem(item);
        }
    }
}

void MainWindow::onComboBoxCurrentIndexChanged(int index) {
    int i = 0;
    // 找到指向当前comboBox选中索引对应的设备指针
    for (m_current_device = m_all_device; i < index; m_current_device = m_current_device->next, i++);

}
/*
 * 捕获数据包
 */
int MainWindow::m_capture() {
    if(!m_current_device)
        return -1;
    // 打开会话
    m_session = pcap_open_live(m_current_device->name, 65536, 1, 1000, m_err_buf);
    if(!m_session) {
        pcap_freealldevs(m_all_device);
        m_current_device = nullptr;
        return -1;
    }
    // 判断是否为以太网
    if(pcap_datalink(m_session) != DLT_EN10MB) {
        pcap_close(m_session);
        pcap_freealldevs(m_all_device);
        m_current_device = nullptr;
        m_session = nullptr;
        return -1;
    }
    return 0;
}

