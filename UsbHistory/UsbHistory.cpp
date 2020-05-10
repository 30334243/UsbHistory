#include <Header.h>
#include "UsbHistory.h"

#include "../FileOperations/FileOperations.hpp"
#include "../Time/Time.hpp"
#include "../Reg/Reg.h"

std::string ROT13(std::string original)
{
	std::string result = original;
	for (unsigned int a = 0; a < result.length(); ++a)
	{
		switch (result[a])
		{
		case 'a':
		result[a] = 'n';
		break;
		case 'A':
		result[a] = 'N';
		break;
		case 'b':
		result[a] = 'o';
		break;
		case 'B':
		result[a] = 'O';
		break;
		case 'c':
		result[a] = 'p';
		break;
		case 'C':
		result[a] = 'P';
		break;
		case 'd':
		result[a] = 'q';
		break;
		case 'D':
		result[a] = 'Q';
		break;
		case 'e':
		result[a] = 'r';
		break;
		case 'E':
		result[a] = 'R';
		break;
		case 'f':
		result[a] = 's';
		break;
		case 'F':
		result[a] = 'S';
		break;
		case 'g':
		result[a] = 't';
		break;
		case 'G':
		result[a] = 'T';
		break;
		case 'h':
		result[a] = 'u';
		break;
		case 'H':
		result[a] = 'U';
		break;
		case 'i':
		result[a] = 'v';
		break;
		case 'I':
		result[a] = 'V';
		break;
		case 'j':
		result[a] = 'w';
		break;
		case 'J':
		result[a] = 'W';
		break;
		case 'k':
		result[a] = 'x';
		break;
		case 'K':
		result[a] = 'X';
		break;
		case 'l':
		result[a] = 'y';
		break;
		case 'L':
		result[a] = 'Y';
		break;
		case 'm':
		result[a] = 'z';
		break;
		case 'M':
		result[a] = 'Z';
		break;
		case 'n':
		result[a] = 'a';
		break;
		case 'N':
		result[a] = 'A';
		break;
		case 'o':
		result[a] = 'b';
		break;
		case 'O':
		result[a] = 'B';
		break;
		case 'p':
		result[a] = 'c';
		break;
		case 'P':
		result[a] = 'C';
		break;
		case 'q':
		result[a] = 'd';
		break;
		case 'Q':
		result[a] = 'D';
		break;
		case 'r':
		result[a] = 'e';
		break;
		case 'R':
		result[a] = 'E';
		break;
		case 's':
		result[a] = 'f';
		break;
		case 'S':
		result[a] = 'F';
		break;
		case 't':
		result[a] = 'g';
		break;
		case 'T':
		result[a] = 'G';
		break;
		case 'u':
		result[a] = 'h';
		break;
		case 'U':
		result[a] = 'H';
		break;
		case 'v':
		result[a] = 'i';
		break;
		case 'V':
		result[a] = 'I';
		break;
		case 'w':
		result[a] = 'j';
		break;
		case 'W':
		result[a] = 'J';
		break;
		case 'x':
		result[a] = 'k';
		break;
		case 'X':
		result[a] = 'K';
		break;
		case 'y':
		result[a] = 'l';
		break;
		case 'Y':
		result[a] = 'L';
		break;
		case 'z':
		result[a] = 'm';
		break;
		case 'Z':
		result[a] = 'M';
		break;
		}
	}
	return result;
}

UsbHistory::UsbHistory(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	count = 0;
	ui.Tree->setColumnCount(1);
	QStringList headers;
	headers << "Устройство";
	ui.Tree->setHeaderLabels(headers);
	currentItem = NULL;
	currentColumn = 0;
}

std::vector<QString> UsbHistory::findDirsRecent()
{
	auto root{ QDir::rootPath() + "Users/" };
	QDirIterator itr(
		root,
		QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,
		QDirIterator::Subdirectories);

	std::vector<QString> lst;
	while (itr.hasNext())
	{
		itr.next();
		auto cur{ itr.fileInfo().absoluteFilePath() };
		if (cur.endsWith(RECENT))
		{
			lst.push_back(cur);
		}
	}
	return lst;
}

