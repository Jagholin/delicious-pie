#include "mainclass.h"
#include "astparser.h"

int main(int argc, char **argv)
{
    Tokenizer tok;
    QString theStr("\"Hello world!\", says everybody #on the planet");
    QList<LexToken> tokens;
    tok.setString(&theStr);
    tok.fillTokenList(tokens);
    MainClass mainObj(argc, argv);
    return mainObj.exec();
}