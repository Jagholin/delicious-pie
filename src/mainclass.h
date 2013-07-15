#pragma once
#include <QObject>
#include <QUrl>

class MainClass_d;
class QQuickTextDocument;
class MainClass: public QObject
{
    Q_OBJECT
public:
    MainClass(int, char**, QObject* parent = nullptr);
    
    int exec();

public slots:
    
    void saveDocument(QQuickTextDocument* doc, QUrl filePath);
protected:
    MainClass_d *m_d;
};