#include "CareRayServer.h"

#include <CrStructure.h>

// 保存当前活动的探测器实例，用于回调
static CareRayServer* s_instance = nullptr;

static int num = 1;

CareRayServer::CareRayServer()
{
    s_instance = this;
}


CareRayServer::~CareRayServer()
{

    stopServer();
    deinitializeLibrary();
}

void CareRayServer::bandSDK()
{
    //心跳
    server->bind("ping", []() {
        return true;
        });
    // 加载 SDK
    server->bind("LoadSDK", [this]() {
        qDebug() << "LoadSDK被调用";
        return this->LoadSDK();
        });

    // 取消加载 SDK
    server->bind("DeinitializeLibrary", [this]() {
        return this->deinitializeLibrary();
        });

    // 获取 SDK 错误信息
    server->bind("GetLastIntlMsg", [this]() {
        return this->getLastIntlMsg();
        });

    // 获取探测器列表
    server->bind("GetDetectorList", [this]() {
        return this->GetDetectorList();
        });

    // 连接探测器
    server->bind("Connect", [this](int detectorIndex) {
        return this->ConnectDetector(detectorIndex);
        });

    // 断开连接探测器
    server->bind("Disconnect", [this](int detectorIndex) {
        return this->DisconnectDetector(detectorIndex);
        });

    // 获取序列号
    server->bind("GetSerialNumber", [this](int detectorIndex) {
        return this->GetSerialNumber(detectorIndex);
        });

    // 配置探测器
    server->bind("ConfigureDetector", [this](int detectorIndex) {
        return this->ConfigureDetector(detectorIndex);
        });

    // 注册事件回调
    server->bind("registerEventCallback", [this]() {
        return this->registerEventCallback();
        });

    // 注册应用模式
    server->bind("RegisterMode", [this](RegisterPara para) {
        return this->RegisterMode(para);
        });

    // 查询模式信息
    server->bind("GetModeInfoByAppModeKey",
        [this](int detectorIndex, int currentAppModeKey) {
            return this->getModeInfoByAppModeKey(detectorIndex, currentAppModeKey);
        }
    );

    // 开始扫描
    server->bind("StartAcquisition", [this](int detectorIndex, int currentAppModeKey, int options) {
    return this->startAcquisition(detectorIndex, currentAppModeKey,options);
        });
    // 停止扫描
    server->bind("StopAcquisition", [this](int detectorIndex) {
        return this->stopAcquisition(detectorIndex);
        });
    //获取查询状态
    server->bind("QueryAcquisitionStatus", [this](int detectorIndex) {
    return this->queryAcquisitionStatus(detectorIndex);
        });
    //暗场校准
    server->bind("StartDarkCalibration", [this](int detr_index, int app_mode_key, int is_traditional_type, int is_update_defect) {
        return this->startDarkCalibration(detr_index, app_mode_key, is_traditional_type, is_update_defect);
        });

    //亮场校准
    server->bind("StartGainCalibration", [this](int detr_index, int app_mode_key) {
        return this->startGainCalibration(detr_index, app_mode_key);
        });

    //查询校准状态
    server->bind("QueryCalibrationStatus", [this](int detr_index) {
        return this->queryCalibrationStatus(detr_index);
        });

    //生成配置文件
    server->bind("GenerateSampleCustomModeConfigFile", [this](int detr_index) {
        return this->generateSampleCustomModeConfigFile(detr_index);
        });

    //授权曝光
    server->bind("PermitExposure", [this](int detr_index) {
        return this->permitExposure(detr_index);
        });

    //获取应用模式列表
    server->bind("GetApplicationMode", [this](int detr_index) {
        return this->getApplicationMode(detr_index);
        });

}

void CareRayServer::startServer()
{
    try {
        if (server == nullptr)
        {
            server = new rpc::server(rpc_server_ip, rpc_server_port);
        }
        if (m_zmqPub == nullptr)
        {
            m_zmqPub = new ZmqPublisher(mq_server_ip, mq_server_port);
        }
        bandSDK();
        m_zmqPub->start();

        //开始发送数据线程
        m_sendThreadRunning = true;
        m_sendThread = std::thread(&CareRayServer::sendThreadLoop, this);
        //会阻塞
        server->run();
    }
    catch (const std::exception& e) {
        qDebug() << "RPC server start failed:" << e.what();
    }
}

