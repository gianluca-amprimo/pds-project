#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "LoginWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Editor");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	
	LoginWindow logWin;
	logWin.show();
	
	return app.exec();
}
