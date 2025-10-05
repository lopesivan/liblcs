//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the libLCS library.
// libLCS is C++ Logic Circuit Simulation library.
//
// Copyright (c) 2006-2007, B. R. Siva Chandra, India
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// In case you would like to contact the author, use the following e-mail
// address: sivachandra_br@yahoo.com
//////////////////////////////////////////////////////////////////////////////////

#ifndef __LCS_TESTER_H__
#define __LCS_TESTER_H__

#include "bus.h"
#include <iostream>
#include <cmath>

namespace lcs
{ // Start of namespace lcs

/// A utility class to test a logic circuit. This class is designed under the assumption
/// that every logic circuit has a well defined set of input data lines encaspulated into
/// a data bus, and a well defined set of out lines encapsulated into another data bus.
/// All possible input data combinations are fed into the circuit and the corresponding
/// outpit bus bits are displayed on the standard output (or display) device.
///
/// \param inBits The number of lines in the input bus.
/// \param inBits The number of lines in the output bus.
///
template <int inBits, int outBits>
class Tester
{
public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler is practically useless.
    ///
    /// \param outBus The output bus of the circuit to be tested.
    /// \param inBus The input bus to the circuit to be tested.
    /// \param inBits The number of lines in the input bus.
    /// \param outBits The number of lines in the output bus.
    Tester(const Bus<inBits> &inBus, const Bus<outBits> &outBus);

    /// Runs the test and displays the results on the stdout device.
    ///
    void run(void);

private:
    Bus<inBits> input;
    Bus<outBits> output;
};

template <int inBits, int outBits>
Tester<inBits, outBits>::Tester(const Bus<inBits> &inBus, const Bus<outBits> &outBus)
                        : input(inBus), output(outBus)
{}

template <int inBits, int outBits>
void Tester<inBits, outBits>::run(void)
{
    std::cout << "\nTest started...\n\n";

    for (int j = 0; j < inBits; j++)
        std::cout << "-";

    std::cout << "----|----";

    for (int j = 0; j < outBits; j++)
        std::cout << "-";

    std::cout << "\n";

    int range = 1 << inBits;
    for (int i = 0; i < range; i++)
    {
        input = i;
        std::cout << input << "    |    " << output << "\n";

        for (int j = 0; j < inBits; j++)
            std::cout << "-";

        std::cout << "----|----";

        for (int j = 0; j < outBits; j++)
            std::cout << "-";

        std::cout << "\n";
    }

    std::cout << "\nTest performed successfully. The results should be verified!\n\n";
}

} // End of namespace lcs

#endif // __LCS_TESTER_H__
