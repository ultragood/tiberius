#include <iostream>
#include <string>
#include <vector>

#include "WhitespaceTokenStream.h"
#include "TokenStream.h"

using namespace std;
using namespace tiberius::analysis::streams;

WhitespaceTokenStream::WhitespaceTokenStream(string & text) : tok(text){
    tok_it = tok.begin();
}

bool WhitespaceTokenStream::next(Token &token){
    if(tok_it == tok.end()){
        return false;
    }
    token  = *tok_it;

    tok_it++;

    return true;
}