void CareRayServer::stopServer()
{
    m_sendThreadRunning = false;
    m_queueCv.notify_all();

    if (m_sendThread.joinable())
        m_sendThread.join();

    if (server)
        server->stop();

    if (m_zmqPub)
        m_zmqPub->stop();
}

void CareRayServer::EventCallback(int detr_index, CrEvent* event)
{
    if (s_instance) {
        s_instance->onSdkEvent(detr_index, event);
    }
}

std::tuple<int, std::string> CareRayServer::GetSerialNumber(int detectorIndex)
{
    // 获取探测器信息
    CrSystemInfo sysInfo;
    std::string serialNumber = "";
    int result = CrGetSystemInformation(detectorIndex, &sysInfo);
    if (result == CR_OK) {
        serialNumber = std::string(sysInfo.serial_number);
    }
    else 
    {
        serialNumber = "";
    }
    return { result ,serialNumber };
}

int CareRayServer::LoadSDK()
{
    int result = CrInitializeLibrary();
    qDebug() << result;
    return result;
}

int CareRayServer::deinitializeLibrary()
{
    int result = CrDeinitializeLibrary();
    return result;
}

std::string CareRayServer::getLastIntlMsg()
{
    // 获取详细错误信息
    std::array<char, 256> errorMsg{};
    CrGetLastIntlMsg(errorMsg.data(), errorMsg.size());
    return std::string(errorMsg.data());
}

std::tuple<int, int,std::string> CareRayServer::GetDetectorList()
{
    CrDetrIdxAndIPAddr detectorArray[8];
    int validDetectorNum = 0;

    // 获取探测器列表
    int result = CrGetDetectorIndexAndIPAddress(detectorArray, &validDetectorNum);
    if (result != CR_OK || validDetectorNum <= 0) {
        return { result ,validDetectorNum ,""};
    }

    // 使用第一个探测器
    int m_detectorIndex = detectorArray[0].index;

    return {result,m_detectorIndex ,std::string(detectorArray[0].ip_addr)};

}

bool CareRayServer::ConfigureDetector(int detectorIndex)
{
    // 获取系统信息
    CrSystemInfo sysInfo;
    int result = CrGetSystemInformation(detectorIndex, &sysInfo);
    if (result == CR_OK) {
        std::string info = "探测器信息: 描述=" +
            std::string(sysInfo.detr_desc) +
            ", 序列号 = " +
            std::string(sysInfo.serial_number) +
            ", 固件版本=" +
            std::string(sysInfo.firmware_version);
        return true;
    }
    return false;
}

int CareRayServer::ConnectDetector(int detectorIndex)
{
    int result = CrConnect(detectorIndex);
    return result;
}

int CareRayServer::DisconnectDetector(int detectorIndex)
{
    int result = CrDisconnect(detectorIndex);
    return result;
}

int CareRayServer::registerEventCallback()
{
    int result = CrRegisterEventCallbackFun(CareRayServer::EventCallback);
    return result;
}

