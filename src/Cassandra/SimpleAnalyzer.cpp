
#include "SimpleAnalyzer.h"
#include "Streams/WhitespaceTokenStream.h"

using namespace tiberius::analysis;
using namespace std;

SimpleAnalyzer::SimpleAnalyzer(){
}

SimpleAnalyzer::~SimpleAnalyzer(){
}


streams::TokenStream  * SimpleAnalyzer::getTokenStream(string & text){
    if(_tokenStream.get() != NULL){
        _tokenStream.reset();
    }

    _tokenStream = boost::shared_ptr<streams::WhitespaceTokenStream>(new streams::WhitespaceTokenStream(text));

    return (streams::TokenStream *) _tokenStream.get();
}


