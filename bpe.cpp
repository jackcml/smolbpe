#include "bpe.hpp"

std::map<std::pair<int, int>, int> BPE::getStats(std::map<std::string, int> vocab) {
    std::map<std::pair<int, int>, int> stats;
    for (const auto& [word, freq] : vocab) {
        for (size_t i = 0; i < word.length()-1; ++i) {
            stats[{word[i], word[i+1]}] += freq;
        }
    }
    return stats;
}
