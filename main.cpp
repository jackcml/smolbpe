#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>

#include "bpe.hpp"
#include "glaze/glaze.hpp" // IWYU pragma: keep

// clang-format off
// {
//     "text": ...,
//     "meta": {"url": "...", "timestamp": "...", "source": "...", "language": "...", ...},
//     "red_pajama_subset": "common_crawl" | "c4" | "github" | "arxiv" | "wikipedia" | "stackexchange"
// }
// clang-format on
struct Meta {
    std::string url;
    std::string timestamp;
    std::string source;
    std::string language;
    std::string question_score;
};
struct Entry {
    std::string text;
    Meta meta;
    std::string red_pajama_subset;
};

std::map<std::vector<int>, int> getVocab(std::string text) {
    auto words =
        text | std::views::split(' ') |
        std::views::filter([](auto &&subrange) { return !subrange.empty(); }) |
        std::ranges::to<std::vector<std::vector<int>>>();

    std::map<std::vector<int>, int> vocab;
    for (const auto &word : words) {
        vocab[word] += 1;
    }

    return vocab;
}

std::optional<std::string> readJsonEntry(std::string input) {
    Entry new_entry{};
    auto error = glz::read_json(new_entry, input);
    if (error) {
        std::string error_msg = glz::format_error(error, input);
        std::cout << error_msg << std::endl;
        return std::nullopt;
    }

    return new_entry.text;
}

int main() {
    std::ifstream file("example_input/single.json");
    if (!file.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        return 1;
    }
    std::string input;
    std::getline(file, input);

    std::string text = readJsonEntry(input).value();
    auto vocab = getVocab(text);

    auto numMerges = 10;
    for (int i = 0; i < numMerges; ++i) {
        auto pair_counts = BPE::getStats(vocab);
        auto best = std::max_element(pair_counts.begin(), pair_counts.end(),
                                     [](const auto &a, const auto &b) {
                                         return a.second < b.second;
                                     })
                        ->first;
        std::cout << "Merging `" << best.first << "," << best.second << "`"
                  << std::endl;
        vocab = BPE::mergeVocab(best, vocab);
    }

    return 0;
}