void CareRayServer::onSdkEvent(int detr_index, CrEvent* event)
{
    if (!event) return;
    ZmqEventHeader hdr{};
    hdr.detr_index = detr_index;
    hdr.event_id = event->event_id;
    hdr.width = event->width;
    hdr.height = event->height;
    hdr.pixel_depth = event->pixel_depth;
    hdr.header_len = event->header_len;
    const void* payload = nullptr;
    // 复制数据
    //根据事件取数据长度
    switch (event->event_id) 
    {
        case CR_EVT_NEW_FRAME:
        {
            if (num == 1)
            {
                m_firstFrameTime = std::chrono::steady_clock::now();
            }
            else
            {
                auto now = std::chrono::steady_clock::now();
                auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - m_firstFrameTime).count();
                double fps = (num -1) * 1000.0 / elapsedMs;
                if (num % 30 == 0)
                {
                    qDebug() << "当前图像个数：" << num << " 当前帧率：" << fps;
                }
            }
            ++num;
            int pixelBytes = event->pixel_depth / 8;
            if (pixelBytes <= 0 || event->width <= 0 || event->height <= 0)
                return;

            uint32_t imageSize = event->width * event->height * pixelBytes;

            hdr.data_len = imageSize;

            payload = static_cast<char*>(event->data);
            return;
            break;
        }
        case CR_EVT_CALIBRATION_IN_PROGRESS:
        case CR_EVT_CALIBRATION_FINISHED:
        {
            hdr.data_len = sizeof(CrCalibrationInfo);
            payload = event->data;
            break;
        }
        case CR_EVT_RAD_ACQ_IN_PROGRESS:
        {
            hdr.data_len = sizeof(CrExpProgress);
            payload = event->data;
            break;
        }
        case CR_EVT_ACQ_STAT_INFO:
        {
            qDebug() <<"实际采集总数量" << num;
            CrAcquisitionStatInfo* statInfo = reinterpret_cast<CrAcquisitionStatInfo*>(event->data);
            if (statInfo) {
                QString msg = QString("采集统计: 总帧数=%1, 丢失=%2, 帧率=%3, 速度=%4MB/s")
                    .arg(statInfo->total_frame_num)
                    .arg(statInfo->lost_frame_num)
                    .arg(statInfo->frame_rate, 0, 'f', 2)
                    .arg(statInfo->transmission_speed, 0, 'f', 2);
                qDebug()<< msg;
            }
            hdr.data_len = sizeof(CrAcquisitionStatInfo);
            payload = event->data;
            break;
        }
        default:
            hdr.data_len = 0;
            break;
    }
    //m_zmqPub->publish(hdr, payload);
    uint32_t dataLen = hdr.data_len;
    enqueueEvent(hdr, payload, dataLen);
}

std::tuple<int, RegisterPara> CareRayServer::RegisterMode(RegisterPara para)
{
   
    // 注册应用模式
    int result = CrRegisterApplicationMode(
        para.detectorIndex,
        para.appModeKey,
        para.modeId,
        &para.frameRate,
        &para.exposureTime,
        para.triggerType,
        para.gainLevel,
        para.rowIndex,
        para.columnIndex
    );
    return { result ,para };
}

bool CareRayServer::getModeInfoByAppModeKey(int detectorIndex, int  currentAppModeKey)
{
    // 获取 modinfo
    CrModeInfo modeInfo;
    CrGetModeInfoByAppModeKey(detectorIndex, currentAppModeKey, &modeInfo);
    return true;
}

int CareRayServer::startAcquisition(int detectorIndex, int currentAppModeKey, int options)
{
    int result = CrStartAcquisitionWithCorrOpt(detectorIndex, currentAppModeKey, options, 1);
    return result;
}

int CareRayServer::stopAcquisition(int detectorIndex)
{
    int result = CrStopAcquisition(detectorIndex);
    return result;
}

std::tuple<int, RpcExpProgress> CareRayServer::queryAcquisitionStatus(int detectorIndex)
{
    CrExpProgress progress;
    int result = CrQueryAcquisitionStatus(detectorIndex, &progress);
    RpcExpProgress rpcProgress;
    rpcProgress.exposure_status = progress.exposure_status;
    rpcProgress.is_fetchable = progress.is_fetchable;
    rpcProgress.result = progress.result;
    return { result,rpcProgress };
}

int CareRayServer::startDarkCalibration(int detr_index, int app_mode_key, int is_traditional_type, int is_update_defect)
{
    return CrStartDarkCalibration(detr_index, app_mode_key, is_traditional_type, is_update_defect);
}

int CareRayServer::startGainCalibration(int detr_index, int app_mode_key)
{
    return CrStartGainCalibration(detr_index, app_mode_key);
}

