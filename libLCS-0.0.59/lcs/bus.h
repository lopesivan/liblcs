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

#ifndef __LCS_BUS_H__
#define __LCS_BUS_H__

#include "inbus.h"
#include "expression.h"
#include "contassmod.h"
#include "hiddenmodman.h"
#include "delbuspair.h"

namespace lcs
{ // Start of namespace lcs.

template <int outbits, typename InExprType, unsigned int delay = 0>
class ContinuousAssignmentModule;

/// This is a class encapsulating a set of data lines to be used as a generic bus in logic
/// circuit systems. The data lines are conceptially organised in the little endian format.
/// A user can set the bus line data through access control mechanism. Reading the individual
/// data lines of a \p Bus object is achieved through the \p lcs::InputBus::operator[]
/// function. There are overloaded operators provided which can be used to join two busses
/// to form a wider composite bus.
///
/// <p>
/// Writing to bus lines should only be allowed for a module which is driving the bus. This
/// sort of access control is provided through the inner class \p LineAccessor, and the three
/// member functions \p Bus::lock, \p Bus::unLock and \p Bus::getLineAccessor.
///
/// \param bits The number of data lines in the bus.
///
template <int bits = 1>
class Bus : public InputBus<bits>
{

/// Bus classes of different sizes are friends of each other.
///
template <int w>
friend class Bus;

public:

    /// The default constructor.
    /// The data lines are all initialised to the \p lcs::UNKNOWN state.
    Bus(void);

    /// Line initialising constructor. The bus lines are initialised in the little-endian
    /// notation with the binary equivalent of a decimal integer. If the integer has
    /// excess bits than the bus width, then they are ignored.
    ///
    /// \param val The decimal integer with which the bus lines have to be initialised.
    Bus(int val);

    /// Copy constructor.
    /// Performs only a shallow copy. The new \p Bus object and \p bus share the same
    /// set of lines.
    ///
    /// \param bus The \p Bus object whose lines are used to create a new \p Bus object.
    ///
    Bus(const Bus<bits> &bus);

    /// Creates a single-line bus object containing the line object which is passed as the
    /// argument. One can only use 1 as the class template argument. Other numbers will lead
    /// to an \p lcs::OutOfRangeException<int>
    ///
    /// \param line The line object which is encapsulated in the resultant bus object.
    Bus(const Line &line) throw(OutOfRangeException<int>);

    /// Destructor.
    ///
    virtual ~Bus();

    /// Returns a part-bus formed from a set of consecutive lines of the original \p lcs::Bus
    /// object. If \p s+w goes beyond the range of the original bus width, then, only the
    /// lines within range are assigned lines of the new bus object.
    ///
    /// \param w The width of the part-bus.
    /// \param s The start bit from where the part-bus should be accumulated.
    template <int w>
    const Bus<w> partSelect(int s) const;

    /// A template function to assign a continuous assignment expression to the bus.
    /// All template parameters except the parameter \p delay are deduced from the
    /// expression passed as an argument to the function. Hence, a call to this function
    /// will have to explicitly specify a single parameter which indicates the assignment
    /// delay.
    template <unsigned int delay, int exBits, ExprType Type,
              typename LExprType, typename RExprType>
    void cass(const Expression<exBits, Type, LExprType, RExprType> &expr);

    /// A template function to assign a continuous assignment to the bus from an
    /// \p lcs::InputBus object. The template parameter \p width is deduced from the width
    /// of the \p lcs::InputBus object passed as an argument to the function. Hence, a call
    /// to this function will have to explicitly specify a single parameter which indicates
    /// the assignment delay.
    template <unsigned int delay, int width>
    void cass(const InputBus<width> &b);

    /// Assigns the binary bit equivalent of an integer to the bus lines.
    ///
    void operator=(int a);

    /// Assigns a desired linestate to all the lines of the bus.
    ///
    void operator=(const LineState &l);

    /// Assignment operator to facilitate assignment using an \p lcs::Expression object.
    /// ie., the operator facilitates assignment with an expression of bitwise operations
    /// on the right hand side of the assignment operator.
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    void operator=(const Expression<w, Type, LExprType, RExprType> &expr);

    ///
    ///
    template <int w>
    void operator=(const InputBus<w> &bus);

    ///
    ///
    template <int w>
    void operator=(const Bus<w> &bus);

    ///
    ///
    void operator=(const Bus<bits> &bus);

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
    void operator=(const DelayExprPair<w, Type, LExprType, RExprType> &dep);

    ///
    ///
    template <int w>
    void operator=(const DelayBusPair<w> &dbp);

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
    void operator=(const DelayStatePair &dsp);

    /// Overloaded operator which returns a suitable expression object corresponding
    /// to the line at index \p i. This is done so that bit-selects can be used in expressions of
    /// bitwise operations. An \p OutOfRangeException is thrown if the index value \p i is beyond
    /// the bus width.
    inline Bus<1> operator[](unsigned int i) throw(OutOfRangeException<unsigned int>);