int UsbHistory::treeCount(QTreeWidget* tree, QTreeWidgetItem* parent = 0)
{
	/*
	не учтёт свёрнутые ветви; потому что правильно было бы делать через модель
	*/
	tree->expandAll(); //а это "костыль"
	int count = 0;
	if (parent == 0)
	{
		int topCount = tree->topLevelItemCount();
		for (int i = 0; i < topCount; i++)
		{
			QTreeWidgetItem* item = tree->topLevelItem(i);
			if (item->isExpanded())
			{
				count += treeCount(tree, item);
			}
		}
		count += topCount;
	}
	else
	{
		int childCount = parent->childCount();
		for (int i = 0; i < childCount; i++)
		{
			QTreeWidgetItem* item = parent->child(i);
			if (item->isExpanded())
			{
				count += treeCount(tree, item);
			}
		}
		count += childCount;
	}
	return count;
}
void UsbHistory::DeleteItem(QTreeWidgetItem* currentItem)
{
	QTreeWidgetItem* parent = currentItem->parent();
	int index;
	if (parent)
	{
		index = parent->indexOfChild(ui.Tree->currentItem());
		delete parent->takeChild(index);
	}
	else
	{
		index = ui.Tree->indexOfTopLevelItem(ui.Tree->currentItem());
		delete ui.Tree->takeTopLevelItem(index);
	}
}

QTreeWidgetItem* UsbHistory::InsertItem(QTreeWidgetItem* parent, QString text)
{
	if (parent == nullptr)
	{
		QTreeWidgetItem* newItem = new QTreeWidgetItem(ui.Tree, ui.Tree->currentItem());
		newItem->setText(0, text);
		newItem->setExpanded(true);
		showAll();
		return newItem;
	}
	else
	{
		QString word = parent->text(currentColumn);
		if (parent->isExpanded() == false)
		{
			parent->setExpanded(true);
		};
		QTreeWidgetItem* newItem = new QTreeWidgetItem(parent, ui.Tree->currentItem());
		newItem->setText(0, text);
		newItem->setExpanded(true);
		showAll();
		return newItem;
	}
}

int UsbHistory::indexOf(QTreeWidgetItem* item)
{
	auto parent{ item->parent() };
	return parent ?
		indexOf(parent) :
		item->treeWidget()->indexOfTopLevelItem(item);
}
QString UsbHistory::nameOf(QTreeWidgetItem* item)
{
	auto parent{ item->parent() };
	return parent ?
		nameOf(parent) :
		item->text(0);
}

void UsbHistory::on_Tree_itemClicked(QTreeWidgetItem* item, int column)
{
	currentItem = item;
	currentColumn = column;

	if (nameOf(item) == "MTP Device")
	{
		addDetailsInfoMtp(indexOf(item));
	}
	else
	{
		addDetailsInfoUsb(indexOf(item) + 1);
	}
}

