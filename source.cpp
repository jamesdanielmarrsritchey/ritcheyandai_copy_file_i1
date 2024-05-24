#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

enum class FileExistsOption {
    Overwrite,
    Append,
    Keep
};

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char* argv[]) {
    std::string destination_file;
    std::string source_file;
    FileExistsOption file_exists_option = FileExistsOption::Keep;

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "--destination_file" && i + 1 < argc) {
            destination_file = argv[++i];
        } else if(arg == "--source_file" && i + 1 < argc) {
            source_file = argv[++i];
        } else if(arg == "--file_exists" && i + 1 < argc) {
            std::string option = argv[++i];
            if(option == "overwrite") {
                file_exists_option = FileExistsOption::Overwrite;
            } else if(option == "append") {
                file_exists_option = FileExistsOption::Append;
            } else if(option == "keep") {
                file_exists_option = FileExistsOption::Keep;
            } else {
                std::cerr << "Unknown file_exists option: " << option << "\n";
                return 1;
            }
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    if(destination_file.empty() || source_file.empty()) {
        std::cerr << "Usage: " << argv[0] << " --destination_file <filename> --source_file <filename> [--file_exists <overwrite|append|keep>]\n";
        return 1;
    }

    if(file_exists_option == FileExistsOption::Keep && file_exists(destination_file)) {
        return 1;
    }

    std::ifstream source(source_file);
    if(!source) {
        std::cerr << "Error opening source file " << source_file << "\n";
        return 1;
    }

    std::ofstream destination;
    if(file_exists_option == FileExistsOption::Append) {
        destination.open(destination_file, std::ios_base::app);
    } else {
        destination.open(destination_file);
    }

    if(!destination) {
        std::cerr << "Error opening destination file " << destination_file << "\n";
        return 1;
    }

    destination << source.rdbuf();
    source.close();
    destination.close();

    return 0;
}