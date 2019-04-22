#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

static std::string array(int length) {
    float num = 0.123f;
    std::stringstream ss;
    ss << "[" << num;
    for (int k = 1; k < length; ++k) {
        num += 0.02;
        ss << "," << num;
    }
    ss << "]";
    return ss.str();
}

static void out(const std::vector<int>& dims, int d, const std::string& last)
{
    if (d == dims.size() - 1) {
        std::cout << last;
        return;
    }
    std::cout << "[";
    std::string sep = "";
    for (int k = 0; k < dims[d]; ++k) {
        std::cout << sep;
        sep = ",";
        out(dims, d + 1, last);
    }
    std::cout << "]";
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Usage: ndarray n1 [n2 [n3 ...]]]\n";
        exit(1);
    }
    std::vector<int> dims;
    for (int k = 1; k < argc; ++k) {
        dims.push_back(atoi(argv[k]));
        if (dims.back() <= 0) {
            std::cerr << "Bad argument or <= 0: " << argv[k] << std::endl;
            exit(2);
        }
    }
    std::string arr = array(dims.back());
    out(dims, 0, arr);
    std::cout << '\n';
    return 0;
}
