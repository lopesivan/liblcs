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

#ifndef __LCS_NOT_H__
#define __LCS_NOT_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A class which encapsulates a logical NOT gate. Unlike most other gates in libLCS,
/// the NOT gate can only take a single line input bus.
class Not : public Module
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
    Not(const Bus<1> &output, const InputBus<1> &input);

    /// Destructor.
    ///
    virtual ~Not();

    /// Propogates the input data line state to the output.
    /// Here, a NOT operation is performed on the line state of the data line of the
    /// input bus, and the result is propogated to the single line output bus.
    virtual void propogate(void);

private:
    Bus<1> outBus;
    InputBus<1> inBus;
};

} // End of namespace lcs

#endif // __LCS_NOT_H__
