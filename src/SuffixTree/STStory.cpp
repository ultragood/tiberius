#include "STStory.h"

bool NLP::SuffixTree::STStory::operator<(const NLP::SuffixTree::STStory &right) const {
    return this->storyId < right.storyId;
}

bool NLP::SuffixTree::STStory::operator==(const NLP::SuffixTree::STStory &right) const {
    return this->storyId == right.storyId;
}

NLP::SuffixTree::STStory& NLP::SuffixTree::STStory::operator=(const NLP::SuffixTree::STStory &right) {
    this->storyId = right.storyId;
    this->arcCount = right.arcCount;
    return *this;
}
