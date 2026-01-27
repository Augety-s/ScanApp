#include "CareRayServerWidget.h"
#include <QPushButton>
#include <QtConcurrent>
CareRayServerWidget::CareRayServerWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::CareRayServerWidget())
{
	ui->setupUi(this);
	connect(ui->startServer, &QPushButton::clicked, this, [this]() {
		auto rpcIp = ui->rpcServerIp->text().toStdString();
		int rpcPort = ui->rpcServerPort->text().toInt();
		auto mqIp = ui->mqServerIp->text().toStdString();
		int mqPort = ui->mqServerPort->text().toInt();
		if (server == nullptr)
			server = new CareRayServer();
		QtConcurrent::run([=]() {
			server->rpc_server_ip = rpcIp;
			server->rpc_server_port = rpcPort;
			server->mq_server_ip = mqIp;
			server->mq_server_port = mqPort;
			server->startServer();
			});
		ui->messageEdit->setText("服务已经启动");
		});
	connect(ui->stopServer, &QPushButton::clicked, this, [this]() {
		if (server == nullptr)
		{
			return;
		}
		server->stopServer();
		ui->messageEdit->setText("服务已经停止");
		});
}

CareRayServerWidget::~CareRayServerWidget()
{
	server->stopServer();
}