std::tuple<int, RpcCalibrationInfo> CareRayServer::queryCalibrationStatus(int detr_index)
{
    CrCalibrationInfo info;
    int result = CrQueryCalibrationStatus(detr_index, &info);
    RpcCalibrationInfo rpcInfo;
    rpcInfo.total_frame_num = info.total_frame_num;
    rpcInfo.current_frame_num = info.current_frame_num;
    rpcInfo.current_frame_mean = info.current_frame_mean;
    rpcInfo.error_msg = info.error_msg;
    rpcInfo.result = info.result;
    rpcInfo.exposure_status = info.exposure_status;
    rpcInfo.target_gray_value = info.target_gray_value;
    return { result ,rpcInfo};
}

int CareRayServer::generateSampleCustomModeConfigFile(int detr_index)
{
    CrGenerateSampleCustomModeConfigFile(detr_index);
    return 0;
}

int CareRayServer::permitExposure(int detr_index)
{
    return CrPermitExposure(detr_index);
}

std::tuple<int, int, std::vector<RpcModeInfo>> CareRayServer::getApplicationMode(int detr_index)
{
    CrModeInfo modeArray[16];
    int validModeNum = 0;
    int result = CrGetApplicationMode(detr_index, modeArray, &validModeNum);
    std::vector<RpcModeInfo> rpcModes;
    rpcModes.reserve(validModeNum); // 提前预分配
    for (int i = 0; i < validModeNum; ++i)
    {
        const CrModeInfo& mode = modeArray[i];
        RpcModeInfo rpc;

        // 基本字段
        rpc.mode_id = mode.mode_id;
        rpc.image_width = mode.image_width;
        rpc.image_height = mode.image_height;
        rpc.cutoff_x = mode.cutoff_x;
        rpc.cutoff_y = mode.cutoff_y;
        rpc.bin_x = mode.bin_x;
        rpc.bin_y = mode.bin_y;
        rpc.max_frame_rate = mode.max_frame_rate;
        rpc.max_exposure_time = mode.max_exposure_time;
        rpc.pixel_depth = mode.pixel_depth;

        // trigger_types 数组 -> vector
        rpc.trigger_types.assign(mode.trigger_types, mode.trigger_types + 16);
        rpc.valid_trigger_type_num = mode.valid_trigger_type_num;

        // gain_levels 数组 -> vector
        rpc.gain_levels.assign(mode.gain_levels, mode.gain_levels + 16);
        rpc.valid_gain_num = mode.valid_gain_num;

        // 其他字段
        rpc.default_trigger_type = mode.default_trigger_type;
        rpc.default_gain_level = mode.default_gain_level;
        rpc.roi_x = mode.roi_x;
        rpc.roi_y = mode.roi_y;
        rpc.mode_conf = mode.mode_conf;

        // 描述信息 char[256] -> std::string
        rpc.desc = std::string(mode.desc);

        rpcModes.push_back(std::move(rpc));
    }


    return { result, validModeNum, rpcModes };
}

void CareRayServer::enqueueEvent(const ZmqEventHeader& hdr,
    const void* payload,
    uint32_t len)
{
    if (!payload || len == 0) return;

    ZmqEventPacket pkt;
    pkt.header = hdr;
    pkt.payload.resize(len);
    std::memcpy(pkt.payload.data(), payload, len);

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        if (m_eventQueue.size() >= MAX_QUEUE_SIZE)
        {
            m_eventQueue.pop_front();
        }

        m_eventQueue.emplace_back(std::move(pkt));
    }

    m_queueCv.notify_one();
}

void CareRayServer::sendThreadLoop()
{
    while (m_sendThreadRunning)
    {
        ZmqEventPacket pkt;

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            m_queueCv.wait(lock, [&]() {
                return !m_eventQueue.empty() || !m_sendThreadRunning;
                });

            if (!m_sendThreadRunning)
                break;

            pkt = std::move(m_eventQueue.front());
            m_eventQueue.pop_front();
        }

        // 真正发送
        if (m_zmqPub)
        {
            m_zmqPub->publish(pkt.header,
                pkt.payload.data());
        }
    }
}
