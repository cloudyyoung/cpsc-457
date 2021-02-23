#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

int main(){
    struct stat sstruct;

    char const *pathToEntity = "wordHistogram.cpp";

    int status = stat(pathToEntity, &sstruct);

    std::cout << "File size (bytes): " << sstruct.st_size << std::endl;
    std::cout << "Is regular file: " << std::boolalpha << S_ISREG(sstruct.st_mode) << std::endl;
    std::cout << "Is directory: " << std::boolalpha << S_ISDIR(sstruct.st_mode) << std::endl;


    return 0;
}