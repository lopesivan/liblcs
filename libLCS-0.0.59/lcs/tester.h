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

#ifndef __LCS_TESTER_H__
#define __LCS_TESTER_H__

#include "bus.h"
#include "clock.h"
#include <iostream>
#include <cmath>

namespace lcs
{ // Start of namespace lcs

/// This is a utility class to test a digital circuit. This class can be used to feed inputs
/// to a bus at every clock state change. When a simulation starts, the  instantiated
/// \p lcs::Tester object will feed a different input to a bus (the bus with which it was
/// instantiated) at every clock pulse. The values it feeds range from 0 to 2<sup>w</sup>-1,
/// where w is the width of the bus with which the \p lcs::Tester object was instantiated.
///
/// \param inBits The number of lines in the input bus.
///
template <int inBits>
class Tester : public TickListener
{
public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler is practically useless.
    ///
    /// \param inBus The input bus to the circuit to be tested.
    /// \param inBits The number of lines in the input bus.
    Tester(const Bus<inBits> &inBus);

    /// Destructor.
    ///
    ~Tester();

    /// The function which is used by the \p lcs::Clock object to notify a \p lcs::Tester
    /// object about an occurance of a clock state change.
    virtual void onTick(void);

private:
    Bus<inBits> input;
    int val;
};

template <int inBits>
Tester<inBits>::Tester(const Bus<inBits> &inBus)
                        :  TickListener(), input(inBus), val(0)
{
    Clock::notifyTick(this);
    input = 0;
}

template <int inBits>
Tester<inBits>::~Tester()
{
    Clock::stopTickNotification(this);
}

template <int inBits>
void Tester<inBits>::onTick(void)
{
    int range = (1 << inBits)-1;
    if (val <= range)
    {
        input = val;
        val++;
    }
}

} // End of namespace lcs

#endif // __LCS_TESTER_H__
