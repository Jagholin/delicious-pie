#include "astparser.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QList>
#include <string>

struct PythonLexerRules
{
    struct ruleDef {
        std::string tokenName;
        std::wstring spiritDef, lexertlDef;
        std::wstring stateStart, stateEnd;
        LexToken::LexTokenType tokenType;
    };

    PythonLexerRules() {
        QFile ruleDefFile("ruledef.txt");
        ruleDefFile.open(QIODevice::ReadOnly);
        QString ruleLine;
        while(!ruleDefFile.atEnd()) {
            ruleLine = QString::fromUtf8(ruleDefFile.readLine());
            QStringList ruleDefs = ruleLine.split('\t');
            ruleDef currentDef;
            currentDef.tokenName = ruleDefs[0].toStdString();
            currentDef.spiritDef = ruleDefs[1].toStdWString();
            if (ruleDefs.size() >= 6) {
                currentDef.lexertlDef = ruleDefs[2].toStdWString();
                currentDef.stateStart = ruleDefs[3].toStdWString();
                currentDef.stateEnd = ruleDefs[4].toStdWString();
                currentDef.tokenType = static_cast<LexToken::LexTokenType>(ruleDefs[5].toInt());
            }
            else {
                currentDef.lexertlDef = currentDef.spiritDef;
                currentDef.stateStart = ruleDefs[2].toStdWString();
                currentDef.stateEnd = ruleDefs[3].toStdWString();
                currentDef.tokenType = static_cast<LexToken::LexTokenType>(ruleDefs[4].toInt());
            }
            rulesList.append(currentDef);
        }
    }

    QList <ruleDef> rulesList;
    static PythonLexerRules* self;

    static const PythonLexerRules* getSingleton(){
        if (self == nullptr) self = new PythonLexerRules;
        return self;
    }

};

PythonLexerRules* PythonLexerRules::self = nullptr;

#ifdef SPIRIT_LEXER

#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <functional>

namespace lex = boost::spirit::lex;

struct ChangeLexerState
{
    ChangeLexerState(size_t state):
        newState(state)
    {

    }

    template <typename Iterator, typename IdType, typename Context>
    void operator()(Iterator&, Iterator&, lex::pass_flags, IdType&, Context& ctx)
    {
        ctx.set_state(newState);
    }
    size_t newState;
};

template <typename Lexer>
struct python_lex_def: public lex::lexer<Lexer>
{
    python_lex_def()
    {
        PythonLexerRules const *rules = PythonLexerRules::getSingleton();

        initStatesMap();

        for (auto const lexRule: rules->rulesList)
        {
            token_def_type myToken(lexRule.spiritDef, lexRule.tokenType);
            auto insertedToken = tokendefs.insert(lexRule.tokenName, myToken);
            if (lexRule.stateStart.compare(L"INITIAL") == 0)
                this->self += (*insertedToken)[ChangeLexerState(map_state(lexRule.stateEnd.c_str()))];
            else
            {
                // doesn't work without next line
                auto currentLexerDef = this->self(lexRule.stateStart); // >:-E
                // Seriously, WTF?
                currentLexerDef += (*insertedToken)[ChangeLexerState(map_state(lexRule.stateEnd.c_str()))];
            }
        }
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
    typedef lex::token_def<boost::spirit::unused_type, wchar_t, LexToken::LexTokenType> token_def_type;
    QMap<std::string, token_def_type>
        tokendefs;

    QMap <size_t, std::wstring> statesMap;
};

typedef lex::lexertl::token<std::wstring::const_iterator
    , boost::mpl::vector<>
    , boost::mpl::true_
    , LexToken::LexTokenType> token_type;
typedef lex::lexertl::actor_lexer<token_type> lexer_type;

class Tokenizer_d
{
public:
    python_lex_def<lexer_type> m_lexer;
};

bool Tokenizer::fillTokenList(QList<LexToken> &list, size_t initialState)
{
    std::wstring unicodeStr = m_str->toStdWString();
    auto begin = unicodeStr.cbegin();
    python_lex_def<lexer_type> &pythonLexer = m_d->m_lexer;

    auto startIter = pythonLexer.begin(begin, unicodeStr.cend(), pythonLexer.mapIntegerToState(initialState));
    auto endIter = pythonLexer.end();
    for (; startIter != endIter && token_is_valid(*startIter); ++startIter){
        QStringRef ref(m_str, startIter->value().begin() - unicodeStr.begin()
            , startIter->value().size());
        LexToken myToken;
        myToken.type = startIter->id();
        myToken.subStr = ref;
        myToken.lexerState = startIter.get_state();
        list.append(myToken);
    }
    return startIter == endIter;
}
#else

#include "lexertl/iterator.hpp"
#include "lexertl/rules.hpp"
#include "lexertl/generator.hpp"
#include "lexertl/state_machine.hpp"

struct PythonLexer
{
    lexertl::wrules m_rules;
    lexertl::wstate_machine m_sm;
    lexertl::wsmatch m_res;
};

bool Tokenizer::fillTokenList(QList<LexToken> &, size_t initialState /* = 0 */)
{
    throw std::runtime_error("Not implemented");
}

#endif //SPIRIT_LEXER

Tokenizer::Tokenizer():
    m_str(nullptr)
{
    m_d = new Tokenizer_d;
}

void Tokenizer::setString(const QString* str)
{
    m_str = str;
}