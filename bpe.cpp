#include "bpe.hpp"

void BPE::initStats(std::map<std::vector<int>, int> vocab) {
    for (const auto &[word, freq] : vocab) {
        for (size_t i = 1; i < word.size(); ++i) {
            std::pair<int, int> pair = {word[i - 1], word[i]};
            pairFreq[pair] += freq;
            BPE::wordsWithPair[pair].insert(word);
        }
    }
}

void BPE::mergeVocab(std::pair<int, int> pair,
                     std::map<std::vector<int>, int> &vocab) {
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

    int mergeCount = 0;
    std::set<std::vector<int>> newWordsWithPair;
    std::map<std::pair<int, int>, int> newPairFreqs;
    for (const auto &word : BPE::wordsWithPair[pair]) {
        auto freq = vocab[word];

        // if pair in word, replace with unique token
        std::vector<int> newWord;
        newWord.reserve(word.size());
        for (size_t i = 0; i < word.size(); ++i) {
            if (i + 1 < word.size() && word[i] == pair.first &&
                word[i + 1] == pair.second) {
                newWord.push_back(mergedToken);
                ++i; // skip the next token since it's merged
                ++mergeCount;

                if (i > 0) {
                    --pairFreq[{word[i-1], word[i]}];
                }
                if (i + 2 < word.size()) {
                    --pairFreq[{word[i+1], word[i+2]}];
                }
            } else {
                newWord.push_back(word[i]);
            }
        }

        // new adjacencies created in this word
        for (size_t i = 1; i < newWord.size(); ++i) {
            if (newWord[i - 1] == mergedToken || newWord[i] == mergedToken) {
                std::pair<int, int> newPair = {newWord[i - 1], newWord[i]};
                pairFreq[newPair] += freq;
                newWordsWithPair.insert(newWord);
            }
        }

        vocab[newWord] += freq;
        vocab.erase(word);
    }

    pairFreq[pair] = 0; // all merged away
    for (const auto &newWord: newWordsWithPair) {
        wordsWithPair[pair].insert(newWord);
    }
}
