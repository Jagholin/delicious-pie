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
        singleQuoteLiteral(L"(r|u|R|U)?\'", LexToken::TOK_STRINGBORDER)
    ,   singleQuoteLiteralContent(L"([^\\\'\\n]*(\\\\.)?)+", LexToken::TOK_STRINGLIT)
    ,   singleQuoteLiteralEnd(L"\'", LexToken::TOK_STRINGBORDER)
    ,   doubleQuoteLiteral(L"(r|u|R|U)?\\\"", LexToken::TOK_STRINGBORDER)
    ,   doubleQuoteLiteralContent(L"([^\\\"\\n]*(\\\\.)?)+", LexToken::TOK_STRINGLIT)
    ,   doubleQuoteLiteralEnd(L"\\\"", LexToken::TOK_STRINGBORDER)
    ,   tripleSingleQuoteLiteral(L"(r|u|R|U)?\'\'\'", LexToken::TOK_STRINGBORDER)
    ,   tripleSingleQuoteLiteralContent(L"([^\']+(\'|\'\')?)+", LexToken::TOK_STRINGLIT)
    ,   tripleSingleQuoteLiteralEnd(L"\'\'\'", LexToken::TOK_STRINGBORDER)
    ,   tripleDoubleQuoteLiteral(L"(r|u|R|U)?\\\"\\\"\\\"", LexToken::TOK_STRINGBORDER)
    ,   tripleDoubleQuoteLiteralContent(L"([^\"]+(\\\"|\\\"\\\")?)+", LexToken::TOK_STRINGLIT)
    ,   tripleDoubleQuoteLiteralEnd(L"\\\"\\\"\\\"", LexToken::TOK_STRINGBORDER)
    ,   comment(L"#.*$", LexToken::TOK_COMMENT)
    {
        this->self += comment;

        this->self.add
            (L"\\+|-|\\*\\*|\\*|\\/\\/|\\/|%|<<|>>|&|\\||\\^|~|<=|>=|<|>|==|!=", LexToken::TOK_OPERATOR)
            (L"\\(|\\)|\\[|\\]|\\{|\\}|,|:|\\.|;|@|=", LexToken::TOK_DELIMETER)
            (L"[ \t]+", LexToken::TOK_SPACE)
            (L"[a-zA-Z_]\\w*", LexToken::TOK_IDENTIFIER)
            (L"(\\d+)?\\.\\d+|\\d+\\.|((\\d+)?\\.\\d+|\\d+\\.?)[eE][+\\-]?\\d+", LexToken::TOK_FLOAT)
            (L"[1-9]\\d*|0([xX][0-9a-fA-F]+|[bB][01]+|[oO][0-7]+)", LexToken::TOK_INTEGER)
            (L"\\n", LexToken::TOK_EOL);

        this->self += singleQuoteLiteral [lex::_state = L"SINGLEQUOTE"]
            |   doubleQuoteLiteral [lex::_state = L"DOUBLEQUOTE"]
            |   tripleSingleQuoteLiteral [lex::_state = L"TRIPLESINGLEQUOTE"]
            |   tripleDoubleQuoteLiteral [lex::_state = L"TRIPLEDOUBLEQUOTE"];

        this->self(L"SINGLEQUOTE") =
            singleQuoteLiteralContent | singleQuoteLiteralEnd [lex::_state = L"INITIAL"];
        this->self(L"DOUBLEQUOTE") = 
            doubleQuoteLiteralContent | doubleQuoteLiteralEnd [lex::_state = L"INITIAL"];
        this->self(L"TRIPLESINGLEQUOTE") =
            tripleSingleQuoteLiteralContent | tripleSingleQuoteLiteralEnd [lex::_state = L"INITIAL"];
        this->self(L"TRIPLEDOUBLEQUOTE") =
            tripleDoubleQuoteLiteralContent | tripleDoubleQuoteLiteralEnd [lex::_state = L"INITIAL"];

        initStatesMap();
    }

    void initStatesMap()
    {
        statesMap.insert(map_state(L"INITIAL"), L"INITIAL");
        statesMap.insert(map_state(L"SINGLEQUOTE"), L"SINGLEQUOTE");
        statesMap.insert(map_state(L"DOUBLEQUOTE"), L"DOUBLEQUOTE");
        statesMap.insert(map_state(L"TRIPLESINGLEQUOTE"), L"TRIPLESINGLEQUOTE");
        statesMap.insert(map_state(L"TRIPLEDOUBLEQUOTE"), L"TRIPLEDOUBLEQUOTE");
    }

    const wchar_t* mapIntegerToState(size_t val)
    {
        return statesMap[val].c_str();
    }

    lex::token_def<boost::spirit::unused_type, wchar_t, LexToken::LexTokenType>
        singleQuoteLiteral, singleQuoteLiteralContent, singleQuoteLiteralEnd,
        doubleQuoteLiteral, doubleQuoteLiteralContent, doubleQuoteLiteralEnd,
        tripleSingleQuoteLiteral, tripleSingleQuoteLiteralContent, tripleSingleQuoteLiteralEnd,
        tripleDoubleQuoteLiteral, tripleDoubleQuoteLiteralContent, tripleDoubleQuoteLiteralEnd,
        comment;

    QMap <size_t, std::wstring> statesMap;
};

typedef lex::lexertl::token<std::wstring::const_iterator
    , boost::mpl::vector<>
    , boost::mpl::true_
    , LexToken::LexTokenType> token_type;
typedef lex::lexertl::actor_lexer<token_type> lexer_type;

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
    myToken.lexerState = token.state();
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

bool Tokenizer::fillTokenList(QList<LexToken> &list, size_t initialState)
{
    std::wstring unicodeStr = m_str->toStdWString();
    auto begin = unicodeStr.cbegin();
    word_counter<lexer_type> pythonLexer;
    return lex::tokenize(begin, unicodeStr.cend(), pythonLexer, 
        std::bind(&onTokenReceived, std::placeholders::_1, std::cref(unicodeStr), m_str, std::ref(list))
        , pythonLexer.mapIntegerToState(initialState));
}