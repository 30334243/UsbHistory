#pragma once
#include "ui_UsbHistory.h"

using FilesInfo = std::vector<QFileInfo>;
using InfoUsb = std::vector<std::tuple<QString, QString, QString, QString, QString, QString, QString, FilesInfo>>;
using InfoMtp = std::vector<std::tuple<QString, QString, QString, QString, QString, QString, QString, QString, FilesInfo>>;
using NameAndSerial = std::vector<std::pair<QString, QString>>;

class UsbHistory : public QMainWindow
{
	Q_OBJECT

public:
	UsbHistory(QWidget* parent = Q_NULLPTR);

private:
	Ui::UsbHistoryClass ui;

	u64 mStartTime;
	u64 mEndTime;

	InfoUsb mInfoUsb;
	InfoMtp mInfoMtp;
	NameAndSerial mNameAndSerial;
	FilesInfo mFilesInfo;

	const QString REG_USB{ "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Enum\\USB" };
	const QString REG_USBSTOR{ "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Enum\\USBSTOR" };
	const QString REG_USERASSIST{ "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist" };

	const QString TEMP{ "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Enum\\USB\\VID_1004&PID_633E\\LGM70089bdccd7" };

	const QString SERVICE_MTP{ "WUDFWpdMtp" };
	const QString SERVICE_USB{ "USBSTOR" };

	const QString DEVICE_DESC{ "DeviceDesc" };
	const QString SERVICE{ "Service" };
	const QString MFG{ "Mfg" };
	const QString FRIENDLYNAME{ "FriendlyName" };

	const QString INSTALL{ "Properties\\{83da6326-97a6-4088-9453-a1923f573b29}\\0064" };
	const QString LAST_MOUNT{ "Properties\\{83da6326-97a6-4088-9453-a1923f573b29}\\0066" };
	const QString LAST_UNMOUNT{ "Properties\\{83da6326-97a6-4088-9453-a1923f573b29}\\0067" };

	const QString RECENT{ "Recent" };

	QListWidget* mList;
	QListWidget* mListAssist;

	const int SZ_VID{ 4 };
	const int SZ_PID{ 4 };

	enum class Type
	{
		Serial,
		Vendor,
		Product,
		Name,
		Mfg
	};
	enum class Time
	{
		Install,
		LastMount,
		LastUnmount
	};

	std::vector<QString> findDirsRecent();

	int treeCount(QTreeWidget* tree, QTreeWidgetItem* parent);
	void DeleteItem(QTreeWidgetItem* currentItem);
	QTreeWidgetItem* InsertItem(QTreeWidgetItem* parent, QString text);
	int indexOf(QTreeWidgetItem* parent);
	QString nameOf(QTreeWidgetItem* item);
	void showAll(void);
	QTreeWidgetItem* insertTree(const QString& alpha, const QString& device, const QString& path);

	void findMTP();
	void findUSB();

	QString getDirSym(const QString& sym);

	bool isCompare(const std::vector<std::pair<QString, QString>>& container, const QString& search);
	bool isCompare(const std::vector<std::pair<QString, QString>>& container, const std::pair<QString, QString>& search);

	int count; //переменная для хранения номера очередного узла
	int currentColumn; //номер столбца, на самом деле будет = 0, т.к. у нас 1 столбец
	QTreeWidgetItem* currentItem; //текущий элемент, запоминается при клике в QTreeWidget

public slots:
	void on_ActScan_triggered();
	void on_ActUserAssist_triggered();
	void on_ActRecent_triggered();
	void on_Tree_itemClicked(QTreeWidgetItem* item, int column);
	void addDetailsInfoUsb(const int& idx);
	void addDetailsInfoMtp(const int& idx);

};
