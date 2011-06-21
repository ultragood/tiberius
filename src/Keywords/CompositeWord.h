#ifndef COMPOSITEWORD_H
#define COMPOSITEWORD_H

#include <vector>
#include "fries/language.h"

using namespace std;

namespace Keywords {
  class CompositeWord {
  public:
    CompositeWord() { }
    
    CompositeWord(word w) {
      this->addWord(w);
    }
    void addWord(word w) {
      this->words.push_back(w);
    }
    void clear() {
      this->words.clear();
    }
    string toString() {
      string w;
      for (unsigned int i=0; i<this->words.size(); i++) {
	if (i > 0) {
	  w.append(" ");
	}
	if (this->words[i].is_multiword()) {
	  list<word> mw_list = this->words[i].get_words_mw();
	  string mw;
	  for (list<word>::iterator it=mw_list.begin(); it!=mw_list.end(); it++) {
	    if (mw.size() > 0) {
	      mw.append(" ");
	    }
	    mw.append(it->get_form());
	  }
	  w.append(mw);
	}else{
	  w.append(this->words[i].get_form());
	}
      }
      return w;
    }
    unsigned int size() {
      return this->words.size();
    }
  private:
    vector<word> words;
  };
}
#endif
