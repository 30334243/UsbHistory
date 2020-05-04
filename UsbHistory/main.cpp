#include "UsbHistory.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UsbHistory w;
	w.show();
	return a.exec();
}
