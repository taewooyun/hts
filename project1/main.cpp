#include <QApplication>
#include "root.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TabWidget w;
    w.resize(1300, 700);

    w.addTab(new QWidget, "tab1");
    w.addTab(new QWidget, "tab2");
    w.addTab(new QWidget, "tab3");
    w.show();

    return a.exec();
}
