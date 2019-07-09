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

#ifndef __LCS_BIT_BUFF_H__
#define __LCS_BIT_BUFF_H__

#include "linestate.h"
#include "array1d.h"
#include "expression.h"
#include "bit.h"
#include <sstream>

namespace lcs
{ // Start of namespace lcs.

template <int w>
class InputBus;

template <int w, ExprType Type, typename LExprType, typename RExprType>
class Expression;

/// A utility class which encapsulates a group of \p lcs::Bit objects. Objects of this class should
/// typically be used as temporary/intermediate data storage buffers.
template <int bits = 1>
class BitBuff : protected Array1D<Bit, bits>
{

template <int w, ExprType Type, typename LExprType, typename RExprType>
friend class Expression;

template <int w>
friend class BitBuff;

public:

    /// Default constructor.
    ///
    BitBuff(void);

    /// Copy constructor.
    ///
    BitBuff(const BitBuff<bits> &reg);

    /// Destructor.
    ///
    virtual ~BitBuff();

    /// Returns a part-buffer formed from a set of consecutive bits of the original
    /// \p BitBuffer object. If \p s+w goes beyond the range of the original bus
    /// width, then, only the bits within range are assigned to the bits of the new bus object.
    ///
    /// \param w The width of the part-buffer.
    /// \param s The start bit from where the part-buffer should be accumulated.
    template <int w>
    const BitBuff<w> partSelect(int s);

    /// Registers the module which has to be notified about a bit state change.
    ///
    /// \param mod Pointer to the module object which seeks to be notified about a bit state change.
    /// \param bit The index of the bit whose state change should be notified to the module.
    void notify(Module *mod, int bit = -1);

    /// De-registers a module from the list of modules which have to be notified of bit state changes.
    ///
    /// \param mod Pointer to the module object which seeks to de-register.
    /// \param bit The index of the bit whose state change was being notified to the module.
    void stopNotification(Module *mod, int bit = -1);

    /// Returns the width of the buffer.
    ///
    int width(void) { return bits; }

    /// Converts the binary bit value in the buffer to a decimal integer.
    ///
    unsigned long toInt(void) const;

    /// Converts the binary bit value in the buffer to a string representation.
    ///
    std::string toStr(void) const;

    /// Assignment operator.
    ///
    template <int rbits>
    BitBuff<bits>& operator=(const BitBuff<rbits>& rhs);

    /// Assignment operator to facilitate assignment using an \p lcs::Expression object.
    /// ie., the operator facilitates assignment with an expression of bitwise operations
    /// on the right hand side of the assignment operator.
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    BitBuff<bits>& operator=(const Expression<w, Type, LExprType, RExprType> &expr);

    /// Assignment operator to facilitate assignment using a \p lcs::InputBus object.
    ///
    template <int rbits>
    BitBuff<bits>& operator=(const InputBus<rbits> &rhs);

    /// Returns a the suitable constant expression object when a bitselect is requested. An
    /// expression object is returned so that a bitselect can be used in a normal expression of
    /// bitwise operations.
    const Expression<1, BITBUFF_EXPR, void, void> operator[](int index) const
                                                        throw (OutOfRangeException<int>);

