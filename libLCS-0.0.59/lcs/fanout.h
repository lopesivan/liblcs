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

#ifndef __LCS_FANOUT_H__
#define __LCS_FANOUT_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A class which encapsulates a fanout module. It takes a single-line bus as input and
/// produces a user specified number of copies of the input.
///
/// \param n The number of desired output lines for the input line.
/// \param delay The propogation delay of the module.
template <unsigned int n = 1, unsigned int delay = 0>
class FanOut : public Module
{
public:
    /// The only practical constructor.
    ///
    /// \param out The output bus containing the fanned-out lines.
    /// \param in The single line input bus.
    FanOut(const Bus<n> &out, const InputBus<> &in);

    /// Destructor.
    ///
    ~FanOut();

    /// The work-horse function of the class. A user of libLCS will never need to call
    /// this function explicitly.
    virtual void onStateChange(int portId);

private:
    Bus<n> output;
    InputBus<> input;
};

template <unsigned int n, unsigned int delay>
FanOut<n, delay>::FanOut(const Bus<n> &out, const InputBus<> &in)
         : Module(), output(out), input(in)
{
    input.notify(this, LINE_STATE_CHANGE, 0);
    onStateChange(0);
}

template <unsigned int n, unsigned int delay>
FanOut<n, delay>::~FanOut()
{
    input.stopNotification(this, LINE_STATE_CHANGE, 0);
}

template <unsigned int n, unsigned int delay>
void FanOut<n, delay>::onStateChange(int portId)
{
    for (unsigned int i = 0; i < n; i++)
    {
        output[i] = (delay, input[0]);
    }
}

} // End of namespace lcs

#endif // __LCS_FANOUT_H__
