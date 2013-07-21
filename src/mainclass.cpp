#include <Python.h>

#include "mainclass.h"
#include "pythonhighlighter.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickTextDocument>
#include <QTextDocumentWriter>
#include <QDebug>

class MainClass_d 
{
public:
    MainClass_d(int, char**, QObject*);

    int exec();
    void saveDocument(QQuickTextDocument* doc, QUrl filePath);
    bool runPython(QString code);

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

void MainClass::loadFile(QQuickTextDocument* doc, QUrl filePath)
{
    QFile file(filePath.toLocalFile());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray fileText = file.readAll();
    doc->textDocument()->setPlainText(QString::fromUtf8(fileText));
}

void MainClass::runDocument(QQuickTextDocument* doc)
{
    if (! m_d->runPython(doc->textDocument()->toPlainText()))
        qWarning() << "Python running failure";
}

MainClass_d::MainClass_d(int argc, char **argv, QObject *owner):
    m_app(argc, argv)
{
    Py_Initialize();

    qmlRegisterType<PythonHighlighter>("Highlighters", 1, 0, "PythonHighlighter");

    m_engine.rootContext()->setContextProperty("mainApp", owner);
    m_engine.load("qml/main.qml");
    qDebug() << m_engine.offlineStoragePath();
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

bool MainClass_d::runPython(QString code)
{
    return PyRun_SimpleString(code.toUtf8()) == 0;
}