    /// Returns a the suitable expression object when a bitselect is requested. An
    /// expression object is returned so that a bitselect can be used in a normal expression of
    /// bitwise operations.
    Expression<1, BITBUFF_EXPR, void, void> operator[](int index) throw (OutOfRangeException<int>);

};

template <int bits>
BitBuff<bits>::BitBuff(void)
              : Array1D<Bit, bits>()
{}

template <int bits>
BitBuff<bits>::BitBuff(const BitBuff<bits> &reg)
              : Array1D<Bit, bits>(reg)
{}

template <int bits>
BitBuff<bits>::~BitBuff(void)
{}

template <int bits>
template <int w>
const BitBuff<w> BitBuff<bits>::partSelect(int s)
{
    BitBuff<w> newBuff;
    Bit *ndata = newBuff.dataPtr->data;
    Bit *data = Array1D<Bit, bits>::dataPtr->data;

    for (int i = s; (i < bits && i-s < w); i++)
    {
        ndata[i-s] = data[i];
    }

    return newBuff;
}

template <int bits>
void BitBuff<bits>::notify(Module *mod, int bit)
{
    Bit *data = Array1D<Bit, bits>::dataPtr->data;

    if (bit < 0)
    {
        for (int i = 0; i < bits; i++)
            data[i].notify(mod);
    }
    else if (bit < bits)
    {
        data[bit].notify(mod);
    }
}

template <int bits>
void BitBuff<bits>::stopNotification(Module *mod, int bit)
{
    Bit *data = Array1D<Bit, bits>::dataPtr->data;

    if (bit < 0)
    {
        for (int i = 0; i < bits; i++)
            data[i].stopNotification(mod);
    }
    else if (bit < bits)
    {
        data[bit].stopNotification(mod);
    }
}

template <int bits>
unsigned long BitBuff<bits>::toInt(void) const
{
    unsigned long val = 0;
    Bit *data = Array1D<Bit, bits>::dataPtr->data;


    for (int i = 0; i < bits; i++)
    {
        unsigned long bitVal = 0;
        if (data[i].state() == HIGH)
            bitVal = 1;
        else if (data[i].state() == UNKNOWN || data[i].state() == HIGH_IMPEDENCE)
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
std::string BitBuff<bits>::toStr(void) const
{
    Bit *data = Array1D<Bit, bits>::dataPtr->data;
    std::stringstream ss;

    for (int i = 0; i < bits; i++)
    {
        LineState state = data[i].state();
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
template <int rbits>
BitBuff<bits>& BitBuff<bits>::operator=(const BitBuff<rbits> &rhs)
{
    Bit *data     = Array1D<Bit, bits>::dataPtr->data;
    Bit *rhs_data = rhs.dataPtr->data;
    if (rbits > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = rhs_data[i];
    }
    else
        for (unsigned int i = 0; i < rbits; i++)
            data[i] = rhs_data[i];

    return *this;
}

template <int bits>
template <int rbits>
BitBuff<bits>& BitBuff<bits>::operator=(const InputBus<rbits> &rhs)
{
    Bit *data     = Array1D<Bit, bits>::dataPtr->data;
    if (rbits > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = rhs[i];
    }
    else
        for (unsigned int i = 0; i < rbits; i++)
            data[i] = rhs[i];

    return *this;
}

template <int bits>
template <int w, ExprType Type, typename LExprType, typename RExprType>
BitBuff<bits>& BitBuff<bits>::operator=(const Expression<w, Type,
                                                           LExprType, RExprType> &expr)
{
    Bit *data = Array1D<Bit, bits>::dataPtr->data;
    if (static_cast<unsigned int>(w) > bits)
    {
        for (unsigned int i = 0; i < bits; i++)
            data[i] = expr[i];
    }
    else
        for (int i = 0; i < w; i++)
            data[i] = expr[i];

    return *this;
}

template <int bits>
const Expression<1, BITBUFF_EXPR, void, void> BitBuff<bits>::operator[](int index) const
                                                        throw (OutOfRangeException<int>)
{
    if (index < 0 || index >= static_cast<int>(bits))
        throw OutOfRangeException<int>(0, bits-1, index);

    Bit *data = Array1D<Bit, bits>::dataPtr->data;
    Bit b = data[index];

    BitBuff<1> buff;
    Bit *buffData = buff.dataPtr->data;
    buffData[0] = b;

    return Expression<1, BITBUFF_EXPR, void, void>(buff);
}

template <int bits>
Expression<1, BITBUFF_EXPR, void, void> BitBuff<bits>::operator[](int index)
                                                        throw (OutOfRangeException<int>)
{
    if (index < 0 || index >= static_cast<int>(bits))
        throw OutOfRangeException<int>(0, bits-1, index);

    Bit *data = Array1D<Bit, bits>::dataPtr->data;
    Bit b = data[index];

    BitBuff<1> buff;
    Bit *buffData = buff.dataPtr->data;
    buffData[0] = b;

    return Expression<1, BITBUFF_EXPR, void, void>(buff);
}

} // End of namespace lcs.

template <int w>
std::ostream& operator<<(const std::ostream &osObj, const lcs::BitBuff<w> &buff)
{
    for (int i = 0; i < w-1; i++)
    {
        if (buff[w-i-1] == lcs::LOW)
            std::cout << "0";
        else if (buff[w-i-1] == lcs::HIGH)
            std::cout << "1";
        else if (buff[w-i-1] == lcs::UNKNOWN)
            std::cout << "x";
        else
            std::cout << "z";
    }

    if (buff[0] == lcs::LOW)
        return std::cout << "0";
    else if (buff[0] == lcs::HIGH)
        return std::cout << "1";
    else if (buff[0] == lcs::UNKNOWN)
        return std::cout << "x";
    else
        return std::cout << "z";
}

#endif // __LCS_BIT_BUFF_H__
