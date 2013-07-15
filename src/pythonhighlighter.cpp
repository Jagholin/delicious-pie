#include "pythonhighlighter.h"
#include <QRegularExpression>
#include <QtDebug>

class PythonHighlighter_d : public QSyntaxHighlighter
{
public:
    PythonHighlighter_d(QTextDocument *doc, PythonHighlighter *owner):
        QSyntaxHighlighter(doc)
    ,   m_owner(owner)
    {
    }

protected:
    void highlightBlock(const QString &text);
    PythonHighlighter *m_owner;
};

PythonHighlighter::PythonHighlighter()
{
    m_keywordFormat.setForeground(Qt::blue);
    m_keywordFormat.setFontWeight(QFont::Bold);
}

PythonHighlighter::~PythonHighlighter()
{
    delete m_d;
}

void PythonHighlighter::setDocument(QQuickTextDocument* doc)
{
    if (m_d) delete m_d;

    if (!doc) qFatal("%s", "no document given");

    m_d = new PythonHighlighter_d(doc->textDocument(), this);
    connect(m_d, SIGNAL(destroyed()), SLOT(onPrivateDestroyed()));
}

void PythonHighlighter::onPrivateDestroyed()
{
    m_d = nullptr;
}

void PythonHighlighter_d::highlightBlock(const QString &text)
{
    // Python keywords, as listed in
    // http://docs.python.org/3.0/reference/lexical_analysis.html
    static const QStringList pyKeywords(QStringList() << "False" << "None" << "True"
        << "and" << "as" << "assert" << "break" << "class" << "continue"
        << "def" << "del" << "elif" << "else" << "except" << "finally"
        << "for" << "from" << "global" << "if" << "import" << "in"
        << "is" << "lambda" << "nonlocal" << "not" << "or" << "pass" << "print"
        << "raise" << "return" << "try" << "while" << "with" << "yield");

    static const QString regexp(QString("\\b(%1)\\b").arg(pyKeywords.join("|")));
    static const QRegularExpression regex(regexp);
    
    QRegularExpressionMatchIterator i = regex.globalMatch(text);
    while (i.hasNext())
    {
        QRegularExpressionMatch match(i.next());
        setFormat(match.capturedStart(), match.capturedLength(), m_owner->m_keywordFormat);
    }
}