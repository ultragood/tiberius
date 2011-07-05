#ifndef __STNode__
#define __STNode__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "SuffixTree.h"
#include "fries/language.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>

using namespace std;

namespace suffixtree {

  class SuffixTree;

  class STNode {
  public:
    STNode(SuffixTree *tree, string &uid);
    STNode(){};
    virtual ~STNode();
    inline bool isRoot() const {
      return (this->root == 1);
    }
    
    inline void setRoot(bool root) {
      if (root) {
	this->root = 1;
      }else{
	this->root = 0;
      }
    }

    STNode * createNodeIfNeeded(string &uid);
    int getLevel();
    void print();
    void addStoryId(string uid);
    unsigned long getNumberOfStories();
    int getFrequencyCount();
    STNode * getChildNode(string &term);
    string getWord();
    //string getPhrase();
    void incrementFrequencyCount();
    STNode* getParent();
    //inline void setFreeLingWord(word w) {
    //  //this->w = w;
    //  this->pos = w.get_parole();
    //}

    inline void setPos(string pos) {
      this->pos = pos;
    }
		
    inline string getPos() {
      //cout << "about to get form: ";
      //cout << w.get_form() << endl;
      //return w.get_parole();
      return this->pos;
    }

    string getUid();
  private:
    string getParentUid();

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
      // note, version is always the latest when saving
//      ar & this->uid;
//      ar & this->term; 
 //     ar & this->parentTerm; 
      ar & this->root;
      ar & this->level;
      ar & this->count;
//      ar & this->childWordSet;
      //ar & this->stories;
      ar & this->pos;
    }

    string term;
    SuffixTree *tree;
    int root;
    string uid;
//    string parentTerm;
    //STNode *parent;
    //map<string, STNode *> wordChildMap;
//    set<string> childWordSet;
    map<string, long> stories;
    int level;
    int count;
    //word w;
    string pos;

  };
}

#endif
