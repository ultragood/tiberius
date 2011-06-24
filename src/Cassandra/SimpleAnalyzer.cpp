
#include "SimpleAnalyzer.h"
#include "Streams/WhitespaceTokenStream.h"

using namespace tiberius::analysis;
using namespace std;

SimpleAnalyzer::SimpleAnalyzer(){
    _tokenStream = NULL;
}

SimpleAnalyzer::~SimpleAnalyzer(){
    if(_tokenStream){
        delete _tokenStream;
    }
}


streams::TokenStream  * SimpleAnalyzer::getTokenStream(string & text){
    if(_tokenStream != NULL){
        delete _tokenStream;
    }
    _tokenStream =( (streams::TokenStream *) new streams::WhitespaceTokenStream(text) );

    return _tokenStream;
}


