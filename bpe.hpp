#include <map>
#include <utility>
#include <vector>

class BPE {
  private:
    static inline int nextUniqueToken = 0x10000; // start > all char values to prevent collisions

  public:
    std::map<std::pair<int, int>, int> static getStats(
        std::map<std::vector<int>, int> vocab);

    static std::map<std::vector<int>, int>
    mergeVocab(std::pair<int, int> pair, std::map<std::vector<int>, int> vocab);
};
