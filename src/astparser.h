#pragma once
#include <QString>
#include <QObject>

struct LexToken
{
    enum LexTokenType 
    {
        TOK_COMMENT=1,
        TOK_STRINGLIT,
        TOK_WORD,
        TOK_SYMBOL,
        TOK_SPACE,
        TOK_EOL
    };
    LexTokenType type;
    QStringRef subStr;
};

class Tokenizer: public QObject
{
    Q_OBJECT
public:
    Tokenizer();

public slots:
    void setString(const QString*);
    void fillTokenList(QList<LexToken> &);

protected:
    QString const *m_str;
};