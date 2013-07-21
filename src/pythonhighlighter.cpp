#include "pythonhighlighter.h"
#include "astparser.h"
#include <QRegularExpression>
#include <QtDebug>

class PythonHighlighter_d : public QSyntaxHighlighter
{
    Q_OBJECT 
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

    m_stringLitFormat.setForeground(Qt::darkMagenta);

    m_numberFormat.setForeground(Qt::green);

    m_commentFormat.setForeground(Qt::darkGray);
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

    Tokenizer pyTokenizer;
    QList<LexToken> tokenList;
    pyTokenizer.setString(&text);
    int prevBlockState = previousBlockState();
    prevBlockState = prevBlockState == -1? 0: prevBlockState;
    pyTokenizer.fillTokenList(tokenList, prevBlockState);

    for(LexToken token: tokenList)
    {
        switch(token.type)
        {
        case LexToken::TOK_COMMENT:
            setFormat(token.subStr.position(), token.subStr.size(), m_owner->m_commentFormat);
            break;
        case LexToken::TOK_STRINGLIT:
        case LexToken::TOK_STRINGBORDER:
            setFormat(token.subStr.position(), token.subStr.size(), m_owner->m_stringLitFormat);
            break;
        case LexToken::TOK_INTEGER:
        case LexToken::TOK_FLOAT:
            setFormat(token.subStr.position(), token.subStr.size(), m_owner->m_numberFormat);
            break;
        case LexToken::TOK_IDENTIFIER:
            if (pyKeywords.contains(token.subStr.toString()))
                setFormat(token.subStr.position(), token.subStr.size(), m_owner->m_keywordFormat);

            break;
        }
    }

    int myOldState = currentBlockState();
    int myNewState;
    if (tokenList.isEmpty())
        myNewState = prevBlockState;
    else
        myNewState = tokenList.last().lexerState;
    if (myNewState != myOldState)
    {
        setCurrentBlockState(myNewState);
    }
}

#include "pythonhighlighter.moc"