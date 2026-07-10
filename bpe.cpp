#include "bpe.hpp"
#include <algorithm>
#include <iostream>

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
    std::set<std::vector<int>> words = BPE::wordsWithPair[pair]; // snapshot
    BPE::wordsWithPair.erase(pair); // pair no longer exists after this merge
    for (const auto &word : words) {
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

                // for pair {a,b}, we're removing the left neighbor {x,a}
                if (i >= 2) {
                    --pairFreq[{word[i - 2], word[i - 1]}];
                }
                // and the right neighbor {b,y}
                if (i + 1 < word.size()) {
                    --pairFreq[{word[i], word[i + 1]}];
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
            }
        }

        vocab[newWord] += freq;
        vocab.erase(word);

        for (size_t i = 1; i < word.size(); ++i) {
            wordsWithPair[{word[i - 1], word[i]}].erase(word);
        }
        // add the new word to all its pairs' sets
        for (size_t i = 1; i < newWord.size(); ++i) {
            wordsWithPair[{newWord[i - 1], newWord[i]}].insert(newWord);
        }
    }
}

void BPE::run(std::map<std::vector<int>, int> vocab, int max_tokens) {
    BPE::initStats(vocab);
    while (BPE::getTokenCount() < max_tokens) {
        auto it = std::max_element(
            BPE::pairFreq.begin(), BPE::pairFreq.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; });
        if (it == BPE::pairFreq.end()) {
            std::cout << "Exhausted possible merges at " << BPE::getTokenCount()
                      << " tokens." << std::endl;
            break;
        }

        auto &[pair, freq] = *it;
        BPE::mergeVocab(pair, vocab);
        BPE::pairFreq.erase(pair);
    }
}