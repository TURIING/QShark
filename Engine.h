//
// Created by TURIING on 2023/6/7.
//

#ifndef QSHARK_ENGINE_H
#define QSHARK_ENGINE_H

#include <QThread>
#include "pcap.h"
#include <QDebug>

class Engine: public QThread{
    Q_OBJECT
public:
    Engine();

    bool setSession(pcap_t *_session);
    void setIsDone();
    void resetIsDone();
    void run() override;

private:
    pcap_t *m_session;
    struct pcap_pkthdr *m_header;
    const u_char *m_pktData;
    struct tm m_localTime;
    char m_timeString[16];
    bool m_isDone;
};


#endif //QSHARK_ENGINE_H
