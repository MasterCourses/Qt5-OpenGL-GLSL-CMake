#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("06_Ex04_Correct_Transform_Normal");
    w.show();
    return a.exec();
}
