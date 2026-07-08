#include <map>
#include <string>
#include <utility>

class BPE {
  public:
    static std::map<std::pair<int, int>, int>
    getStats(std::map<std::string, int> vocab);
};
