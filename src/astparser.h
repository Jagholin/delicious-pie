#pragma once
#include <QString>
#include <QObject>

#define SPIRIT_LEXER

struct LexToken
{
    enum LexTokenType 
    {
        TOK_COMMENT=1,
        TOK_STRINGBORDER,
        TOK_STRINGLIT,
        TOK_IDENTIFIER,
        TOK_OPERATOR,
        TOK_DELIMETER,
        TOK_INTEGER,
        TOK_FLOAT,
        TOK_SPACE,
        TOK_EOL
    };
    LexTokenType type;
    QStringRef subStr;
    size_t lexerState;
};

class Tokenizer_d;
class Tokenizer: public QObject
{
    Q_OBJECT
public:
    Tokenizer();

public slots:
    void setString(const QString*);
    bool fillTokenList(QList<LexToken> &, size_t initialState = 0);

protected:
    QString const *m_str;
    Tokenizer_d *m_d; 
};