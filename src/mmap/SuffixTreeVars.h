#ifndef __SUFFIXTREEVARS__
#define __SUFFIXTREEVARS__

#include "Node.h"

namespace tiberius {
  namespace mmap {
    struct SuffixTreeVars {
      long offset;
      unsigned long docCount;
      Node *root;
      long root_offset;
    };
  }
}
#endif
