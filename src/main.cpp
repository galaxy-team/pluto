/*

This file is part of pluto.

pluto is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pluto is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pluto.  If not, see <http://www.gnu.org/licenses/>.

Your copy of the GNU General Public License should be in the
file named "LICENSE.txt".

*/

#include <cstdint>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <libasteroid.hpp>
#include <libpluto.hpp>

/* third party */
#include "OptionParser.h"

std::vector<galaxy::asteroid> read_in_files(std::vector<std::string> filenames) {
    std::vector<galaxy::asteroid> asteroids;

    for (auto filename : filenames) {
        std::ifstream inf(filename);

        if (!inf.good()) {
            std::cerr << "Could not open input file: " << filename << std::endl;
            exit(-1);
        }

        asteroids.push_back(
            galaxy::asteroid_belt::read_obj(inf)
        );

        inf.close();
    }

    return asteroids;
}

void write_out_binary(std::string filename, std::vector<std::uint16_t> binary) {
    std::ofstream outf(filename);

    if (!outf.good()) {
        std::cerr << "Could not open output file: " << filename << std::endl;
        exit(-1);
    }

    galaxy::asteroid_belt::write_uint16_t(outf, binary.size());
    for (std::uint16_t byte : binary) {
        galaxy::asteroid_belt::write_uint16_t(outf, byte);
    }

    outf.close();
}

int main(int argc, char** argv)
{
    // setup the command line argument parser
    optparse::OptionParser parser = optparse::OptionParser()
       .description("Pluto, Galaxy's linker")
       .usage("usage: %prog [options] <input_files> <output_filename>");

    // parse the buggers - Dom
    optparse::Values options = parser.parse_args(argc, argv);
    std::vector<std::string> args = parser.args();

    // Test that we have the right number of arguments
    if (args.empty()) {
        // if no positional (required) arguments were provided, print help and exit
        parser.print_help();
        return -1;
    }

    // declare input and output filenames
    std::vector<std::string> in;
    std::string out;

    // get input and output filenames from command line arguments
    // Test that we have the right number of arguments
    if (args.size() < 2) {
        std::cerr << "Error: You must provide at least one input, and one output file."  << std::endl;
        parser.print_help();
        return -1;
    } else {
        // remove the last argument, as it is the output filename
        out = args.back();
        args.pop_back();

        // The input filenames are all arguments bar the last.
        in = args;
    }

    // load in the object files
    std::vector<galaxy::asteroid> input_files = read_in_files(in);

    // link the object code
    std::vector<std::uint16_t> binary = galaxy::pluto::link(input_files);

    // write out to output file
    write_out_binary(out, binary);
}
