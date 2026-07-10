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

void updateVocab(std::string text, std::map<std::vector<int>, int> &vocab) {
    auto words = text | std::views::chunk_by([](char a, char b) {
                     return !std::isspace(static_cast<unsigned char>(a)) &&
                            !std::isspace(static_cast<unsigned char>(b));
                 }) |
                 std::views::filter([](auto chunk) {
                     return !std::isspace(static_cast<unsigned char>(chunk[0]));
                 }) |
                 std::views::transform([](auto chunk) {
                     return std::vector<int>(chunk.begin(), chunk.end());
                 }) |
                 std::ranges::to<std::vector<std::vector<int>>>();

    for (const auto &word : words) {
        vocab[word] += 1;
    }
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
    std::ifstream file("example_input/stackexchange.jsonl");
    if (!file.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        return 1;
    }

    std::string input, text;
    std::map<std::vector<int>, int> vocab;
    const int LINE_COUNT = 1000;
    for (size_t i = 0; i < LINE_COUNT; ++i) {
        std::getline(file, input);
        text = readJsonEntry(input).value();
        updateVocab(text, vocab);
    }

    const int max_tokens = 0xFFFF + 5000;
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

    for (int i = 0x10000; i < BPE::getTokenCount(); ++i) {
        std::cout << BPE::getTokenString(i) << " ";
    }
    std::cout << std::endl;

    return 0;
}
