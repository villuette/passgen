#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <unistd.h> // for getopt

int main(int argc, char *argv[]) {
    int length = 12;
    bool strong = false;
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_+=<>?";
    bool verbose = false;
    int opt;
    while ((opt = getopt(argc, argv, "l:svh")) != -1) {
        switch (opt) {
            case 'l':
                try {
                    length = std::stoi(optarg);
                } catch (const std::invalid_argument&) {
                    std::cerr << "Error: Invalid input for -l. Please provide a valid number." << std::endl;
                    return 1;
                } catch (const std::out_of_range&) {
                    std::cerr << "Error: Number out of range for -l." << std::endl;
                    return 1;
                }
                break;
            case 's':
                strong = true;
                break;
            case 'v':
                verbose = true;
                std::cout << "Verbose mode enabled. Each symbol will be printed as it is generated." << std::endl;
                break;
            case 'h':
                std::cerr << "Error: Invalid option or missing argument." << std::endl;
                std::cerr << "Usage: " << argv[0] << " -l <length> [-s] [-v] [-h]" << std::endl;
                std::cout << "  -l <length>  Specify the length of the password (required)" << std::endl;
                std::cout << "  -s           Enable strong mode (include special characters)" << std::endl;
                std::cout << "  -v           Enable verbose mode (print each generated character)" << std::endl;
                std::cout << "  -h           Display this help message" << std::endl;
                std::cout << "This program creates a password with the specified length, where each symbol can't be  \
                            met more than length/5 times and each adjacent pair of symbols is differ to each other" << std::endl;
                return 0;
            default:
                std::cerr << "Usage: " << argv[0] << " -l <length> [-s] [-v] [-h]" << std::endl;
                return 1;
        }
    }


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, strong ? 79 : 61);

    std::vector<char> password(length);
    std::unordered_map<char, int> frequency;
    for (size_t i = 0; i < password.size(); ++i) {
        char c;
        do {
            c = charset[dis(gen)];
        } while (frequency[c] >= length / 5 || (i > 0 && c == password[i - 1]));
        password[i] = c;
        frequency[c]++;
        if (verbose) {
            std::cout << "Generated character: " << c << std::endl;
        }
    }
    
    std::cout << std::string(password.begin(), password.end()) << std::endl;
    return 0;
}
