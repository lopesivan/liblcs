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

#ifndef __D_FLIP_FLOP_H__
#define __D_FLIP_FLOP_H__

#include "module.h"
#include "clock.h"
#include "pulsedge.h"

namespace lcs
{

/// This is a class template which encapsulates a D-Flipflop. Initialising as object of
/// using this template requires two template parameters. The first parameter should either
/// be \p lcs::POS_EDGE or \p lcs::NEG_EDGE. \p lcs::POS_EDGE (which is the default)
/// indicates a positive edge triggered flipflop, where as \p lcs::NEG_EDGE indicates a
/// negetive edge triggered flipflop. The second template parameter indicates the propogation
/// delay involved in transeffering the data input to the output of the flipflop when
/// triggered.
template <PulseEdge type = POS_EDGE, unsigned int delay = 0>
class DFlipFlop : public Module
{
public:

    /// A static integer which denotes the port corresponding to the D input to the
    /// flipflop.
    static const int D  ;

    /// A static integer which denotes the port corresponding to the clock input to the
    /// flipflop.
    static const int CLK;

    /// A static integer which denotes the port corresponding to the output to the flipflop.
    ///
    static const int Q  ;

    /// A static integer which denotes the port corresponding to the reset input of the
    /// flipflop.
    static const int RST;

public:

    /// The only explicit constructor. The other constructors, if synthesized by the
    /// compiler, are practically useless and hence their use should be avoided.
    ///
    /// \param QBus The bus connected to the Q output port of the flipflop.
    /// \param DBus The bus connected to the D input port of the flipflop.
    /// \param clock The clock input to the flipflop
    DFlipFlop(const Bus<1> &QBus, const InputBus<1> &DBus, const InputBus<1> &clock);

    /// Destructor.
    ///
    virtual ~DFlipFlop(void);

    /// The function which is triggered by the clock input in case of a positive edge
    /// triggered flipflop. User of the class \p lcs::DFlipFlop will never need to call
    /// this function under normal circumstances.
    virtual void onPosEdge(int portId) { trigger(); }

    /// The function which is triggered by the clock input in case of a negetive edge
    /// triggered flipflop. User of the class \p lcs::DFlipFlop will never need to call
    /// this function under normal circumstances.
    virtual void onNegEdge(int portId) { trigger(); }

private:
    void trigger(void);

    Bus<1> q;
    InputBus<1> d, clk;
};

template <PulseEdge type, unsigned int delay> const int DFlipFlop<type, delay>::D    = 0;
template <PulseEdge type, unsigned int delay> const int DFlipFlop<type, delay>::CLK  = 1;
template <PulseEdge type, unsigned int delay> const int DFlipFlop<type, delay>::Q    = 2;
template <PulseEdge type, unsigned int delay> const int DFlipFlop<type, delay>::RST  = 3;

template <PulseEdge type, unsigned int delay >
DFlipFlop<type, delay>::DFlipFlop(const Bus<1> &QBus, const InputBus<1> &DBus,
                           const InputBus<1> &clock)
                : q(QBus), d(DBus), clk(clock)
{
    if (type == POS_EDGE)
        clk.notify(this, LINE_POS_EDGE, 1);
    else
        clk.notify(this, LINE_NEG_EDGE, 1);

    q.lock(this, delay);
}

template <PulseEdge type, unsigned int delay>
DFlipFlop<type, delay>::~DFlipFlop()
{
    if (type == POS_EDGE)
        clk.stopNotification(this, LINE_POS_EDGE, 1);
    else
        clk.stopNotification(this, LINE_NEG_EDGE, 1);

    d.stopNotification(this, LINE_STATE_CHANGE, 0);
    q.unLock(this);
}

template <PulseEdge type, unsigned int delay>
void DFlipFlop<type, delay>::trigger(void)
{
    typename Bus<1>::LineAccessor ql(q.getLineAccessor(this));

    ql[0] = d[0];

    return;
}

}

#endif // __D_FLIP_FLOP_H__