void UsbHistory::addDetailsInfoUsb(const int& idx)
{
	ui.usbstor->clear();
	u64 correctIdx{
		idx > std::size(mInfoUsb) ?
		std::size(mInfoUsb) + std::size(mInfoMtp) - idx :
		std::size(mInfoUsb) - idx
	};
	const auto& [name, vid, pid, serial, init, lastm, lastum, infoFiles] =
		mInfoUsb.at(std::size(mInfoUsb) - idx);
	ui.usbstor->addItem(
		"VendorID: " + vid + "\n" +
		"ProductID: " + pid + "\n" +
		"Serial: " + serial + "\n" +
		"Первое подключение: " + init + "\n" +
		"Последнее подключение: " + lastm + "\n" +
		"Последнее отключение: " + lastum + "\n"
	);
	for (const auto& file : infoFiles)
	{
		ui.usbstor->addItem("Ярлык: " + file.symLinkTarget());
		ui.usbstor->addItem("Файл: " + file.absoluteFilePath());
	}
}
void UsbHistory::addDetailsInfoMtp(const int& idx)
{
	ui.usbstor->clear();
	u64 correctIdx{
		idx > std::size(mInfoMtp) ?
		(std::size(mInfoMtp) + std::size(mInfoUsb) - 1) - idx :
		std::size(mInfoMtp) - idx
	};
	const auto& [name, vid, pid, serial, mfg, init, lastm, lastum, infoFiles] =
		mInfoMtp.at(correctIdx);
	ui.usbstor->addItem(
		"VendorID: " + vid + "\n" +
		"ProductID: " + pid + "\n" +
		"Serial: " + serial + "\n" +
		"Mfg: " + mfg + "\n" +
		"Первое подключение: " + init + "\n" +
		"Последнее подключение: " + lastm + "\n" +
		"Последнее отключение: " + lastum + "\n"
	);
	for (const auto& file : infoFiles)
	{
		ui.usbstor->addItem("Ярлык: " + file.symLinkTarget());
		ui.usbstor->addItem("Файл: " + file.absoluteFilePath());
	}
}

void UsbHistory::showAll(void)
{
	int cnt = treeCount(ui.Tree);
	//QString str("Всего: " + QString("%1").arg(cnt));
	//setWindowTitle(str);
}

QTreeWidgetItem* UsbHistory::insertTree(
	const QString& alpha, const QString& device, const QString& path)
{
	QTreeWidgetItem* newItem = new QTreeWidgetItem(ui.Tree, ui.Tree->currentItem());
	//указываем 2-м параметром текущий элемент как предшествующий
	newItem->setText(currentColumn, alpha.mid(0, '/'));
	newItem->setExpanded(true);

	newItem = InsertItem(newItem, " " + device);
	newItem = InsertItem(newItem, " " + path);

	showAll();
	return newItem;
}

void UsbHistory::findMTP()
{
	auto dirsRcent{ findDirsRecent() };
	auto lstRecent{ fops::get_files_info(dirsRcent) };

	Reg mtp{ REG_USB };
	auto vids_pids{ mtp.childGroups() };
	for (auto& vid_pid : vids_pids)
	{
		Reg serial{ mtp.getParam() + "\\" + vid_pid };
		auto serial_c{ serial.childGroups() };
		for (const auto& ser : serial_c)
		{
			QString pid, vid, name, mfg, ser_no, inst, lastm, lastum;
			if (QString service{ serial.value(ser + "\\" + SERVICE).toString() };
				service.startsWith(SERVICE_MTP))
			{
				vid = vid_pid.mid(SZ_VID, vid_pid.indexOf("&") - SZ_VID);
				pid = vid_pid.mid(vid_pid.indexOf('&') + 1 + SZ_PID);
				name = serial.value(ser + "\\" + DEVICE_DESC).toString();
				mfg = serial.value(ser + "\\" + MFG).toString();
				ser_no = ser;

				Reg rinst{ serial.getParam() + "\\" + ser + "\\" + INSTALL };
				u64 ru64inst{ rinst.value().toU64() };
				inst = ct::from_filetime(ru64inst);

				Reg rlastu{ serial.getParam() + "\\" + ser + "\\" + LAST_MOUNT };
				u64 ru64lastm{ rlastu.value().toU64() };
				//ui.usbstor->addItem(ct::from_filetime(ru64lastm));
				lastm = ct::from_filetime(ru64lastm);

				Reg rlastum{ serial.getParam() + "\\" + ser + "\\" + LAST_UNMOUNT };
				u64 ru64lastum{ rlastum.value().toU64() };
				//ui.usbstor->addItem(ct::from_filetime(ru64lastum));
				lastum = ct::from_filetime(ru64lastum);

				//ui.usbstor->addItem(lastm + "000000"+lastum);

				std::vector<QFileInfo> corretFiles;
				for (const auto& info : lstRecent)
				{
					u64 time{ ct::to_filetime(info.absoluteFilePath()) };
					//ui.usbstor->addItem(ct::from_filetime(mStartTime));
					//ui.usbstor->addItem(ct::from_filetime(mEndTime));
					if (!(time > mStartTime && time < mEndTime))
					{

					}
					else if (time > ru64lastm && time < ru64lastum)
					{
						corretFiles.push_back(info);
					}
				}
				if (corretFiles.size() != 0)
				{
					mInfoMtp.push_back(
						std::tie(name, vid, pid, ser_no, mfg, inst, lastm, lastum, corretFiles));
				}
			}
		}
	}


	if (mInfoMtp.size() == 0)
	{
		return;
	}

	for (const auto& [name, vid, pid, ser_no, mfg, inst, lastm, lastum, files] : mInfoMtp)
	{
		auto item{ InsertItem(nullptr, "MTP Device") };
		InsertItem(item, name);
	}
}

