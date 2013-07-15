#include "mainclass.h"
#include "pythonhighlighter.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickTextDocument>
#include <QTextDocumentWriter>

class MainClass_d 
{
public:
    MainClass_d(int, char**, QObject*);

    int exec();
    void saveDocument(QQuickTextDocument* doc, QUrl filePath);

protected:
    PythonHighlighter m_Highlighter;
    QApplication m_app;
    QQmlApplicationEngine m_engine;
};

MainClass::MainClass(int argc, char **argv, QObject* parent): QObject(parent)
,   m_d(new MainClass_d(argc, argv, this))
{

}

int MainClass::exec()
{
    return m_d->exec();
}

void MainClass::saveDocument(QQuickTextDocument* doc, QUrl filePath)
{
    m_d->saveDocument(doc, filePath);
}

MainClass_d::MainClass_d(int argc, char **argv, QObject *owner):
    m_app(argc, argv)
{
    qmlRegisterType<PythonHighlighter>("Highlighters", 1, 0, "PythonHighlighter");

    m_engine.rootContext()->setContextProperty("mainApp", owner);
    m_engine.load("qml/main.qml");
}

int MainClass_d::exec()
{
    return m_app.exec();
}

void MainClass_d::saveDocument(QQuickTextDocument* doc, QUrl filePath)
{
    QTextDocumentWriter writer(filePath.toLocalFile(), "plaintext");
    writer.write(doc->textDocument());
}

