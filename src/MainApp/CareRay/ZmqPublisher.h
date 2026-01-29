#pragma once

#include <zmq.hpp>
#include <string>
#include <cstdint>
#pragma pack(push, 1)
struct ZmqEventHeader
{
    int detr_index;
    int event_id;
    int width;
    int height;
    int pixel_depth;
    int header_len;
    uint32_t data_len;
    bool isCompressed;
};

struct ZmqEventPacket
{
    ZmqEventHeader header;
    std::vector<uint8_t> payload; // 深拷贝数据
};

#pragma pack(pop)

class ZmqPublisher
{
public:
    ZmqPublisher(
        const std::string& bind_ip = "0.0.0.0",
        int port = 7000);
    ~ZmqPublisher();
    void publish(const ZmqEventHeader& hdr, const void* data);

    void start();

    void stop();
    std::string m_bind_ip;
    int m_port;
private:
    zmq::context_t ctx_;
    zmq::socket_t pub_;
};