#ifndef __NODE__
#define __NODE__

namespace tiberius {
  namespace mmap {
    struct LinkedList;
    struct Node {
      int root;
      unsigned int level;
      unsigned int frequencyCount;
      unsigned int docCount;
      char pos[5];
      char term[50];
      LinkedList *childList;
      LinkedList *lastChild;
    };
    struct LinkedList {
      char s[50];
      Node *node;
      LinkedList *next;
    };


  }
}
#endif
