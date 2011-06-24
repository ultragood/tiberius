#pragma once
#include "Types.h"

namespace tiberius{ namespace analysis { namespace streams {
        class TokenStream{
            public:
            virtual bool next(Token &token) = 0;
//            virtual string getName() = 0;
            virtual ~TokenStream(){};

        };
} } }
