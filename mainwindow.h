//
// Created by TURIING on 2023/6/5.
//

#ifndef QSHARK_MAINWINDOW_H
#define QSHARK_MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QVector>
#include <QDebug>

#include "Engine.h"

#include <pcap.h>
#include <winsock2.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;


private:
    void m_findNetworkCard();
    int m_capture();
public slots:
    void onComboBoxCurrentIndexChanged(int index);


private:
    Ui::MainWindow *ui;
    // 用于保存获取的所有设备
    pcap_if_t *m_all_device;
    // 当前选择的设备
    pcap_if_t *m_current_device;
    // 用以保存网络接口会话
    pcap_t *m_session;
    // 用于保存错误信息
    char m_err_buf[PCAP_ERRBUF_SIZE];
    // 是否开始捕获
    bool m_isCapture;
};



#endif //QSHARK_MAINWINDOW_H
