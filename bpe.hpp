#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class BPE {
  private:
    int m_nextUniqueToken =
        0x10000; // start > all char values to prevent collisions
    std::map<int, std::string> m_tokenToString;
    std::map<std::pair<int, int>, std::set<std::vector<int>>> m_wordsWithPair;
    std::map<std::pair<int, int>, int> m_pairFreq;

  public:
    void run(std::map<std::vector<int>, int> vocab, int max_tokens);

    void initStats(std::map<std::vector<int>, int> vocab);

    void mergeVocab(std::pair<int, int> pair,
                    std::map<std::vector<int>, int> &vocab);

    std::string getTokenString(int token) {
        if (token < 0x10000) {
            return std::string(1, static_cast<char>(token));
        } else {
            return m_tokenToString[token];
        }
    }

    int getTokenCount() { return m_nextUniqueToken; }
};
