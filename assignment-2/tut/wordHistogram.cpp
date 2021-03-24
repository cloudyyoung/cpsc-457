#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>            // std::transform, ::tolower
#include <vector>
#include <unordered_map>
#include <cctype>


bool comparePairByInt(std::pair<std::string, int> &a, std::pair<std::string, int> &b){
    return a.second > b.second;
}

std::vector<std::string> getWords(std::string &str){
    std::vector<std::string> result;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    
    std::string word;
    for(char &ch : str){
        if(!std::isalpha(ch)){
            if(word.size() == 0){
                continue;
            }
            result.push_back(word);
            word.clear();
        }else{
            word.push_back(ch);
        }
    }

    // Special case: When the last word contains the last char
    // Because getline() discards the new line ('\n') character and EOF character
    // We need to do a special check here
    if(word.size() != 0)
        result.push_back(word);

    return result;
}

int main(int argc, char *argv[]){

    const int N = 5;

    if(argc != 2){
        std::cout << "Wrong arguments!" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];
    std::ifstream file(filePath);

    std::string line;
    std::unordered_map<std::string, int> histogram;

    if(file.is_open()){
        while(std::getline(file, line)){
            for(std::string word : getWords(line)){
                histogram[word]++;
            }
        }
    }else{
        std::cout << "Unable to open file: " + filePath << std::endl;
        return -1;
    }

    file.close();

    std::vector< std::pair<std::string, int> > vec;

    for(auto &entry : histogram)
        vec.emplace_back(entry);

    if(vec.size() > N){
        std::partial_sort(vec.begin(), vec.begin() + N, vec.end(), comparePairByInt);
        vec.resize(N);
    }else{
        std::sort(vec.begin(), vec.end(), comparePairByInt);
    }

    for(auto &entry : vec){
        std::cout << entry.first << " : " << entry.second << std::endl; 
    }

}