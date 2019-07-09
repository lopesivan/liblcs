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

#ifndef __LCS_COUNTER_H__
#define __LCS_COUNTER_H__

#include "bus.h"
#include "module.h"
#include "linevent.h"

namespace lcs
{ // Start of namespace lcs

/// A class which encapsulates a counter module. This counter module increments
/// the count on its output bus when a line event occurs on its input bus. The 
/// line event can be of any type. 
///
/// \param bits The bit-width of the counter
/// \param event The event on the input bus which triggers the counter
template <unsigned int bits, LineEvent event = LINE_POS_EDGE>
class Counter : public Module
{
public:

    /// Constructor
    ///
    /// \param output The output bus of the counter
    /// \param clk The input bus of the counter. 
    Counter(const Bus<bits> &output, const InputBus<1> &clk);

    /// Destructor
    ///
    virtual ~Counter();

    virtual void onPosEdge(int portId) { incr(); }

    virtual void onNegEdge(int portId) { incr(); }

    virtual void onStateChange(int portId) { incr(); }

private:
    Bus<bits> _out;
    InputBus<1> _clk; 
    unsigned int count, max;

    void incr(void);
};

template <unsigned int bits, LineEvent event>
Counter<bits, event>::Counter(const Bus<bits> &output, const InputBus<1> &clk)
       : Module(), _out(output), _clk(clk)
{
    count = 0;
    max = (1 << bits) - 1;
    _out = 0;

    if (event == LINE_POS_EDGE)
        _clk.notify(this, LINE_POS_EDGE, 0);
    else if (event == LINE_NEG_EDGE)
        _clk.notify(this, LINE_NEG_EDGE, 0);
    else
        _clk.notify(this, LINE_STATE_CHANGE, 0);
}

template <unsigned int bits, LineEvent event>
Counter<bits, event>::~Counter()
{
    if (event == LINE_POS_EDGE)
        _clk.stopNotification(this, LINE_POS_EDGE, 0);
    else if (event == LINE_NEG_EDGE)
        _clk.stopNotification(this, LINE_NEG_EDGE, 0);
    else
        _clk.stopNotification(this, LINE_STATE_CHANGE, 0);
}

template <unsigned int bits, LineEvent event>
void Counter<bits, event>::incr(void)
{
    if (count <= max)
        _out = count;
    else
    {
        count = 0;
        _out = 0;
    }

    count++;
}

} // End of namespace lcs

#endif // __LCS_COUNTER_H__

