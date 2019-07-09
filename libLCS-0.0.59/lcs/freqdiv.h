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

#ifndef __LCS_FREQ_DIV_H__
#define __LCS_FREQ_DIV_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A module which divides the frequency of the input signal by a specified factor.
/// The factor is specified by the first template parameter. The second template parameter
/// specifies the input to output propogation delay.
template <unsigned int factor, unsigned int delay = 0>
class FrequencyDivider : public Module
{
public:

    /// The only usefull constructor for the class. The default constructor, if synthesised
    /// by the compiler, is practically useless.
    ///
    /// \param output The bus of the module.
    /// \param input The input bus of the module.
    FrequencyDivider(const Bus<1> &output, const InputBus<1> &input);

    /// Destructor.
    ///
    virtual ~FrequencyDivider() {}

    /// Function which toggles the output line state at the suitable positive edge.
    /// A user of libLCS will never need to use this function directly/explicitly.
    virtual void onPosEdge(int portId);

    virtual void onNegEdge(int portId);

private:
    InputBus<1> input_;
    Bus<1> output_;
    int edgeCount_;
};

template <unsigned int factor, unsigned int delay>
FrequencyDivider<factor, delay>::FrequencyDivider(const Bus<1> &output, const InputBus<1> &input)
                : input_(input), output_(output), edgeCount_(factor)
{
    input_.notify(this, LINE_POS_EDGE, 0);
    input_.notify(this, LINE_NEG_EDGE, 0);

    output_[0] = LOW;
}

template <unsigned int factor, unsigned int delay>
void FrequencyDivider<factor, delay>::onPosEdge(int portId)
{
    if (edgeCount_ < static_cast<int>(factor))
        edgeCount_++;
    else
    {
        edgeCount_ = 1;
        output_[0] = (delay, ~output_[0]);
    }
}

template <unsigned int factor, unsigned int delay>
void FrequencyDivider<factor, delay>::onNegEdge(int portId)
{
    if (edgeCount_ < static_cast<int>(factor))
        edgeCount_++;
    else
    {
        edgeCount_ = 1;
        output_[0] = (delay, ~output_[0]);
    }
}

} // End of namespace lcs

#endif // __LCS_FREQ_DIV_H__