    /// The overloaded operator to join data lines from two busses and form a new composite
    /// bus. The right operand bus takes the MSB locations.
    ///
    /// \param bits The width of the left-operand bus
    /// \param w The width of the right operand bus
    /// \param bus The right operand \p lcs::Bus object.
    ///
    template <int w>
    const Bus<w+bits> operator,(const Bus<w> &bus) const;

    ///
    ///
    template <int w>
    const InputBus<w+bits> operator,(const InputBus<w> &bus) const;

    /// The overloaded operator to join a data line to a bus to form a new composite bus.
    /// The line to be joined will have to be the right operand. The joined line takes
    /// the MSB location in the resulting \p lcs::Bus object.
    ///
    /// \param line The right operand \p lcs::Line object.
    ///
    const Bus<bits+1> operator,(const Line &line) const;

    using InputBus<bits>::operator[];
    using InputBus<bits>::toInt;
    using InputBus<bits>::toStr;

private:

    /// Does nothing. This function is inherited from \p InputBus. However, it has been
    /// declared private so that a lcs::Module derivative is prevented from using a
    /// \p lcs::Bus object to register to drive a module.
    void notify(Module *mod, const LineEvent &event, const int &portId,
                const int &line = -1) throw(OutOfRangeException<int>)
    {}

    /// Does nothing. This function is inherited from \p InputBus. However, it has been
    /// declared private so that the user is prevented from using a \p Bus object to
    /// de-register to drive (or un-drive) a module.
    void stopNotification(Module *mod, const LineEvent &event, const int &portId,
                          const int &line = -1) throw(OutOfRangeException<int>)
    {}

};

template <int bits>
Bus<bits>::Bus(void)
   : InputBus<bits>()
{}

template <int bits>
Bus<bits>::Bus(const Line &l) throw(OutOfRangeException<int>)
   : InputBus<1>()
{
    if (bits != 1)
        throw OutOfRangeException<int>(1, 1, bits);

    Line *data = Array1D<Line, bits>::dataPtr->data;
    data[0] = l;
}

template <int bits>
Bus<bits>::Bus(int a)
   : InputBus<bits>()
{
    Line *data = InputBus<bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        int b = a%2;
        if (b == 0)
            data[i].setLineValue(LOW);
        else
            data[i].setLineValue(HIGH);

        a = a/2;
    }
}

template <int bits>
Bus<bits>::Bus(const Bus<bits> &bus)
   : InputBus<bits>(bus)
{}

template <int bits>
Bus<bits>::~Bus()
{}

template <int bits>
template <unsigned int delay, int exBits, ExprType Type,
          typename LExprType, typename RExprType>
void Bus<bits>::cass(const Expression<exBits, Type, LExprType, RExprType> &expr)
{
    ContinuousAssignmentModule<
                                bits, Expression<exBits, Type, LExprType, RExprType>, delay
                              > *mod
    = new ContinuousAssignmentModule<
                                bits, Expression<exBits, Type, LExprType, RExprType>, delay
                                    >(*this, expr);

    HiddenModuleManager::registerModule(mod);
}

template <int bits>
template <unsigned int delay, int width>
void Bus<bits>::cass(const InputBus<width> &b)
{
    Expression<width, BUS_EXPR, void, void> expr(b);

    ContinuousAssignmentModule<
                                bits, Expression<width, BUS_EXPR, void, void>, delay
                              > *mod
    = new ContinuousAssignmentModule<
                                bits, Expression<width, BUS_EXPR, void, void>, delay
                                    >(*this, expr);

    HiddenModuleManager::registerModule(mod);

}

template <int bits>
void Bus<bits>::operator=(int value)
{
    Line *data = InputBus<bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        int rem = value % 2;

        if (rem == 1)
            data[i] = HIGH;
        else
            data[i] = LOW;

        value = value >> 1;
    }
}

template <int bits>
void Bus<bits>::operator=(const LineState &l)
{
    Line *data = InputBus<bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        data[i] = l;
    }
}

template <int bits>
template <int w>
void Bus<bits>::operator=(const InputBus<w> &bus)
{
    Line *data = InputBus<bits>::dataPtr->data;
    if (w > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = bus.get[i];
    }
    else
        for (int i = 0; i < w; i++)
            data[i] = bus.get[i];
}

template <int bits>
template <int w>
void Bus<bits>::operator=(const Bus<w> &bus)
{
    Line *data = Bus<bits>::dataPtr->data;
    if (w > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = bus.get(i);
    }
    else
        for (int i = 0; i < w; i++)
            data[i] = bus.get(i);
}

template <int bits>
void Bus<bits>::operator=(const Bus<bits> &bus)
{
    Line *data = Bus<bits>::dataPtr->data;
    for (unsigned int i = 0; i < bits; i++)
            data[i] = bus.get(i);
}

