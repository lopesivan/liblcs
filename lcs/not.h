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

#ifndef __LCS_NOT_H__
#define __LCS_NOT_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A class which encapsulates a logical NOT gate. Unlike most other gates in libLCS,
/// the NOT gate can only take a single line input bus. The template parameter \p delay
/// indicates the propogation delay from the input to the output of the NOT gate.
template <unsigned int delay = 0>
class Not : public Module
{
public:

    /// A constant integer which denotes the port id for the input port/bus.
    ///
    static const int INPUT;

    /// A constant integer which denotes the port id for the output bus/port.
    ///
    static const int OUTPUT;

public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler is practically useless.
    ///
    /// \param output The single line output bus of the gate.
    /// \param input The multiline input bus to the gate.
    /// \param width The number of lines in the input bus.
    ///
    Not(const Bus<1> &output, const InputBus<1> &input);

    /// Destructor.
    ///
    virtual ~Not();

    /// Propogates the input data line states to the output. This function is called
    /// by the input bus line to notify the gate of a lcs::LINE_STATE_CHANGE event.
    /// When notified, a NOT operation is performed on the line state of the data line
    /// of the input bus and the result is propogated to the single line output bus.
    virtual void onStateChange(int portId);

private:
    Bus<1> outBus;
    InputBus<1> inBus;
};

template <unsigned int delay> const int Not<delay>::INPUT  = 0;
template <unsigned int delay> const int Not<delay>::OUTPUT = 1;

template <unsigned int delay>
Not<delay>::Not(const Bus<1> &output, const InputBus<1> &input)
    : Module(), outBus(output), inBus(input)
{
    inBus.notify(this, LINE_STATE_CHANGE, INPUT);
    onStateChange(INPUT);
}

template <unsigned int delay>
Not<delay>::~Not()
{
    inBus.stopNotification(this, LINE_STATE_CHANGE, INPUT);
}

template <unsigned int delay>
void Not<delay>::onStateChange(int portId)
{
    if (inBus == UNKNOWN || inBus == LOW || inBus == HIGH_IMPEDENCE)
        outBus = (delay, HIGH);
    else
        outBus = (delay, LOW);
}

} // End of namespace lcs

#endif // __LCS_NOT_H__
