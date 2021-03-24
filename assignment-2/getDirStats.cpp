/// =========================================================================
/// Written by pfederl@ucalgary.ca in 2020, for CPSC457.
/// =========================================================================
/// You need to edit this file.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, but you need to implement the getDirStats() function as
/// defined in "getDirStats.h".

#include "getDirStats.h"
#include "digester.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stack>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>


static bool is_dir(const std::string& path)
{
    struct stat buff;
    if (0 != stat(path.c_str(), &buff)) return false;
    return S_ISDIR(buff.st_mode);
}

// ======================================================================
// You need to re-implement this function !!!!
// ======================================================================
//
// getDirStats() computes stats about directory a directory
//   dir_name = name of the directory to examine
//   n = how many top words/filet types/groups to report
//
// if successful, results.valid = true
// on failure, results.valid = false
//
Results getDirStats(const std::string& dir_name, int n)
{
    // Initialize res
    Results res;
    res.valid = false;
    res.n_dirs = 0;
    res.n_files = 0;
    res.all_files_size = 0;
    res.largest_file_size = -1;
    res.largest_file_path = "";
    res.duplicate_files.clear();
    res.most_common_types.clear();
    res.most_common_words.clear();

    // Initialize local variables
    std::unordered_map<std::string, int> file_types; // [<file_type, occurences>, ...]
    std::unordered_map<std::string, std::vector<std::string>> duplicate_files; // [<digest, <path, ...>>, ...]
    std::unordered_map<std::string, int> words; // [<word, occurences>, ...]

    // If root path is not directory
    if (!is_dir(dir_name)) return res;

    // Recursively visits all folders & files
    std::stack<std::string> stack;
    stack.push(dir_name);

    while (!stack.empty()) {
        // Take the first element of stack
        std::string path = stack.top();
        stack.pop();

        // printf("%s\n", path.c_str());

        // Get file property
        struct stat property;
        int status = stat(path.c_str(), &property);

        // Fail to get stats
        if (status != 0) return res;

        if (S_ISDIR(property.st_mode) || is_dir(path)) {
            // printf(" - directory\n");

            // Open current directory
            DIR* directory = opendir(path.c_str());

            // If directory successfully opened
            if (directory) {
                while (true) {
                    // Read directory entities
                    struct dirent* de = readdir(directory);
                    if (!de) break;

                    // Entity name
                    std::string name = de->d_name;
                    if (name == "." || name == "..") continue;

                    std::string new_path = path + "/" + name;
                    stack.push(new_path);
                }
                closedir(directory);
            }

            // Fail to open directory
            if (!directory) return res;

            // Cumulative dir amount
            res.n_dirs++;
        } else {
            long file_size = property.st_size;

            // Largesr file & its size
            if (res.largest_file_size < file_size) {
                res.largest_file_size = file_size;
                res.largest_file_path = path;
            }

            // Cumulative file size
            res.all_files_size += file_size;

            // Cumulative file amount
            res.n_files++;

            // Get file type
            // Code cited from TA Zilin's Week 4 Assignment 2 Discussions: getFileType.cpp, with small changes
            const std::string command = "file -b " + path;  // Citation BEGIN

            FILE* fp = popen(command.c_str(), "r");
            if (!fp) return res;

            char buffer[64];
            fgets(buffer, sizeof(buffer), fp);

            std::stringstream ss;
            ss << buffer;
            std::string file_type;
            std::getline(ss, file_type, ',');               // Citation END

            pclose(fp);

            // Trim trailing new line character
            if (file_type.back() == '\n') {
                file_type.erase(file_type.size() - 1);
            }

            // Record occurence in the map
            if (!file_type.empty()) {
                file_types[file_type]++;
            }

            // Record duplicate files
            std::string sha256 = sha256_from_file(path);
            duplicate_files[sha256].emplace_back(path);

            // Read file words
            std::ifstream file(path);
            std::string line;

            if (!file.is_open()) return res;

            while (std::getline(file, line)) {
                std::string word;
                line.push_back(' ');
                std::transform(line.begin(), line.end(), line.begin(), ::tolower);

                for (char& each : line) {
                    if (std::isalpha(each)) {
                        word.push_back(each);
                    } else {
                        if (word.size() >= 3) words[word]++;
                        word.clear();
                    }
                }
            }
            file.close();

            // printf(" - file: %s\n", file_type.c_str());
            // printf(" - size: %ld\n", file_size);
        }

    }

    // Copying file types pair to res, and sort by occurences
    for (auto& file_type : file_types) {
        res.most_common_types.emplace_back(file_type.first, file_type.second);
    }

    // Sorting function
    auto compare_file_types = [](auto& a, auto& b) {
        return a.second > b.second;
    };

    // Sort
    if ((int)(res.most_common_types.size()) > n) {
        std::partial_sort(res.most_common_types.begin(), res.most_common_types.begin() + n, res.most_common_types.end(), compare_file_types);
        res.most_common_types.resize(n);
    } else {
        std::sort(res.most_common_types.begin(), res.most_common_types.end(), compare_file_types);
    }


    // Copying duplicate files to res, and sort by group amount
    for (auto& duplicate_file : duplicate_files) {
        if (duplicate_file.second.size() > 1) {
            res.duplicate_files.emplace_back(duplicate_file.second);
        }
    }

    // Sorting function
    auto compare_duplicate_files = [](auto& a, auto& b) {
        return a.size() > b.size();
    };

    // Sort
    if ((int)(res.duplicate_files.size()) > n) {
        std::partial_sort(res.duplicate_files.begin(), res.duplicate_files.begin() + n, res.duplicate_files.end(), compare_duplicate_files);
        res.duplicate_files.resize(n);
    } else {
        std::sort(res.duplicate_files.begin(), res.duplicate_files.end(), compare_duplicate_files);
    }


    // Copy words to res, and sort by occurences
    for (auto& word : words) {
        res.most_common_words.emplace_back(word.first, word.second);
    }

    // Sorting function
    auto compare_words = [](auto& a, auto& b) {
        return a.second > b.second;
    };

    // Sort
    if ((int)(res.most_common_words.size()) > n) {
        std::partial_sort(res.most_common_words.begin(), res.most_common_words.begin() + n, res.most_common_words.end(), compare_words);
        res.most_common_words.resize(n);
    } else {
        std::sort(res.most_common_words.begin(), res.most_common_words.end(), compare_words);
    }


    res.valid = true;
    res.n_dirs--; // Exclude root directory
    return res;
}