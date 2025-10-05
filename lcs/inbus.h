//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the libLCS library.
// libLCS is C++ Logic Circuit Simulation library.
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// In case you would like to contact the author, use the following e-mail
// address: sivachandra_br@yahoo.com
//////////////////////////////////////////////////////////////////////////////////

#ifndef __LCS_INPUT_BUS_H__
#define __LCS_INPUT_BUS_H__

#include "dataptr.h"
#include "module.h"
#include "line.h"
#include "list.h"

namespace lcs
{ // Start of namespace lcs

/// A class encapsulating a set of data lines to be used as an input to
/// a \p lcs::Module object. A client can only read the data lines and cannot set/modify
/// them. Use the class \p lcs::Bus in order to obtain lines whose levels can be set by a
/// client.
///
/// \param bits The number of data lines in the bus.
template <int bits = 1>
class InputBus
{

/// InputBus classes of other sizes are friends of this class.
///
template <int w>
friend class InputBus;

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

    /// Destructor.
    ///
    virtual ~InputBus();

    /// Returns the bus width.
    /// Bus width is the same as the number of data lines in the bus.
    int width(void) { return *size; }

    /// Registers the module which has to be driven.
    /// Every module which intends to be driven by an \p InputBus will have to register
    /// itself using this function.
    ///
    /// \param mod A pointer to the lcs::Module object which has to be driven.
    void drive(Module *mod);

    /// De-registers a module from the list of modules which have to be driven.
    /// See InputBus::drive for more details.
    void unDrive(Module *mod);

    /// Overloaded operator which returns a const lcs::LineState variable corresponding
    /// to the state if the line at index \p i. The returned value is only a copy of the
    /// line state.
    const LineState operator[](int i) const;

    /// The overloaded operator to join data lines from two busses and form a new bus
    /// from these. The right operand bus takes the MSB locations of the new InputBus
    /// object.
    ///
    /// \param bits The width of the left-operand bus
    /// \param w The width of the right operand bus
    /// \param bus The right operand Bus object.
    ///
    template <int w>
    const InputBus<w+bits> operator*(const InputBus<w> &bus) const;

    /// The overloaded operator to join a data line to a bus to form a new bus. The line
    /// to be joined will have to be the right operand. The joined line takes the MSB
    /// location in the resulting InputBus object.
    ///
    /// \param line The right operand lcs::Line object.
    ///
    const InputBus<bits+1> operator*(const Line &line) const;

protected:

    /// A pointer to the array of \p lcs::Line objects encapsulated in an \p InputBus.
    ///
    DataPtr<Line> *dataPtr;

private:
    int *size;
    int *refCount;
};

template <int bits>
InputBus<bits>::InputBus(void)
{
    dataPtr = new DataPtr<Line>();
    size = new int;

    refCount = new int;
    *refCount = 1;

    if (bits <= 0)
    {
        dataPtr->data = new Line;
        Line *data = dataPtr->data;

        data[0] = UNKNOWN;
        *size = 1;
    }
    else
    {
        dataPtr->data = new Line [bits];
        *size = bits;

        Line *data = dataPtr->data;
        for (int i = 0; i < bits; i++)
            data[i] = UNKNOWN;
    }
}

template <int bits>
InputBus<bits>::InputBus(const InputBus<bits> &bus)
   : dataPtr(bus.dataPtr), size(bus.size), refCount(bus.refCount)
{
    (*refCount)++;
}

template <int bits>
InputBus<bits>::~InputBus()
{
    if (*refCount > 1)
    {
        (*refCount)--;

        refCount = NULL;
        dataPtr = NULL;
        size = NULL;
    }
    else
    {
        delete [] dataPtr->data;
        delete dataPtr;
        delete refCount;
        delete size;
    }
}

template <int bits>
void InputBus<bits>::drive(Module *mod)
{
    Line *data = dataPtr->data;
    for (int i = 0; i < *size; i++)
    {
        Line line = data[i];
        line.modList.append(mod);
    }
}

template <int bits>
void InputBus<bits>::unDrive(Module *mod)
{
    Line *data = dataPtr->data;
    for (int i = 0; i < *size; i++)
    {
        Line line = data[i];
        line.modList.remove(mod);
    }
}

template <int bits>
const LineState InputBus<bits>::operator[](int index) const
{
    Line *data, l;
    data = dataPtr->data;
    l = data[index];

    return l();
}

template <int bits>
template <int w>
const InputBus<w+bits> InputBus<bits>::operator*(const InputBus<w> &bus) const
{
    InputBus<w+bits> b;
    Line *data = dataPtr->data, *bdata = b.dataPtr->data;
    for (int i = 0; i < bits; i++)
        bdata[i] = data[i];

    data = bus.dataPtr->data;
    for (int i = 0; i < w; i++)
        bdata[i+bits] = data[i];

    return b;
}

template <int bits>
const InputBus<bits+1> InputBus<bits>::operator*(const Line &line) const
{
    Bus<1+bits> b;
    Line *data = dataPtr->data, *bdata = b.dataPtr->data;
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
