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

#ifndef __LCS_INTOUT_BUS_H__
#define __LCS_INTOUT_BUS_H__

#include "bus.h"

namespace lcs
{ // Start of namespace lcs

/// A template class which encapsulates bus which can drive modules, as well as serve as their
/// outputs.
template <int bits = 1>
class InOutBus : public Bus<bits>
{
public:
    /// Default construtor.
    ///
    InOutBus(void);

    /// Initialised the bus lines two the binary bit equivalent of the integer argument.
    ///
    InOutBus(int val);

    /// Generates an \p InOutBus from a \p lcs::Bus object.
    ///
    InOutBus(const Bus<bits> &b);

    /// Copy constructor.
    ///
    InOutBus(const InOutBus<bits> &b);

    /// Destructor.
    ///
    virtual ~InOutBus();

    /// Overloaded operator which returns a suitable expression object corresponding
    /// to the line at index \p i. This is done so that bit-selects can be used in expressions of
    /// bitwise operations. An \p OutOfRangeException is thrown if the index value \p i is beyond
    /// the bus width.
    inline InOutBus<1> operator[](unsigned int i)
    {
        return InOutBus<1>(Bus<bits>::operator[](i));
    }

    /// Assigns the binary bit equivalent of an integer to the bus lines.
    ///
    void operator=(int a)
    {
        Bus<bits>::operator=(a);
    }

    /// Assigns a desired linestate to all the lines of the bus.
    ///
    void operator=(const LineState &l)
    {
        Bus<bits>::operator=(l);
    }

    /// Assignment operator to facilitate assignment using an \p lcs::Expression object.
    /// ie., the operator facilitates assignment with an expression of bitwise operations
    /// on the right hand side of the assignment operator.
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    void operator=(const Expression<w, Type, LExprType, RExprType> &expr)
    {
        Bus<bits>::operator=(expr);
    }

    ///
    ///
    template <int w>
    void operator=(const InputBus<w> &bus)
    {
        Bus<bits>::operator=(w);
    }

    ///
    ///
    template <int w>
    void operator=(const Bus<w> &bus)
    {
        Bus<bits>::operator=(bus);
    }

    ///
    ///
    void operator=(const InOutBus<bits> &bus);

    /// Assignment operator for assignment with a delay-expression pair.
    /// An example of usage of this overloaded assignment operator is as follows:
    ///
    /// <pre>
    /// lcs::Bus<3> b1(0), b2(5), b3(7);
    /// b1 = (5, b2 ^ b3);
    /// </pre>
    ///
    /// In the second line of the above code snippet, an assignment is being made to the
    /// \p lcs::Bus object on the LHS with a delay-expression pair in the RHS. The value of
    /// delay used here is 5 system time units. Note the use of the <b> neccessary </b>
    /// parentheses enclosing the delay-expression pair.
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    void operator=(const DelayExprPair<w, Type, LExprType, RExprType> &dep)
    {
        Bus<bits>::operator=(dep);
    }

    ///
    ///
    template <int w>
    void operator=(const DelayBusPair<w> &dbp)
    {
        Bus<bits>::operator=(dbp);
    }

    /// Assignment operator for assignment with a delay-state pair.
    /// An example of usage of this overloaded assignment operator is as follows:
    ///
    /// <pre>
    /// lcs::Bus<3> b1;
    /// b1 = (5, lcs::HIGH);
    /// </pre>
    ///
    /// In the second line of the above code snippet, an assignment is being made to the
    /// \p lcs::Bus object on the LHS with a delay-state pair in the RHS. The value of
    /// delay used here is 5 system time units. Note the use of the <b> neccessary </b>
    /// parentheses enclosing the delay-state pair.
    void operator=(const DelayStatePair &dsp)
    {
        Bus<bits>::operator=(dsp);
    }

    /// The overloaded operator to join data lines from two busses and form a new composite
    /// bus. The right operand bus takes the MSB locations.
    ///
    /// \param bits The width of the left-operand bus
    /// \param w The width of the right operand bus
    /// \param bus The right operand \p lcs::Bus object.
    ///
    template <int w>
    const InOutBus<w+bits> operator,(const InOutBus<w> &bus) const
    {
        return InOutBus<w+bits>(Bus<bits>::operator,(bus));
    }

    /// The overloaded operator to join a data line to a bus to form a new composite bus.
    /// The line to be joined will have to be the right operand. The joined line takes
    /// the MSB location in the resulting \p lcs::Bus object.
    ///
    /// \param line The right operand \p lcs::Line object.
    ///
    const InOutBus<bits+1> operator,(const Line &line) const
    {
        return InOutBus<1+bits>(Bus<bits>::operator,(line));
    }

    using Bus<bits>::operator,;
    using InputBus<bits>::operator[];
    using InputBus<bits>::toInt;
    using InputBus<bits>::toStr;
    using InputBus<bits>::notify;
    using InputBus<bits>::stopNotification;
};

template <int bits>
InOutBus<bits>::InOutBus(void)
    : Bus<bits>()
{}

template <int bits>
InOutBus<bits>::InOutBus(int val)
    : Bus<bits>(val)
{}

template <int bits>
InOutBus<bits>::InOutBus(const InOutBus<bits> &b)
    : Bus<bits>(b)
{}

template <int bits>
InOutBus<bits>::InOutBus(const Bus<bits> &b)
    : Bus<bits>(b)
{}

template <int bits>
InOutBus<bits>::~InOutBus()
{}

template <int bits>
void InOutBus<bits>::operator=(const InOutBus<bits> &bus)
{
    Line *data = InOutBus<bits>::dataPtr->data;
    for (unsigned int i = 0; i < bits; i++)
            data[i] = bus.get(i);
}

} // End of namespace lcs

#endif // __LCS_INTOUT_BUS_H__
