//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the libLCS library.
// libLCS is C++ Logic Circuit Simulation library.
//
// Copyright (c) 2006-2007, B. R. Siva Chandra
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// In case you would like to contact the author, use the following e-mail
// address: sivachandra_br@yahoo.com
//////////////////////////////////////////////////////////////////////////////////

#ifndef __LCS_FA_H__
#define __LCS_FA_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs.

/// A class which encapsulates a 1-bit full-adder.
///
class FullAdder : public Module
{
public:

    /// A static integer id to denote one of the inputs to the fulladder.
    ///
    static const int A_IN = 0;

    /// A static integer id to denote one of the inputs to the fulladder.
    ///
    static const int B_IN = 1;

    /// A static integer id to denote the carry input to the fulladder.
    ///
    static const int C_IN = 2;

public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler (if at all) is practically useless.
    ///
    /// \param S The single line output \p Bus which carries the sum.
    /// \param Cout The single line output \p Bus which carries the carry output.
    /// \param A The single line \p InputBus object which carries one of the data lines.
    /// \param B The single line \p InputBus object which carries the other data line.
    /// \param Cin The single line \p InputBus object which carries the carry input to the
    /// full-adder.
    ///
    FullAdder(const Bus<1> &S, const Bus<1> Cout, const InputBus<1> &A,
              const InputBus<1> &B, const InputBus<1> &Cin);

    /// Destructor.
    ///
    ~FullAdder();

    /// This function is triggered by the inputs to the fulladder whenever their states
    /// change. When triggered, binary 1-bit addition is performed on the line states of
    /// input busses and the result is propogated to the output busses.
    virtual void onStateChange(int portId);

private:
    Bus<1> s, cout;
    InputBus<1> a, b, c;
};

} // End of namespace lcs.

#endif // __LCS_FA_H__
