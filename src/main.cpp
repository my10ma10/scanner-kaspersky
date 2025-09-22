#include "traversal.hpp"

int main(int argc, char** argv) {
    Travelsal t(fs::current_path().parent_path());
    t();
    
    return 0;
}