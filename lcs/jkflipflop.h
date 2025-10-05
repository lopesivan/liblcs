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

#ifndef __JK_FLIP_FLOP_H__
#define __JK_FLIP_FLOP_H__

#include "module.h"
#include "pulsedge.h"

namespace lcs
{

/// This is class template which encapsulates a JK flipflop.The template parameter
/// should either be lcs::POS_EDGE or lcs::NEG_EDGE. lcs::POS_EDGE (which is the default)
/// indicates a positive edge triggered flipflop, where as lcs::NEG_EDGE indicates a
/// negetive edge triggered flipflop.
template <PulseEdge type = POS_EDGE, unsigned int delay = 0>
class JKFlipFlop : public Module
{
public:

    /// A static integer id which denotes the port corresponding to the J input.
    ///
    static const int J;

    /// A static integer id which denotes the port corresponding to the K input.
    ///
    static const int K;

    /// A static integer id which denotes the port corresponding to the clock input.
    ///
    static const int CLK;

    /// A static integer id which denotes the port corresponding to the output of the
    /// flipflop.
    static const int Q;

    /// A static integer id which denotes the port corresponding to the reset input.
    ///
    static const int RST;

public:

    /// The only explicit constructor.The other constructors, if synthesized by the
    /// compiler, are practically useless and hence their use should be avoided.
    ///
    /// \param QBus The single line output bus of the flipflip.
    /// \param JBus The single line J input bus of the flipflop.
    /// \param KBus The single line K input bus of the flipflop.
    /// \param clock The clock input to the flipflop.
    JKFlipFlop(const Bus<1> &QBus, const InputBus<1> &JBus, const InputBus<1> &KBus,
               const InputBus<1> &clock);

    /// Destructor.
    ///
    virtual ~JKFlipFlop(void);

    /// The function which is triggered by the clock input in case of a positive edge
    /// triggered flipflop. User of the class \p lcs::JKFlipFlop will never need to call
    /// this function under normal circumstances.
    virtual void onPosEdge(int portId) { trigger(); }

    /// The function which is triggered by the clock input in case of a negetive edge
    /// triggered flipflop. User of the class \p lcs::JKFlipFlop will never need to call
    /// this function under normal circumstances.
    virtual void onNegEdge(int portId) { trigger(); }

private:
    void trigger(void);

    Bus<1> q;
    InputBus<1> j, k, clk;
};

template <PulseEdge type, unsigned int delay> const int JKFlipFlop<type, delay>::J   = 0;
template <PulseEdge type, unsigned int delay> const int JKFlipFlop<type, delay>::K   = 1;
template <PulseEdge type, unsigned int delay> const int JKFlipFlop<type, delay>::Q   = 2;
template <PulseEdge type, unsigned int delay> const int JKFlipFlop<type, delay>::CLK = 3;
template <PulseEdge type, unsigned int delay> const int JKFlipFlop<type, delay>::RST = 4;

template <PulseEdge type, unsigned int delay>
JKFlipFlop<type, delay>::JKFlipFlop(const Bus<1> &QBus, const InputBus<1> &JBus,
                             const InputBus<1> &KBus, const InputBus<1> &clock)
                : q(QBus), j(JBus), k(KBus), clk(clock)
{
    if (type == POS_EDGE)
        clk.notify(this, LINE_POS_EDGE, 1);
    else
        clk.notify(this, LINE_NEG_EDGE, 1);
}

template <PulseEdge type, unsigned int delay>
JKFlipFlop<type, delay>::~JKFlipFlop()
{
    if (type == POS_EDGE)
        clk.stopNotification(this, LINE_POS_EDGE, 1);
    else
        clk.stopNotification(this, LINE_NEG_EDGE, 1);
}

template <PulseEdge type, unsigned int delay>
void JKFlipFlop<type, delay>::trigger(void)
{
    if (j[0] == LOW && k[0] == LOW)
        ; // Leave the output state the same as before.
    else if (j[0] == HIGH && k[0] == HIGH)
        q[0] = (delay, ~q[0]);
    else if (j[0] == LOW && k[0] == HIGH)
        q[0] = (delay, LOW);
    else if (j[0] == HIGH && k[0] == LOW)
        q[0] = (delay, HIGH);

    return;
}

}

#endif // __JK_FLIP_FLOP_H__
