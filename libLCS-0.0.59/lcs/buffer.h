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

#ifndef __LCS_BUFFER_H__
#define __LCS_BUFFER_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A template class which encapsulates a module representing a buffer. The first template parameter
/// \p lines denotes the number of lines for which the module acts as a buffer. The second template
/// parameter \p delay denotes the input to ourput propogation delay of the buffer.
///
/// This module can also be used as a delay element by specifying the desired amount of
/// propogation delay.
template <unsigned int lines = 1, unsigned int delay = 0>
class Buffer : public Module
{
public:

    /// The only usefull constructor. The default constructor, if generated by the compiler, is of no
    /// practical use.
    Buffer(const Bus<lines> &output, const InputBus<lines> &input);

    /// Destructor.
    ///
    ~Buffer();

    /// This is the workhorse function of the module class. A user of libLCS will, under normal
    /// circumstances, never need to call this function explicitly.
    virtual void onStateChange(int portId);

private:
    Bus<lines> output_;
    InputBus<lines> input_;
};

template <unsigned int lines, unsigned int delay>
Buffer<lines, delay>::Buffer(const Bus<lines> &output, const InputBus<lines> &input)
             : Module(), output_(output), input_(input)
{
    input_.notify(this, LINE_STATE_CHANGE, 0);

    for (unsigned int i = 0; i < lines; i++)
    {
        output_[i] = input_[i];
    }
}

template <unsigned int lines, unsigned int delay>
Buffer<lines, delay>::~Buffer()
{
    input_.stopNotification(this, LINE_STATE_CHANGE, 0);
}

template <unsigned int lines, unsigned int delay>
void Buffer<lines, delay>::onStateChange(int portId)
{
    for (unsigned int i = 0; i < lines; i++)
    {
        output_[i] = (delay, input_[i]);
    }
}

} // End of namespace lcs

#endif // __LCS_BUFFER_H__
