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

	initUi();
	connect(ui->startServer, &QPushButton::clicked, this, [this]() {
		auto rpcIp = ui->rpcServerIp->text().toStdString();
		int rpcPort = ui->rpcServerPort->text().toInt();
		auto mqIp = ui->mqServerIp->text().toStdString();
		int mqPort = ui->mqServerPort->text().toInt();
		//是否压缩
		bool isCompress = ui->isCompress->isChecked();
		//压缩等级
		int compressLevel = ui->cutNum->currentText().toInt();
		//是否裁剪
		bool isCrop = ui->isCrop->isChecked();

		//获取值
		int roi_x = 0;
		int roi_y = 0;
		int roi_w = 0;
		int roi_h = 0;
		//开始点
		QString startText = ui->startPoint->text();
		QList<int> startPoint;
		for (const QString& s : startText.split(',', Qt::SkipEmptyParts)) {
			bool ok = false;
			int v = s.trimmed().toInt(&ok);
			if (ok)
				startPoint.append(v);
		}
		if (startPoint.size() == 2)
		{
			roi_x = startPoint[0];
			roi_y = startPoint[1];
		}
		//长宽
		QString text = ui->whPoint->text();
		QList<int> point;
		for (const QString& s : text.split(',', Qt::SkipEmptyParts)) {
			bool ok = false;
			int v = s.trimmed().toInt(&ok);
			if (ok)
				point.append(v);
		}
		if (point.size() == 2)
		{
			roi_w = point[0];
			roi_h = point[1];
		}

		QtConcurrent::run([=]() {
			server->rpc_server_ip = rpcIp;
			server->rpc_server_port = rpcPort;
			server->mq_server_ip = mqIp;
			server->mq_server_port = mqPort;
			server->isCompress = isCompress;
			server->compressLevel = compressLevel;
			server->isCrop = isCrop;
			server->roi_x = roi_x;
			server->roi_y = roi_y;
			server->roi_h = roi_h;
			server->roi_w = roi_w;
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
		ui->messageEdit->moveCursor(QTextCursor::End);
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


void CareRayServerWidget::initUi()
{
	for (int i = 1; i <= 12; i++)
	{
		ui->cutNum->addItem(QString::number(i));
	}
}