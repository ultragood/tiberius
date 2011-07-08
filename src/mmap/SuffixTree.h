#ifndef __TIBERIUS_MMAP_SUFFIXTREE__
#define __TIBERIUS_MMAP_SUFFIXTREE__

#include "SuffixTreeVars.h"

#include <string>
#include <list>

#include "freeling.h"
#include "freeling/traces.h"

using namespace std;

namespace tiberius {
  namespace mmap {
    struct WordAttributes {
      unsigned int frequencyCount;
      unsigned int docCount;
      unsigned int level;
      string pos;
      string parent;
      map<string, WordAttributes *> children;
      // used for persistence purposes
      unsigned int nodeOffset;
    };
    class SuffixTree {
    public:
      SuffixTree(void *memoryFile, SuffixTreeVars *globalVars);
      void addStory(list<sentence> &sentences);
      Node * getChildNode(Node *node, string &candidateTerm);
      double calcInformativeness(string &phrase);
      double _ridf(Node *node);
      void persist(WordAttributes *ptr=NULL);

      inline Node * getRoot() {
	tiberius::mmap::Node *root = (tiberius::mmap::Node *) memoryFile + this->globalVars->root_offset;	
	return root;
      }

      inline LinkedList * getLinkedList(long offset) {
	tiberius::mmap::LinkedList *linkedList = (tiberius::mmap::LinkedList *) memoryFile + offset;
	return linkedList;
      }

      inline Node * getNode(long offset) {
	tiberius::mmap::Node *Node = (tiberius::mmap::Node *) memoryFile + offset;
	return Node;
      }

    private:
      void parseSentence(list<sentence> &sentences);
      LinkedList *childList;
      void *memoryFile;
      int llsize;
      SuffixTreeVars *globalVars;
      unsigned long docCount;
      static char *ROOT_NAME;
      WordAttributes *tree;
      
    };
  }
}
#endif