void UsbHistory::findUSB()
{
	auto dirsRcent{ findDirsRecent() };
	auto lstRecent{ fops::get_files_info(dirsRcent) };

	Reg usb{ REG_USB };
	auto vids_pids{ usb.childGroups() };
	for (auto& vid_pid : vids_pids)
	{
		Reg serial{ usb.getParam() + "\\" + vid_pid };
		auto serial_c{ serial.childGroups() };
		for (const auto& ser : serial_c)
		{
			QString pid, vid, ser_no, inst, lastm, lastum, name;
			if (QString service{ serial.value(ser + "\\" + SERVICE).toString() };
				service.startsWith(SERVICE_USB))
			{
				vid = vid_pid.mid(SZ_VID, vid_pid.indexOf("&") - SZ_VID);
				pid = vid_pid.mid(vid_pid.indexOf('&') + 1 + SZ_PID);
				ser_no = ser;

				Reg rinst{ serial.getParam() + "\\" + ser + "\\" + INSTALL };
				u64 ru64inst{ rinst.value().toU64() };
				inst = ct::from_filetime(ru64inst);

				Reg rlastu{ serial.getParam() + "\\" + ser + "\\" + LAST_MOUNT };
				u64 ru64lastm{ rlastu.value().toU64() };
				lastm = ct::from_filetime(ru64lastm);

				Reg rlastum{ serial.getParam() + "\\" + ser + "\\" + LAST_UNMOUNT };
				u64 ru64lastum{ rlastum.value().toU64() };
				lastum = ct::from_filetime(ru64lastum);

				std::vector<QFileInfo> correctFile;
				for (const auto& info : lstRecent)
				{
					u64 time{ ct::to_filetime(info.absoluteFilePath()) };
					if (!(time > mStartTime && time < mEndTime))
					{

					}
					else if (time > ru64lastm && time < ru64lastum)
					{
						QString root{ info.symLinkTarget().toUpper() };
						QString C{ "C:/" };
						QString D{ "D:/" };
						if (!root.startsWith(C) && !root.startsWith(D))
						{
							correctFile.push_back(info);
						}
					}
				}
				if (correctFile.size() != 0)
				{
					mInfoUsb.push_back(
						std::tie("", vid, pid, ser_no, inst, lastm, lastum, correctFile));
				}
			}
		}
	}

	if (mInfoUsb.size() == 0)
	{
		return;
	}

	Reg usbstor{ REG_USBSTOR };
	auto disks{ usbstor.childGroups() };
	for (auto& disk : disks)
	{
		Reg serial{ usbstor.getParam() + "\\" + disk };
		auto serial_c{ serial.childGroups() };
		for (const auto& ser : serial_c)
		{
			Reg rname{ serial.getParam() + "\\" + ser };
			auto name = rname.value(FRIENDLYNAME).toString();

			mNameAndSerial.push_back(
				std::pair(name, ser.mid(0, ser.indexOf("&"))));
		}
	}

	std::vector<std::pair<QString, QString>> temp;
	for (auto& [name, vid, pid, ser_no, inst, lastm, lastum, info] : mInfoUsb)
	{
		for (auto& newname : mNameAndSerial)
		{
			if (ser_no == newname.second)
			{
				name = newname.first;
			}
		}
	}

	for (const auto& [name, vid, pid, ser_no, inst, lastm, lastum, files] : mInfoUsb)
	{
		auto item{ InsertItem(nullptr, files[0].symLinkTarget().mid(0, 2)) };
		InsertItem(item, name);
	}
}

