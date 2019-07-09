//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the libLCS library.
// libLCS is a C++ Logic Circuit Simulation library.
//
// Copyright (c) 2006, 2007, 2008 B. R. Siva Chandra
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

#ifndef __LCS_SHIFT_REG_H__
#define __LCS_SHIFT_REG_H__

#include "bus.h"
#include "module.h"
#include "shift.h"
#include "pulsedge.h"

namespace lcs
{ // Start of namespace lcs

/// This class encapsulates a real hardware-like shift register element. It is a template
/// class which takes four template arguments. The shift register module requries an input 
/// bit-stream, and a clock input.
///
/// \param bits The number of bits in the register.
/// \param edgeType The edge type at which triggers a shift.
/// \param shift This template argument denotes whether a left shift, or a right shift has 
/// to be performed.
/// \param delay The delay with which the shift should be performed after the
/// triggering edge occurs at the clock input. This can considered as a propogation
/// delay.
template <unsigned int bits = 1, PulseEdge edgeType = POS_EDGE, Shift shift = LEFT_SHIFT, unsigned int delay = 0>
class ShiftReg : public Module
{
public:
    /// Constructor. The only usefull constructor.
    ///
    /// \param out The output bus, or the read bus, of the register.
    /// \param in The input stream to the shift register.
    ShiftReg(const Bus<bits> &out, const InputBus<1> &in, const InputBus<1> &clk);

    /// Destructor.
    ///
    virtual ~ShiftReg();

    /// Internal function which will never be used by a user of libLCS.
    ///
    virtual void onPosEdge(int portId) { onEdge(); }

    /// Internal function which will never be used by a user of libLCS.
    ///
    virtual void onNegEdge(int portId) { onEdge(); }

    /// An overloaded read only \p operator[] which can be used to read the
    /// line state of the i-th bit of the register.
    const LineState operator[](const int &i) { return out_.get(i); }

private:
    Bus<bits> out_;
    InputBus<1> in_;
    InputBus<1> clk_;

    void onEdge(void)
    {
        if (shift == LEFT_SHIFT)
        {
            out_[0] = (delay, in_[0]);
            for (int i = 0; i < bits-1; i++)
                out_[i+1] = (delay, out_[i]);
        }
        
        if (shift == RIGHT_SHIFT)
        {
            out_[bits-1] = (delay, in_[0]);
            for (int i = bits-2; i >= 0; i--)
                out_[i] = out_[i+1];
        }
    }

};

template <unsigned int bits, PulseEdge edgeType, Shift shift, unsigned int delay>
ShiftReg<bits, edgeType, shift, delay>::ShiftReg(const Bus<bits> &out,
                                          const InputBus<1> &in, const InputBus<1> &clk)
                                : Module(), out_(out), in_(in), clk_(clk)
{
    if (edgeType == POS_EDGE)
        clk_.notify(this, LINE_POS_EDGE, 0);
    if (edgeType == NEG_EDGE)
        clk_.notify(this, LINE_NEG_EDGE, 0);
}

template <unsigned int bits, PulseEdge edgeType, Shift shift, unsigned int delay>
ShiftReg<bits, edgeType, shift, delay>::~ShiftReg()
{
    if (edgeType == POS_EDGE)
        clk_.stopNotification(this, LINE_POS_EDGE, 0);
    if (edgeType == NEG_EDGE)
        clk_.stopNotification(this, LINE_NEG_EDGE, 0);
}

} // End of namespace lcs

#endif // __LCS_SHIFT_REG_H__
