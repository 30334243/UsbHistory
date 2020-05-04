#include <Header.h>
#include "UsbHistory.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UsbHistory w;
	w.show();

	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	return a.exec();
}
