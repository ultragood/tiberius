#ifndef WHITESPACETOKENSTREAM_H_INCLUDED
#define WHITESPACETOKENSTREAM_H_INCLUDED

#include <boost/tokenizer.hpp>
#include <string>
#include "Types.h"

using namespace std;
using namespace boost;

namespace tiberius{ namespace analysis { namespace streams{

    class WhitespaceTokenStream {
        public:
        ~WhitespaceTokenStream(){}
        WhitespaceTokenStream(std::string & text);
        virtual bool next(Token &token);

        private:
        tokenizer<> tok; 
        tokenizer<>::iterator tok_it; 

    };
} } }

#endif // WHITESPACETOKENSTREAM_H_INCLUDED