QString UsbHistory::getDirSym(const QString& sym)
{
	return
		sym.lastIndexOf('.') != -1 ?
		sym.mid(0, sym.lastIndexOf('/')) :
		sym;
}

bool UsbHistory::isCompare(
	const std::vector<std::pair<QString, QString>>& container,
	const QString& search)
{
	for (const auto& pair : container)
	{
		if (pair.second == search)
		{
			return true;
		}
	}

	return false;
}
bool UsbHistory::isCompare(
	const std::vector<std::pair<QString, QString>>& container,
	const std::pair<QString, QString>& search)
{
	for (const auto& pair : container)
	{
		if (pair == search)
		{
			return true;
		}
	}

	return false;
}

void UsbHistory::on_ActScan_triggered()
{
	ui.Tree->clear();
	ui.usbstor->clear();

	mInfoMtp.clear();
	mInfoUsb.clear();
	mNameAndSerial.clear();
	mFilesInfo.clear();

	mStartTime = ct::to_filetime(ui.DateStart->dateTime());
	mEndTime = ct::to_filetime(ui.DataEnd->dateTime());

	findMTP();
	findUSB();
}


void UsbHistory::on_ActUserAssist_triggered()
{
	/*Reg assist{ REG_USERASSIST };
	auto keys{ assist.allKeys() };
	ui.usbstor->addItem(QString::number(keys.size()));
	std::vector<std::string> out;
	for (const auto& key : keys)
	{
		out.push_back(ROT13(key.toStdString()));
	}
	mListAssist = new QListWidget();
	for (auto& elm : out)
	{
		mListAssist->addItem(QString{ elm.c_str() });
	}
	mListAssist->setMinimumSize(500, 800);
	mListAssist->show();*/
	//system("userassist.exe");
	//std::this_thread::sleep_for(1s);
	/*QString path_programm = (fs::current_path() / "userassist.exe").generic_string().data();
	QStringList lst = QStringList();
	std::shared_ptr<QProcess> process(new QProcess(this));
	process->start(path_programm);
	process->waitForFinished();*/

	Reg assist{ "HKEY_USERS" };
	auto keys{ assist.allKeys() };
	std::vector<std::string> out;
	std::vector<QString> str2;
	QString yt{ "Software/Microsoft/Windows/CurrentVersion/Explorer/UserAssist" };
	for (const auto& key : keys)
	{
		if (fops::find(key.toStdString(), yt.toStdString(), std::execution::par))
		{
			QString p{ key.mid(key.lastIndexOf('}') + 1) };
			out.push_back(ROT13(p.toStdString()));
		}
	}
	
	mListAssist = new QListWidget();
	for (auto& elm : out)
	{
		mListAssist->addItem(QString{ elm.c_str() });
	}

	mListAssist->setMinimumSize(500, 800);
	mListAssist->show();
}

void UsbHistory::on_ActRecent_triggered()
{
	auto lst{ findDirsRecent() };
	mList = new QListWidget();
	for (const QFileInfo& info : fops::get_files_info(lst))
	{
		mList->addItem(
			ct::get_filetime<QString>(
				info.absoluteFilePath(), static_cast<int>(ct::type::last_write)));
		mList->addItem(info.symLinkTarget());
		mList->addItem(info.absoluteFilePath());
		mList->addItem("\n");
	}
	mList->setMinimumSize(400, 800);
	mList->show();
}