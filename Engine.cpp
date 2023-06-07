//
// Created by TURIING on 2023/6/7.
//

#include "Engine.h"

Engine::Engine() {
    m_isDone = true;
}

bool Engine::setSession(pcap_t *_session) {
    m_session = _session;
    if(_session)
        return true;
    else
        return false;
}

void Engine::setIsDone() {
    m_isDone = false;
}

void Engine::resetIsDone() {
    m_isDone = true;
}

void Engine::run() {
    // 用以保存数据包的时间戳
    time_t _localTimeSec;
    while (true) {
        if(m_isDone)
            break;
        else {
            // 获取下一个数据包
            int _res = pcap_next_ex(m_session, &m_header, &m_pktData);
            // 返回值为0，说明达到超时时间而没有捕获到数据包
            if(_res == 0)
                continue;

            _localTimeSec = m_header->ts.tv_sec;
            // 将时间戳转换为本地时间
            localtime_s(&m_localTime, &_localTimeSec);

            strftime(m_timeString, sizeof(m_timeString), "%H:%M:%S", &m_localTime);
            qDebug() << m_timeString;
        }
    }
}