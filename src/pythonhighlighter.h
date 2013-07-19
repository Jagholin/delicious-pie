#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QQuickTextDocument>

class PythonHighlighter_d;

class PythonHighlighter: public QObject
{
    Q_OBJECT
public:
    PythonHighlighter();
    ~PythonHighlighter();

    friend class PythonHighlighter_d;
public slots:
    void setDocument(QQuickTextDocument*);

protected slots:
    void onPrivateDestroyed();

protected:
    //void highlightBlock( const QString &text );

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_stringLitFormat;
    QTextCharFormat m_numberFormat;
    QTextCharFormat m_commentFormat;
    PythonHighlighter_d *m_d;
};