template <int bits>
template <int w, ExprType Type, typename LExprType, typename RExprType>
void Bus<bits>::operator=(const Expression<w, Type, LExprType, RExprType> &expr)
{
    Line *data = InputBus<bits>::dataPtr->data;
    if (w > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = expr[i];
    }
    else
        for (int i = 0; i < w; i++)
            data[i] = expr[i];
}

template <int bits>
template <int w>
void Bus<bits>::operator=(const DelayBusPair<w> &dbp)
{
    Line *data = InputBus<bits>::dataPtr->data;
    InputBus<w> bus(dbp.getBus());
    unsigned int delay = dbp.getDelay();

    if (w > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
        {
            data[i].setAssDelay(delay);
            data[i] = bus.get(i);
        }
    }
    else
        for (int i = 0; i < w; i++)
        {
            data[i].setAssDelay(delay);
            data[i] = bus.get(i);
        }
}

template <int bits>
template <int w, ExprType Type, typename LExprType, typename RExprType>
void Bus<bits>::operator=(const DelayExprPair<w, Type, LExprType, RExprType> &dep)
{
    Line *data = InputBus<bits>::dataPtr->data;
    Expression<w, Type, LExprType, RExprType> expr = dep.getExpr();
    unsigned int delay = dep.getDelay();

    if (w > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
        {
            data[i].setAssDelay(delay);
            data[i] = expr[i];
        }
    }
    else
        for (int i = 0; i < w; i++)
        {
            data[i].setAssDelay(delay);
            data[i] = expr[i];
        }
}

template <int bits>
void Bus<bits>::operator=(const DelayStatePair &dsp)
{
    Line *data = InputBus<bits>::dataPtr->data;
    LineState state = dsp.getState();
    unsigned int delay = dsp.getDelay();

    for (unsigned int i = 0; i < bits; i++)
    {
        data[i].setAssDelay(delay);
        data[i] = state;
    }
}

template <int bits>
Bus<1> Bus<bits>::operator[](unsigned int index) throw(OutOfRangeException<unsigned int>)
{
    if (index >= bits)
        throw OutOfRangeException<unsigned int>(0, static_cast<unsigned int>(bits-1), index);

    Bus<1> b;

    Line *line = InputBus<bits>::dataPtr->data,
         *bline = b.dataPtr->data;

    bline[0] = line[index];
    return b;
}

template <int bits>
template <int w>
const Bus<w+bits> Bus<bits>::operator,(const Bus<w> &bus) const
{
    Bus<w+bits> b;
    Line *data = InputBus<bits>::dataPtr->data,
         *bdata = b.dataPtr->data;
    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    data = bus.dataPtr->data;
    for (int i = 0; i < w; i++)
        bdata[i+bits] = data[i];

    return b;
}

template <int bits>
template <int w>
const InputBus<w+bits> Bus<bits>::operator,(const InputBus<w> &bus) const
{
    Bus<w+bits> b;
    Line *data = InputBus<bits>::dataPtr->data,
         *bdata = b.dataPtr->data;
    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    data = bus.dataPtr->data;
    for (int i = 0; i < w; i++)
        bdata[i+bits] = data[i];

    return b;
}

template <int bits>
const Bus<bits+1> Bus<bits>::operator,(const Line &line) const
{
    Bus<1+bits> b;
    Line *data = InputBus<bits>::dataPtr->data,
         *bdata = b.dataPtr->data;

    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    bdata[bits] = line;

    return b;
}

template <int bits>
template <int w>
const Bus<w> Bus<bits>::partSelect(int s) const
{
    Bus<w> b;

    Line *bdata = b.dataPtr->data, *data = InputBus<bits>::dataPtr->data;
    for (int i = 0; i < w; i++)
    {
        if (i+s < bits)
            bdata[i] = data[i+s];
        else
            break;
    }

    return b;
}

} // End of namespace lcs.

/// \fn
/// The overloaded bitshift operator to display the bus data lines' states in the
/// little-endian format onto the \p std::stdout device.
template <int w>
std::ostream& operator<<(const std::ostream &osObj, const lcs::Bus<w> &bus)
{
    for (int i = 0; i < w-1; i++)
    {
        if (bus[w-i-1] == lcs::LOW)
            std::cout << "0";
        else if (bus[w-i-1] == lcs::HIGH)
            std::cout << "1";
        else if (bus[w-i-1] == lcs::UNKNOWN)
            std::cout << "x";
        else
            std::cout << "z";
    }

    if (bus[0] == lcs::LOW)
        return std::cout << "0";
    else if (bus[0] == lcs::HIGH)
        return std::cout << "1";
    else if (bus[0] == lcs::UNKNOWN)
        return std::cout << "x";
    else
        return std::cout << "z";
}

bool operator==(const lcs::InputBus<1> &b, const lcs::LineState &l);
bool operator==(const lcs::LineState &l, const lcs::InputBus<1> &b);
bool operator!=(const lcs::InputBus<1> &b, const lcs::LineState &l);
bool operator!=(const lcs::LineState &l, const lcs::InputBus<1> &b);

#endif // __LCS_BUS_H__
