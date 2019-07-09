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

#ifndef __LCS_INPUT_BUS_H__
#define __LCS_INPUT_BUS_H__

#include "expression.h"
#include "array1d.h"
#include "dataptr.h"
#include "module.h"
#include "line.h"
#include "list.h"
#include "outofrange_exception.h"
#include <fstream>

namespace lcs
{ // Start of namespace lcs

/// A class encapsulating a set of data lines to be used as an input to
/// a \p lcs::Module object. A client can only read the data lines and cannot set/modify
/// them. Use the class \p lcs::Bus in order to obtain encapsulated lines whose levels can
/// be set.
///
/// \param bits The number of data lines in the bus.
template <int bits = 1>
class InputBus : protected Array1D<Line, bits>
{

template <int w, ExprType Type, typename LExprType, typename RExprType>
friend class Expression;

/// InputBus classes of different sizes are friends of each other.
///
template <int w>
friend class InputBus;

/// InputBus classes of different sizes are friends of each other.
///
template <int w>
friend class Bus;

public:

    /// The default constructor.
    /// The data lines are all initialised to the lcs::UNKNOWN state.
    InputBus(void);

    /// Copy constructor.
    /// Performs only a shallow copy. The new \p InputBus object and \p bus share the same
    /// set of lines.
    ///
    /// \param bus The \p InputBus object whose lines are used to create a new \p InputBus
    /// object.
    InputBus(const InputBus<bits> &bus);

    /// Creates a single-line bus object containing the line object which is passed as the
    /// argument. One can only use 1 as the class template argument. Other numbers will lead
    /// to an \p lcs::OutOfRangeException<int>
    ///
    /// \param line The line object which is encapsulated in the resultant bus object.
    InputBus(const Line &line) throw(OutOfRangeException<int>);

    /// Destructor.
    ///
    virtual ~InputBus();

    /// Returns the bus width.
    /// Bus width is the same as the number of data lines in the bus.
    int width(void) const { return bits; }

    /// Registers the module which has to be notified about an occurance of a line event.
    /// A module can request notification at the occurance of one of the three different
    /// kinds of line events. These line events are enumerated as \p lcs::LineEvent members.
    /// Every module which intends to be driven by an \p InputBus will have to register
    /// itself using this function. Also, the module should be an object of a class which
    /// is a derivate of the \p lcs::Module class.
    ///
    /// This function typically needs to be used only by module implementers in the
    /// constructors of their \p lcs::Module derivatives. Other users will never need to
    /// use this function.
    ///
    /// \param mod Pointer to the \p lcs::Module object which has to be notified about a line
    /// event.
    ///
    /// \param event The event type whose occurance the module will be notified of. For
    /// example, if lcs::LINE_POS_EDGE is used, then the module will notified at the
    /// occurance of a positive edge on the line.
    ///
    /// \param portId The id of the module port to which the bus is connected. The bus will
    /// use this id while notifying the module.
    ///
    /// \param line The index of the bus line whose line events the module seeks to be
    /// notified of. The default value is -1. In general, a negetive value indicates that
    /// the module should be notified of the event occuring on all lines of the bus.
    ///
    void notify(Module *mod, const LineEvent &event, const int &portId,
                const int &line = -1) throw(OutOfRangeException<int>);

    /// De-registers a module from the list of modules which have to be notified.
    /// See \p InputBus::notify for more details.
    ///
    /// This function typically needs to be used only by module implementers in the
    /// destructors of their \p lcs::Module derivatives. Other users will never need to
    /// use this function.
    ///
    /// \param mod Pointer to the \p lcs::Module object which has to be de-registered.
    ///
    /// \param event The event type whose occurance the module was be notified.
    ///
    /// \param portId The id of the module port to which the bus is connected.
    ///
    /// \param line The index of the bus line whose line events the module sought to be
    /// notified of. The default value is -1. In general, a negetive value indicates that
    /// the module should de-registered from being notified by all lines of the bus.
    ///
    void stopNotification(Module *mod, const LineEvent &event, const int &portId,
                          const int &line = -1) throw(OutOfRangeException<int>);

    /// Converts the binary bit value in the bus lines to a decimal integer.
    ///
    unsigned long toInt() const;

    /// Converts the binary bit value in the bus lines to a string representation.
    ///
    std::string toStr(void) const;

    /// Returns the line state of the line specified by the \p index argument. Results in an
    /// \p lcs::OutOfRangeException if an invalid or out of range index is used. Valid range
    /// for the index is \p 0 to \p w-1, where \p w is the width of the bus.
    ///
    /// \param index The index of the line whose line state will be returned by the
    /// function.
    const LineState get(int index) const throw (OutOfRangeException<int>);

    /// Overloaded operator which returns a suitable expression object corresponding
    /// to the line at index \p i. This is done so that bit-selects can be used in expressions of
    /// bitwise operations. An \p OutOfRangeException is thrown if the index value \p i is beyond
    /// the bus width.
    inline const InputBus<1> operator[](unsigned int i) const throw(OutOfRangeException<unsigned int>);

    /// The overloaded operator to join data lines from two busses and form a new bus
    /// from these. The right operand bus takes the MSB locations of the new \p InputBus
    /// object.
    ///
    /// \param bits The width of the left-operand bus
    /// \param w The width of the right operand bus
    /// \param bus The right operand \p Bus object.
    ///
    template <int w>
    const InputBus<w+bits> operator,(const InputBus<w> &bus) const;

    /// The overloaded operator to join a data line to a bus to form a new bus. The line
    /// to be joined will have to be the right operand. The joined line takes the MSB
    /// location in the resulting \p InputBus object.
    ///
    /// \param line The right operand \p lcs::Line object.
    ///
    const InputBus<bits+1> operator,(const Line &line) const;

