#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct TkPair {
    std::pair<int, int> pair;
    int freq;

    bool operator<(const TkPair &other) const { return freq < other.freq; }
};

class BPE {
  private:
    static inline int nextUniqueToken =
        0x10000; // start > all char values to prevent collisions
    static inline std::map<int, std::string> tokenToString;
    static inline std::map<std::pair<int, int>, std::set<std::vector<int>>>
        wordsWithPair;

  public:
    static inline std::priority_queue<TkPair> statHeap;
    static inline std::map<std::pair<int, int>, int> invalidPairs;

    static void initStats(std::map<std::vector<int>, int> vocab);

    static void mergeVocab(std::pair<int, int> pair,
                           std::map<std::vector<int>, int> &vocab);

    static std::string getTokenString(int token) {
        if (token < 0x10000) {
            return std::string(1, static_cast<char>(token));
        } else {
            return tokenToString[token];
        }
    }

    static int getTokenCount() { return nextUniqueToken; }
};
