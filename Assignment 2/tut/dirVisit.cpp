#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <stack>

int main(int argc, char *argv[]){

    if(argc != 2){
        std::cout << "Wrong arguments!" << std::endl;
        return -1;
    }

    std::string rootPath = argv[1];

    std::stack<std::string> stack;
    stack.push(rootPath);

    while(!stack.empty()){
        std::string path = stack.top();
        stack.pop();

        std::cout << path << std::endl;

        DIR *dirStream = opendir(path.c_str());
        if(dirStream != NULL){
            while(true){
                struct dirent *de = readdir(dirStream);
                if(de == NULL)
                    break;

                std::string entityName = de -> d_name;

                if(entityName == "." || entityName == "..")
                    continue;
                
                std::string newPath = path + "/" + entityName;
                stack.push(newPath);
            }
            closedir(dirStream);
        }
    }
    return 0;
}
