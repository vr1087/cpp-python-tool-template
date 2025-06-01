#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <fstream>

TEST_CASE("Count dummy SAM records") {
    // prepare a small SAM-like file
    std::ofstream f("test.sam");
    f << "@HD" << std::endl;
    f << "read1" << std::endl;
    f << "read2" << std::endl;
    f.close();

    // run the linecount executable
    int rc = std::system("./linecount test.sam > out.txt");
    CHECK(rc == 0);

    std::ifstream in("out.txt");
    int n = 0;
    in >> n;
    CHECK(n == 2);
}