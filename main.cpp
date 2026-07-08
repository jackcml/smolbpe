#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#include "glaze/glaze.hpp"

// {
//     "text": ...,
//     "meta": {"url": "...", "timestamp": "...", "source": "...", "language": "...", ...},
//     "red_pajama_subset": "common_crawl" | "c4" | "github" | "arxiv" | "wikipedia" | "stackexchange"
// }
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

int main() {
    std::ifstream file("example_input/single.json");
    if (!file.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        return 1;
    }

    std::string input;
    std::getline(file, input);

    Entry new_entry{};
    auto error = glz::read_json(new_entry, input);
    if (error) {
       std::string error_msg = glz::format_error(error, input);
       std::cout << error_msg << std::endl;
       return 1;
    }

    std::cout << new_entry.text << std::endl;
    return 0;
}
