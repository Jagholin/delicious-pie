#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "pythonhighlighter.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qmlRegisterType<PythonHighlighter>("Highlighters", 1, 0, "PythonHighlighter");

    QQmlApplicationEngine engine("qml/main.qml");
    return app.exec();
}