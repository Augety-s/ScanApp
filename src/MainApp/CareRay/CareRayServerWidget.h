#pragma once

#include <QWidget>
#include "ui_CareRayServerWidget.h"
#include <CareRayServer.h>
class CareRayServerWidget : public QWidget
{
	Q_OBJECT

public:
	CareRayServerWidget(QWidget *parent = nullptr);
	~CareRayServerWidget();

	void onDetectorStatusChanged(CareRayServer::Status status);

private:
	Ui::CareRayServerWidget* ui;
	CareRayServer* server = nullptr;
};