    /// Returns a part-bus formed from a set of consecutive lines of the original
    /// \p lcs::InputBus object. If \p s+w goes beyond the range of the original bus
    /// width, then, only the lines within range are assigned to the lines of the new bus object.
    ///
    /// \param w The width of the part-bus.
    /// \param s The start bit from where the part-bus should be accumulated.
    template <int w>
    const InputBus<w> partSelect(int s) const;

};

template <int bits>
InputBus<bits>::InputBus(void) : Array1D<Line, bits>()
{
    Line *data = Array1D<Line, bits>::dataPtr->data;
    for (int i = 0; i < bits; i++)
        data[i].setLineValue(UNKNOWN);
}

template <int bits>
InputBus<bits>::InputBus(const InputBus<bits> &bus) : Array1D<Line, bits>(bus) { }

template <int bits>
InputBus<bits>::InputBus(const Line &l) throw(OutOfRangeException<int>)
   : Array1D<Line, 1>()
{
    if (bits != 1)
        throw OutOfRangeException<int>(1, 1, bits);

    Line *data = Array1D<Line, bits>::dataPtr->data;
    data[0] = l;
}

template <int bits>
InputBus<bits>::~InputBus() { }

template <int bits>
void InputBus<bits>::notify(Module *mod, const LineEvent &event, const int &portId,
                            const int &line) throw(OutOfRangeException<int>)
{
    if (line >= bits)
    {
        OutOfRangeException<int> ex(0, bits-1, line);
        throw ex;
    }

    Line *data = Array1D<Line, bits>::dataPtr->data;

    if (line < 0)
    {
        for (int i = 0; i < bits; i++)
        {
            Line dataline = data[i];
            dataline.notify(event, mod, portId);
        }
    }
    else
    {
        Line dataline = data[line];
        dataline.notify(event, mod, portId);
    }
}

template <int bits>
void InputBus<bits>::stopNotification(Module *mod, const LineEvent &event, const int &portId,
                                      const int &line) throw(OutOfRangeException<int>)
{
    if (line >= bits)
    {
        OutOfRangeException<int> ex(0, bits-1, line);
        throw ex;
    }

    Line *data = Array1D<Line, bits>::dataPtr->data;

    if (line < 0)
    {
        for (int i = 0; i < bits; i++)
        {
            Line dataline = data[i];
            dataline.stopNotification(event, mod, portId);
        }
    }
    else
    {
        Line dataline = data[line];
        dataline.stopNotification(event, mod, portId);
    }
}

template <int bits>
template <int w>
const InputBus<w> InputBus<bits>::partSelect(int s) const
{
    InputBus<w> b;

    Line *bdata = b.dataPtr->data, *data = Array1D<Line, bits>::dataPtr->data;
    for (int i = 0; i < w; i++)
    {
        if (i+s < bits)
            bdata[i] = data[i+s];
        else
            break;
    }

    return b;
}

template <int bits>
unsigned long InputBus<bits>::toInt(void) const
{
    unsigned long val = 0;
    Line *data = Array1D<Line, bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        unsigned long bitVal = 0;
        if (data[i].operator()() == HIGH)
            bitVal = 1;
        else if (data[i].operator()() == UNKNOWN || data[i].operator()() == HIGH_IMPEDENCE)
        {
            val = 0;
            break;
        }

        bitVal <<= i;
        val += bitVal;
    }

    return val;
}

template <int bits>
std::string InputBus<bits>::toStr(void) const
{
    Line *data = Array1D<Line, bits>::dataPtr->data;
    std::stringstream ss;

    for (int i = 0; i < bits; i++)
    {
        LineState state = data[i].operator()();
        char bitVal = '0';

        if (state == HIGH)
            bitVal = '1';
        else if (state == HIGH_IMPEDENCE)
            bitVal = 'z';
        else if (state == UNKNOWN)
            bitVal = 'x';

        ss << bitVal;
    }

    return ss.str();
}

template <int bits>
const LineState InputBus<bits>::get(int index) const throw (OutOfRangeException<int>)
{
    if (index < 0 || index >= bits)
        throw OutOfRangeException<int>(0, bits-1, index);

    Line *line = Array1D<Line, bits>::dataPtr->data;
    line += index;

    return line->operator()();
}

template <int bits>
const InputBus<1> InputBus<bits>::operator[](unsigned int index) const
                                                            throw(OutOfRangeException<unsigned int>)
{
    if (index >= bits)
        throw OutOfRangeException<unsigned int>(0, static_cast<unsigned int>(bits-1), index);

    InputBus<1> b;

    Line *line = Array1D<Line, bits>::dataPtr->data,
         *bline = b.dataPtr->data;

    bline[0] = line[index];
    return b;
}

template <int bits>
template <int w>
const InputBus<w+bits> InputBus<bits>::operator,(const InputBus<w> &bus) const
{
    InputBus<w+bits> b;
    Line *data = Array1D<Line, bits>::dataPtr->data, *bdata = b.dataPtr->data;
    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    data = bus.dataPtr->data;
    for (int i = 0; i < w; i++)
        bdata[i+bits] = data[i];

    return b;
}

template <int bits>
const InputBus<bits+1> InputBus<bits>::operator,(const Line &line) const
{
    Bus<1+bits> b;
    Line *data = Array1D<Line, bits>::dataPtr->data, *bdata = b.dataPtr->data;
    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    bdata[bits] = line;

    return b;
}

} // End of namespace lcs

/// \function
/// The overloaded bitshift operator to display the bus data lines' states in the
/// little-endian format onto the \p std::stdout device.
template <int w>
std::ostream& operator<<(const std::ostream &osObj, const lcs::InputBus<w> &bus)
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

#endif // __LCS_INPUT_BUS_H__
