/**
 * @file main.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <iostream>
#include "context.hpp"
#include "parser.hpp"

using namespace jawa;

void show_usage(const char *name)
{
    std::cerr << "usage: " << name << " [--ścieżkaklasy ŚCIEŻKAKLASY] <PLIK_ŹRÓDŁOWY ...>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        show_usage(argv[0]);
        return 1;
    }

    const char *classpath = ".";
    std::vector<const char *> sources;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--ścieżkaklasy") == 0) {
            if (i + 1 >= argc) {
                show_usage(argv[0]);
                return 1;
            }
            classpath = argv[++i];
        } else {
            sources.push_back(argv[i]);
        }
    }

    if (sources.empty()) {
        show_usage(argv[0]);
        return 1;
    }

    Context ctx(classpath);

    for (auto file : sources) {
        FILE *iff = fopen(file, "r");
        if (!iff) {
            std::cerr << "could not open file " << argv[1] << std::endl;
            return 1;
        }

        auto scn = lexer_init(iff);
        parser prs(scn, &ctx);

        prs.parse();

        lexer_shutdown(scn);
    }

    return 0;
}
