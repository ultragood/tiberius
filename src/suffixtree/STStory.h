#ifndef __STStory_H__
#define __STStory_H__

#include <string>

using namespace std;

namespace suffixtree {

  class STStory {
    friend class STStoryCompare;

  public:
    STStory(string &storyId, int count) {
      this->storyId = storyId;
      this->arcCount = count;
    }

    STStory(const STStory &story) {
      this->storyId = story.storyId;
      this->arcCount = story.arcCount;
    }

    void addToArcCount(int inc) {
      this->arcCount += inc;
    }

    long getArcCount() {
      return this->arcCount;
    }

    bool operator<(const STStory &right) const;
    bool operator==(const STStory &right) const;
    STStory& operator=(const STStory &right) ;

  private:
    string storyId;
    long arcCount;
  };

  class STStoryCompare {
  public:
    inline bool operator()(const STStory &left, const STStory &right) {
      return left.storyId < right.storyId;
    }
  };
}

#endif
