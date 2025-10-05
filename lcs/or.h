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

#ifndef __LCS_OR_H__
#define __LCS_OR_H__

#include "module.h"
#include "bus.h"
#include "linestate.h"

namespace lcs
{ // Start of namespace lcs

/// A template class which encapsulates a logical OR gate. The template parameter
/// \p width denotes the number of lines in the input data bus. In other words, the
/// template parameter \p width denotes the number of inputs to the OR gate. Hence,
/// one can instantiate an N-input OR gate by setting the template parameter value
/// to N.
template <int width = 1>
class Or : public Module
{
public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler is practically useless.
    ///
    /// \param output The single line output bus of the gate.
    /// \param input The multiline input bus to the gate.
    /// \param width The number of lines in the input bus.
    ///
    Or(const Bus<1> &output, const InputBus<width> &in1);

    /// Destructor.
    ///
    virtual ~Or();

    /// Propogates the input data line states to the output.
    /// Here, an OR operation is performed on the line states of the data lines of the
    /// input bus, and the result is propogated to the single line output bus.
    virtual void propogate(void);

private:
    Bus<1> outBus;
    InputBus<width> inBus;
};

template <int width>
Or<width>::Or(const Bus<1>& output, const InputBus<width> &in)
                    : Module(), outBus(output), inBus(in)
{
    inBus.drive(this);
}

template <int width>
Or<width>::~Or()
{
    inBus.unDrive(this);
}

template <int width>
void Or<width>::propogate(void)
{
    int w = inBus.width();

    for (int i = 0; i < w; i++)
    {
        if (inBus[i] == HIGH)
        {
            outBus[0] = HIGH;
            return;
        }
    }

    outBus[0] = LOW;
    return;
}

} // End of namespace lcs

#endif // __LCS_OR_H__
