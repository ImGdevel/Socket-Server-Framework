#include "Server.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        Server server(argc, argv);
        server.run();
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
    return 0;
}
