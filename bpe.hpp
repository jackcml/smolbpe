#include <map>
#include <string>
#include <utility>
#include <vector>

class BPE {
  private:
    static inline int nextUniqueToken =
        0x10000; // start > all char values to prevent collisions
    static inline std::map<int, std::string> tokenToString;

  public:
    static std::map<std::pair<int, int>, int>
    getStats(std::map<std::vector<int>, int> vocab);

    static std::map<std::vector<int>, int>
    mergeVocab(std::pair<int, int> pair, std::map<std::vector<int>, int> vocab);

    static std::string getTokenString(int token) {
        if (token < 0x10000) {
            return std::string(1, static_cast<char>(token));
        } else {
            return tokenToString[token];
        }
    }
};
