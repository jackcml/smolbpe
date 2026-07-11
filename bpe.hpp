#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "mutable_priority_map.hpp"

struct PairHash {
    std::size_t operator()(const std::pair<int, int> &p) const noexcept {
        const std::size_t h1 = std::hash<int>{}(p.first);
        const std::size_t h2 = std::hash<int>{}(p.second);

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

class BPE {
  private:
    int m_nextUniqueToken =
        0x10000; // start > all char values to prevent collisions
    std::map<int, std::string> m_tokenToString;
    std::map<std::string, int> m_stringToToken;
    std::map<std::pair<int, int>, std::set<std::vector<int>>> m_wordsWithPair;
    MutablePriorityMap<std::pair<int, int>, int, PairHash> m_pairFreq;

  public:
    void run(std::map<std::vector<int>, int> vocab, int max_tokens);

    void initStats(std::map<std::vector<int>, int> vocab);

    void mergeVocab(std::pair<int, int> pair,
                    std::map<std::vector<int>, int> &vocab);

    std::vector<int> tokenize(std::string text);
    std::string detokenize(std::vector<int> tokens);

    std::string getTokenString(int token) {
        if (token < 0x10000) {
            return std::string(1, static_cast<char>(token));
        } else {
            return m_tokenToString[token];
        }
    }

    int getTokenCount() { return m_nextUniqueToken; }
};
