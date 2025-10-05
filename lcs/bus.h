//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the Improla library.
// Improla is a GUI framework for image processing.
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
// You should have received a copy of the GNU Library General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// In case you would like to contact the author, use the following e-mail
// address: sivachandra_br@yahoo.com
//////////////////////////////////////////////////////////////////////////////////

#ifndef __LCS_BUS_H__
#define __LCS_BUS_H__

#include "inbus.h"

namespace lcs
{ // Start of namespace lcs.

/// A class encapsulating a set of data lines to be used as a generic bus in logic circuit
/// systems. The data lines are conceptially organised in the little endian format. A user
/// can set and read the individual data lines of the bus. There are overloaded operators
/// provided which can be used to join two busses, or join a line to a bus.
///
/// \param bits The number of data lines in the bus.
///
template <int bits = 1>
class Bus : public InputBus<bits>
{

/// Bus classes of other sizes are friends of this class.
///
template <int w>
friend class Bus;

public:

    /// The default constructor.
    /// The data lines are all initialised to the lcs::UNKNOWN state.
    Bus(void);

    /// Copy constructor.
    /// Performs only a shallow copy. The new \p Bus object and \p bus share the same
    /// set of lines.
    ///
    /// \param bus The \p Bus object whose lines are used to create a new \p Bus object.
    ///
    Bus(const Bus<bits> &bus);

    /// Destructor.
    ///
    virtual ~Bus();

    /// The operator to read the data lines. It is inherited from lcs::InputBus.
    ///
    using InputBus<bits>::operator[];

    /// The operator which can be used to read and write data onto the lines of the bus.
    ///
    /// \param index The index of the data line in the bus. Note that all data in libLCS
    /// is stored in the little-endian format.
    ///
    Line& operator[](int index);

    /// The overloaded operator to join data lines from two busses and form a new bus
    /// from these. The right operand bus takes the MSB locations.
    ///
    /// \param bits The width of the left-operand bus
    /// \param w The width of the right operand bus
    /// \param bus The right operand Bus object.
    ///
    template <int w>
    const Bus<w+bits> operator*(const Bus<w> &bus) const;

    /// The overloaded operator to join a data line to a bus to form a new bus. The line
    /// to be joined will have to be the right operand. The joined line takes the MSB
    /// location in the resulting Bus object.
    ///
    /// \param line The right operand lcs::Line object.
    ///
    const Bus<bits+1> operator*(const Line &line) const;

    /// The assignment operator which sets the data lines to a binary equivalent of the
    /// decimal argument \p value.
    ///
    /// \param value The decimal equivalent of the binary value which has to be set.
    ///
    void operator=(int value);

private:

    /// This function is inherited from InputBus. However, it has been declared
    /// private so that the user is prevented from using a Bus object to drive/un-drive
    /// a module.
    void drive(Module *mod) {}

    /// This function is inherited from InputBus. However, it has been declared
    /// private so that the user is prevented from using a Bus object to drive/un-drive
    /// a module.
    void unDrive(Module *mod) {}
};

template <int bits>
Bus<bits>::Bus(void)
   : InputBus<bits>()
{}

template <int bits>
Bus<bits>::Bus(const Bus<bits> &bus)
   : InputBus<bits>(bus)
{}

template <int bits>
Bus<bits>::~Bus()
{}

template <int bits>
Line& Bus<bits>::operator[](int index)
{
    Line *data, line;
    data = InputBus<bits>::dataPtr->data;

    return data[index];
}

template <int bits>
template <int w>
const Bus<w+bits> Bus<bits>::operator*(const Bus<w> &bus) const
{
    Bus<w+bits> b;
    Line *data = InputBus<bits>::dataPtr->data;
    for (int i = 0; i < bits; i++)
        b[i] = data[i];

    data = bus.dataPtr->data;
    for (int i = 0; i < w; i++)
        b[i+bits] = data[i];

    return b;
}

template <int bits>
const Bus<bits+1> Bus<bits>::operator*(const Line &line) const
{
    Bus<1+bits> b;
    Line *data = InputBus<bits>::dataPtr->data;
    for (int i = 0; i < bits; i++)
        b[i] = data[i];

    b[bits] = line;

    return b;
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

/// \function
/// Performs an in-place negation of all the data lines of a bus. The data lines whose
/// state is \p lcs::UNKOWN, or \p lcs::HIGH_IMPEDENCE, are left unaltered.
template <int bits>
const Bus<bits> &operator~(Bus<bits> &bus)
{
    for (int i = 0; i < bits; i++)
    {
        if (bus[i] == LOW)
            bus[i] = HIGH;
        else if (bus[i] == HIGH)
            bus[i] = LOW;
    }

    return bus;
}

} // End of namespace lcs.

/// \function
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

#endif // __LCS_BUS_H__
