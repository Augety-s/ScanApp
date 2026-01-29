#include "CareRayServerWidget.h"
#include <QPushButton>
#include <QtConcurrent>
#include <QTime>
#include <QDateTime>
CareRayServerWidget::CareRayServerWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::CareRayServerWidget())
{
	ui->setupUi(this);
	if (server == nullptr)
		server = new CareRayServer();
	connect(ui->startServer, &QPushButton::clicked, this, [this]() {
		auto rpcIp = ui->rpcServerIp->text().toStdString();
		int rpcPort = ui->rpcServerPort->text().toInt();
		auto mqIp = ui->mqServerIp->text().toStdString();
		int mqPort = ui->mqServerPort->text().toInt();
		QtConcurrent::run([=]() {
			server->rpc_server_ip = rpcIp;
			server->rpc_server_port = rpcPort;
			server->mq_server_ip = mqIp;
			server->mq_server_port = mqPort;
			server->startServer();
			});
		});
	connect(ui->stopServer, &QPushButton::clicked, this, [this]() {
		if (server == nullptr)
		{
			return;
		}
		server->stopServer();
		});
	connect(server, &CareRayServer::Message, this, [this](QString message){
		QString text = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +"：  " + message;
		ui->messageEdit->append(text);
	}, Qt::QueuedConnection);
	connect(ui->clearLog, &QPushButton::clicked, this, [this]() {
		ui->messageEdit->clear();
		});
	connect(ui->recoverButton, &QPushButton::clicked, this, [this]() {
		server->recoveryDetectorState();
		});
	connect(server, &CareRayServer::StatusChanged, this, &CareRayServerWidget::onDetectorStatusChanged);
}

CareRayServerWidget::~CareRayServerWidget()
{
	server->stopServer();
}


void CareRayServerWidget::onDetectorStatusChanged(CareRayServer::Status status)
{
	switch (status)
	{
	case CareRayServer::Status::Status_Connected:
		ui->DetectorStatus->setText("已连接");
		break;
	case CareRayServer::Status::Status_Disconnected:
		ui->DetectorStatus->setText("未连接");
		break;
	case CareRayServer::Status::Status_UnLoadSdk:
		ui->DetectorStatus->setText("未加载sdk");
		break;
	case CareRayServer::Status::Status_LoadSdk:
		ui->DetectorStatus->setText("已加载sdk");
		break;
	case CareRayServer::Status::AcquisitionStatus_Started:
		ui->DetectorStatus->setText("采集中");
		break;
	case CareRayServer::Status::AcquisitionStatus_Stopped:
		ui->DetectorStatus->setText("未采集");
		break;
	case CareRayServer::Status::Status_Error:
		ui->DetectorStatus->setText("错误");
		break;
	default:
		break;
	}
}