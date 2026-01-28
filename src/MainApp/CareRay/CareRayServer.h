#include "CareRayAPI.h"
#include <rpc/server.h>
#include <string>
#include <rpc/msgpack.hpp>
#include <rpc/client.h>
#include <qdebug.h>
#include <rpc/rpc_error.h>
#include <QDateTime>
#include "RpcStruct.hpp"
#include "ZmqPublisher.h"



class CareRayServer
{
public:

	CareRayServer();
	~CareRayServer();
	void bandSDK();
	void startServer();
	void stopServer();

	std::tuple<int, std::string> GetSerialNumber(int detectorIndex);

	/**
	 * \brief 处理回调事件
	 * \param detr_index 探测器索引
	 * \param event 事件指针
	 */
	static void EventCallback(int detr_index, CrEvent* event);

	/**
	 * \brief 加载SDK
	 * \return 是否成功
	 */
	int LoadSDK();

	/**
	 * \brief 取消加载SDK
	 * \return 是否成功
	 */
	int deinitializeLibrary();

	//获取错误信息
	std::string getLastIntlMsg();


	/**
	 * \brief 获取探测器列表
	 * \return 是否成功
	 */
	std::tuple<int, int, std::string> GetDetectorList();

	/**
	 * \brief 配置探测器
	 * \return 是否成功
	 */
	bool ConfigureDetector(int detectorIndex);

	int ConnectDetector(int detectorIndex);

	int DisconnectDetector(int detectorIndex);

	/**
	* \brief 注册自定义模式
	* \param appModeKey 应用模式键值
	* \param modeId 模式ID
	* \param frameRate 帧率
	* \param exposureTime 曝光时间
	* \param triggerType 触发类型
	* \param gainLevel 增益级别
	* \param rowIndex 行索引(全景模式)
	* \param columnIndex 列索引(全景模式)
	* \return 是否成功
	*/
	std::tuple<int, RegisterPara> RegisterMode(RegisterPara para);

	int registerEventCallback();

	void onSdkEvent(int detr_index, CrEvent* event);

	bool getModeInfoByAppModeKey(int detectorIndex,int  currentAppModeKey);

	//开始扫描
	int startAcquisition(int detectorIndex, int currentAppModeKey,int options);

	//停止扫描
	int stopAcquisition(int detectorIndex);

	//获取查询状态
	std::tuple<int, RpcExpProgress> queryAcquisitionStatus(int detectorIndex);

	//暗场校准
	int startDarkCalibration(int detr_index, int app_mode_key, int is_traditional_type, int is_update_defect);

	//亮场校准
	int startGainCalibration(int detr_index, int app_mode_key);

	//查询校准状态
	std::tuple<int, RpcCalibrationInfo> queryCalibrationStatus(int detr_index);


	//生成示例配置文件
	int generateSampleCustomModeConfigFile(int detr_index);

	//授权曝光
	int permitExposure(int detr_index);

	// 获取应用模式列表
	std::tuple<int,int, std::vector<RpcModeInfo>> getApplicationMode(int detr_index);

	std::string mq_server_ip = "0.0.0.0";
	int mq_server_port = 8001;

	std::string rpc_server_ip = "0.0.0.0";
	int rpc_server_port = 8000;

private:
	void sendThreadLoop();

	void enqueueEvent(const ZmqEventHeader& hdr,
		const void* payload,
		uint32_t len);

private:
	rpc::server* server = nullptr;
	ZmqPublisher* m_zmqPub = nullptr;

	// 队列
	std::deque<ZmqEventPacket> m_eventQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_queueCv;

	// 队列最大长度
	static constexpr size_t MAX_QUEUE_SIZE = 1000;

	// 发送线程
	std::thread m_sendThread;
	std::atomic<bool> m_sendThreadRunning{ false };

	std::chrono::steady_clock::time_point m_firstFrameTime;

};