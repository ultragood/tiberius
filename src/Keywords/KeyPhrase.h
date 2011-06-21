#ifndef KEYPHRASE_H
#define KEYPHRASE_H

#include <string>

using namespace std;
namespace Keywords {
  class KeyPhrase {
  public:
    KeyPhrase(string a, double b) {
      this->s = a;
      this->tfidf = b;
    }
    
    inline string getPhrase() {
      return this->s;
    }
    inline double getTfidf() {
      return tfidf;
    }
    
    inline bool operator<(const KeyPhrase& kp) const {
      double a = this->tfidf;
      double b = kp.tfidf;
      return (a < b);
    }
    
    inline bool operator<(const double arg1) const {
      return tfidf < arg1;
    }
    double tfidf;
  private:
    string s;
  };
}
#endif
