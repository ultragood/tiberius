#include "STStory.h"

bool suffixtree::STStory::operator<(const suffixtree::STStory &right) const {
    return this->storyId < right.storyId;
}

bool suffixtree::STStory::operator==(const suffixtree::STStory &right) const {
    return this->storyId == right.storyId;
}

suffixtree::STStory& suffixtree::STStory::operator=(const suffixtree::STStory &right) {
    this->storyId = right.storyId;
    this->arcCount = right.arcCount;
    return *this;
}
