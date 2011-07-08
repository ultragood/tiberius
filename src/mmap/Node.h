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
      char term[21];
      /*
      LinkedList *childList;
      LinkedList *lastChild;
      */
      long offset;
      long childListOffset;
      long lastChildOffset;
    };
    struct LinkedList {
      //char s[50];
      //Node *node;
      //LinkedList *next;
      long nodeOffset;
      long nextOffset;
    };


  }
}
#endif
