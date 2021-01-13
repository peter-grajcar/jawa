#include <iostream>
#include "context.hpp"
#include "parser.hpp"

using namespace jawa;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <source file>" << std::endl;
        return 1;
    }

    context ctx{};

    FILE *iff = fopen(argv[1], "r");
    if (!iff) {
        std::cerr << "could not open file " << argv[1] << std::endl;
        return 1;
    }

    auto scn = lexer_init(iff);
    parser prs(scn, &ctx);

    prs.parse();

    lexer_shutdown(scn);

    return 0;
}
