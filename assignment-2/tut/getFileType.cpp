#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[]){

    if(argc != 2){
        std::cout << "Wrong arguments!" << std::endl;
        return -1;
    }

    std::string path = argv[1];
    const std::string fileCommandBase = "file -b ";

    std::string command = fileCommandBase + path;

    FILE *fp = popen(command.c_str(), "r");

    if(fp == NULL){
        perror("Popen error");
        return -1;
    }

    char buffer[1024];
    while(fgets(buffer, 1024, fp) != NULL){
        std::cout << buffer;
    }

    std::stringstream ss;
    ss << buffer;
    std::string firstSubString;
    std::getline(ss, firstSubString, ',');
    std::cout << firstSubString << std::endl;


}