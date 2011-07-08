#ifndef PHRASE_H
#define Phrase_H

#include <vector>
#include "fries/language.h"

using namespace std;

namespace Keywords {
  class Phrase {
  public:
    //Phrase(string phrase, double ridf) : phrase(phrase),ridf(ridf) { 
      //this->ridf= ridf;
    //}
   
    bool operator< (const Phrase &a) const {
      return this->ridf < a.ridf;
    }
    //protected: 
    double ridf;
    string phrase;

  };
}
#endif
