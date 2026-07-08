#include "bpe.hpp"

std::map<std::pair<int, int>, int>
BPE::getStats(std::map<std::vector<int>, int> vocab) {
    std::map<std::pair<int, int>, int> stats;
    for (const auto &[word, freq] : vocab) {
        for (size_t i = 1; i < word.size(); ++i) {
            stats[{word[i - 1], word[i]}] += freq;
        }
    }
    return stats;
}

std::map<std::vector<int>, int>
BPE::mergeVocab(std::pair<int, int> pair,
                std::map<std::vector<int>, int> vocab) {
    std::map<std::vector<int>, int> newVocab;
    int mergedToken = BPE::nextUniqueToken++;

    std::string l, r;
    if (pair.first < 0x10000) {
        l = static_cast<char>(pair.first);
    } else {
        l = BPE::tokenToString[pair.first];
    }
    if (pair.second < 0x10000) {
        r = static_cast<char>(pair.second);
    } else {
        r = BPE::tokenToString[pair.second];
    }
    BPE::tokenToString[mergedToken] = l + r;

    for (const auto &[word, freq] : vocab) {
        // if pair in word, replace with unique token
        std::vector<int> newWord;
        newWord.reserve(word.size());
        for (size_t i = 0; i < word.size(); ++i) {
            if (i + 1 < word.size() && word[i] == pair.first &&
                word[i + 1] == pair.second) {
                newWord.push_back(mergedToken);
                ++i; // skip the next token since it's merged
            } else {
                newWord.push_back(word[i]);
            }
        }
        newVocab[newWord] += freq;
    }
    return newVocab;
}
