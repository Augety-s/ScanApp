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
		QString text = QDateTime::currentDateTime().toString("HH:mm:ss") +"  " + message;
		ui->messageEdit->append(text);
	}, Qt::QueuedConnection);
	connect(ui->clearLog, &QPushButton::clicked, this, [this]() {
		ui->messageEdit->clear();
		});
}

CareRayServerWidget::~CareRayServerWidget()
{
	server->stopServer();
}
