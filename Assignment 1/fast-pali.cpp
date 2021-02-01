#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>

// split string p_line into a vector of strings (words)
// the delimiters are 1 or more whitespaces
std::vector<std::string>
split(const std::string &p_line)
{
    auto line = p_line + " ";
    std::vector<std::string> res;
    bool in_str = false;
    std::string curr_word = "";
    for (auto c : line)
    {
        if (isspace(c))
        {
            if (in_str)
                res.push_back(curr_word);
            in_str = false;
            curr_word = "";
        }
        else
        {
            curr_word.push_back(c);
            in_str = true;
        }
    }
    return res;
}

// global variable used in stdin_readline()
char buffer[1024 * 1024 * 10];

// reads in a line from STDIN
// reads until \n or EOF and result includes \n if present
// returns empty string on EOF
std::string
stdin_read()
{
    std::string result;

    unsigned int size = read(STDIN_FILENO, buffer, sizeof(buffer));
    // printf("%d / %d\n", size, sizeof(buffer));
    result.append(buffer);

    // End of file, return empty string
    if (size <= 0)
        return "";

    // If the string end is not a space
    while (!isspace(result.back()))
    {
        // Read the next character from file (avoid cutting off words)
        char buffer2;
        unsigned int size2 = read(STDIN_FILENO, &buffer2, 1);

        // The file has ended, then break and return
        if (size2 <= 0)
            break;

        // Append next character to result string
        result.push_back(buffer2);

        // If buffer2 is a space, then break and return
        if (isspace(buffer2))
            break;
    }

    return result;
}

// returns true if a word is palindrome
// palindrome is a string that reads the same forward and backward
//    after converting all characters to lower case
bool is_palindrome(const std::string &s)
{
    for (size_t i = 0; i < s.size() / 2; i++)
        if (tolower(s[i]) != tolower(s[s.size() - i - 1]))
            return false;
    return true;
}

// returns the longest palindrome on standard input
// in case of ties for length, returns the first palindrome
// all input is broken into words, and each word is checked
// word is a sequence of characters separated by white space
// white space is whatever isspace() says it is
//    i.e. ' ', '\n', '\r', '\t', '\n', '\f'
std::string
get_longest_palindrome()
{
    std::string max_pali;
    while (1)
    {
        std::string line = stdin_read();
        if (line.size() == 0)
            break;
        auto words = split(line);
        for (auto word : words)
        {
            if (word.size() <= max_pali.size())
                continue;
            if (is_palindrome(word))
            {
                // std::cout << word << "\n";
                max_pali = word;
            }
        }
    }
    return max_pali;
}

int main()
{
    std::string max_pali = get_longest_palindrome();
    printf("Longest palindrome: %s\n", max_pali.c_str());
    return 0;
}
