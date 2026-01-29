#include "ZmqPublisher.h"
#include <iostream>

ZmqPublisher::ZmqPublisher(const std::string& bind_ip ,int port)
    : ctx_(1),
    pub_(ctx_, zmq::socket_type::pub),
    m_bind_ip(bind_ip),
    m_port(port)
{
    pub_.set(zmq::sockopt::linger, 0);
}
ZmqPublisher::~ZmqPublisher()
{
    stop();       // 只关 socket
    ctx_.close(); // 程序退出时关 context
}
void ZmqPublisher::start()
{
    // 重新创建 socket，方便下次 start
    pub_ = zmq::socket_t(ctx_, zmq::socket_type::pub);
    pub_.set(zmq::sockopt::linger, 0);
    std::string addr = "tcp://" + m_bind_ip + ":" + std::to_string(m_port);
    pub_.bind(addr);
}

void ZmqPublisher::stop()
{
    if (pub_)
    {
        pub_.close();
    }
}

void ZmqPublisher::publish(const ZmqEventHeader& hdr, const void* data)
{
    // 第一帧：主题（SUB 可以按主题过滤）
    pub_.send(zmq::buffer("EVENT"), zmq::send_flags::sndmore);

    // 第二帧：头
    pub_.send(zmq::buffer(&hdr, sizeof(hdr)), zmq::send_flags::sndmore);

    // 第三帧：数据
    if (hdr.data_len > 0 && data)
    {
        pub_.send(zmq::buffer(data, hdr.data_len),
            zmq::send_flags::none);
    }
    else
    {
        pub_.send(zmq::message_t(), zmq::send_flags::none);
    }
}