#pragma once
#include "ui_UsbHistory.h"

class UsbHistory : public QMainWindow
{
	Q_OBJECT

public:
	UsbHistory(QWidget *parent = Q_NULLPTR);

private:
	Ui::UsbHistoryClass ui;
};
