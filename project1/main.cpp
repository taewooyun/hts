#include <QApplication>
#include "root.h"
#include "source/chart/chart.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TabWidget w;
    w.resize(1300, 700);

    w.addTab(new QWidget, "tab1");
    // w.addTab(new QWidget, "Home");
    w.addTab(new Chart(), "Chart");
    w.addTab(new QWidget, "DashBoard");
    w.setCurrentIndex(1);
    w.show();

    return a.exec();
}
