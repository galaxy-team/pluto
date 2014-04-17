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

int main(int argc, char** argv)
{
    /// DECLARE INPUT AND OUTPUT FILENAMES.
    std::string in;
    std::string out;

    /// GET INPUT AND OUTPUT FILENAMES FROM COMMAND LINE ARGUMENTS
    // Test that we have the right number of arguments
    if (argc <= 1 || argc >= 4) {
        std::cerr << "Error: Invalid usage. Usage is `pluto inputfile outputfile`"  << std::endl;
        return -1;
    } else {
        // The input filename is always the first argument.
        in = argv[1];

        // If the output filename is unspecified, use a modified form of
        // the input filename.
        //
        // e.g. "pluto boot.asm" is equivalent to "pluto boot.asm boot.o"
        if (argc == 2) {
            // Strip the last file extension, and replace with ".o"
            out = in.substr(0, in.find_last_of('.')) + ".o";
        }
        // Otherwise, we use the second argument as the output filename.
        else {
            out = argv[2];
        }
    }

    /// READ IN THE INPUT FILE - sorry about the mess
    std::ifstream inf(in);
    
    if (!inf.good()) {
        std::cerr << "Bad file" << std::endl;
        return -1;
    }

    auto object_file = galaxy::asteroid_belt::read_obj(inf);

    inf.close();

    /// LINK THE OBJECT CODE
    std::vector<std::uint16_t> binary = galaxy::pluto::link(std::vector<galaxy::asteroid>{object_file});

    /// WRITE OUT TO OUTPUT FILE
    std::ofstream outf(out);

    galaxy::asteroid_belt::write_uint16_t(outf, binary.size());
    for (std::uint16_t byte : binary) {
        galaxy::asteroid_belt::write_uint16_t(outf, byte);
    }

    outf.close();
}
