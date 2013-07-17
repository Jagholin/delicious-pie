#include "astparser.h"

#include <QDebug>
#include <QString>

#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <functional>

namespace lex = boost::spirit::lex;

template <typename Lexer>
struct word_counter: public lex::lexer<Lexer>
{
    word_counter():
        comment(L"#.+$", LexToken::TOK_COMMENT)
    ,   strLit(L"\\\"([^\\\"\\\\\\n]*(\\\\\\\")?)*\\\"", LexToken::TOK_STRINGLIT)
    ,   word(L"\\w+", LexToken::TOK_WORD)
    ,   symbolLit(L"[^A-Za-z0-9\\n \\t]+", LexToken::TOK_SYMBOL)
    ,   spaceLit(L"[ \t]+", LexToken::TOK_SPACE)
    ,   eol(L"\\n", LexToken::TOK_EOL)
    {
        this->self = comment | strLit | symbolLit | spaceLit | word | eol;
    }

    lex::token_def<boost::spirit::unused_type, wchar_t, LexToken::LexTokenType> 
        comment, strLit, word, symbolLit, spaceLit, eol;  
};

typedef lex::lexertl::token<std::wstring::const_iterator
    , boost::mpl::vector<>
    , boost::mpl::false_
    , LexToken::LexTokenType> token_type;
typedef lex::lexertl::lexer<token_type> lexer_type;

bool onTokenReceived(token_type const& token, std::wstring const& referenceString,
                     QString const *sourceStr,
                     QList<LexToken> &tokList)
{
    //QString str(QByteArray(token.value().begin(), token.value().size()));
    QStringRef ref(sourceStr, token.value().begin() - referenceString.begin()
        , token.value().size());
    //qDebug() << "Token: " << ref << ";";
    LexToken myToken;
    myToken.type = token.id();
    myToken.subStr = ref;
    tokList.append(myToken);
    return true;
}

Tokenizer::Tokenizer():
    m_str(nullptr)
{

}

void Tokenizer::setString(const QString* str)
{
    m_str = str;
}

void Tokenizer::fillTokenList(QList<LexToken> &list)
{
    std::wstring unicodeStr = m_str->toStdWString();
    auto begin = unicodeStr.cbegin();
    lex::tokenize(begin, unicodeStr.cend(), word_counter<lexer_type>(), 
        std::bind(&onTokenReceived, std::placeholders::_1, std::cref(unicodeStr), m_str, std::ref(list)));
}