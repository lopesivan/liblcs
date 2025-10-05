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
#include "shortcircuit_exception.h"

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

    /// This is an inner class to privde a bus write-access control mechanism.
    /// This class provides a mechanism to write line data on to a bus. See
    /// \p Bus::getLineAccessor and \p Bus::lock for more information on why and when
    /// to use a \p LineAccessor object.
    class LineAccessor
    {
        public:

            /// Default constructor.
            /// An object initialised through a default constructor is practically useless.
            /// Always use the lcs::LineAccessor object returned by the function
            /// \p lcs::Bus::getLineAccessor().
            LineAccessor() : linePtr(NULL) {}

            /// The only usefull constructor.
            /// User will never need to initialise a \p LineAccessor object through the
            /// constructor. Unless very sure, let the function \p Bus::getLineAccessor
            /// construct and return a \p lcs::LineAccessor object for you.
            ///
            /// \param lineDataPtr Pointer to the array of \p lcs::Line objects which
            /// constitute a \p Bus object.
            LineAccessor(Line *lineDataPtr) : linePtr(lineDataPtr) {}

            /// Copy constructor.
            ///
            LineAccessor(const LineAccessor &la) : linePtr(la.linePtr) {}

            /// Returns \p true if the \p LineAccessor can access valid bus line data,
            /// \p false if not.
            bool isValid() const;

            /// Overloaded operator for line access. Usage is very similar to accessing
            /// array elements from arrays of standard data types.
            /// An \p lcs::OutOfRangeException is thrown if \p index is beyond the bus width.
            /// A \p lcs::ShortCircuitException is thrown if the line access is illegal.
            ///
            /// \param index The index into the bus bits.
            Line& operator[](int index);

            /// Makes a bitwise assignment to the lines of the bus object according
            /// to the bit representation of the integer \p value.
            /// A \p lcs::ShortCircuitException is thrown if modifying line states is illegal.
            ///
            void operator=(int value);

        private:
            Line *linePtr;  // The overloaded operator[] returns a valid reference to a
                            // Line object only if linePtr is not NULL;
    };

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

    /// Destructor.
    ///
    virtual ~Bus();

    /// Locks a \p Bus object with the module \p mod. Locking a bus with a module will
    /// provide bus line write access only to that module. Other modules trying
    /// to write on to the bus will cause a short circuit (which in software is
    /// equivalent to an occurrance of an exception). If a bus has been locked previously
    /// by a \p lcs::Module derivative, then a warning is issued about a possible short
    /// circuit when an other module tries to lock the same bus.
    ///
    /// Apart from locking the bus, one can also use this function to set the assignment
    /// delay associated with the bus. The assignment delays can be used to simulate
    /// input to output propogation delays involved with modules.
    ///
    /// This function should typically be used by module implementers in the constructors
    /// of their \p lcs::Module derivatives. Other users will never need to use this
    /// function.
    ///
    /// See \p Bus::getLineAccessor and \p LineAccessor for more information.
    ///
    /// \param mod Pointer to an object which is a \p lcs::Module derivative. The bus will
    /// be locked with write access only to this object.
    /// \param delay The assignment delay associated with the line in system time units.
    void lock(Module *mod, unsigned int delay = 0);

    /// Un-locks the bus from a module. If a module does not intend to drive a bus
    /// anymore, it can (ideally should) unlock, or free, the bus for use by some other
    /// module. A call to this function does nothing if the bus is not locked by a module.
    /// Nothing is done even when a module tries to unlock a bus which has been locked by
    /// another module.
    ///
    /// This function should typically be used by module implementers in the destructors
    /// of their \p lcs::Module derivatives. Other users will never need to use this
    /// function.
    ///
    /// See \p lcs::Bus::lock for more information.
    ///
    /// \param mod Pointer to the module object which wants to unlock/free the bus.
    ///
    void unLock(Module *mod);

    /// Returns true if the bus is locked by some module. False if not.
    ///
    /// See the functions \p lcs::Bus::lock, \p lcs::Bus::unLock,
    /// \p lcs::Bus::getLineAccessor for more information on locking and un-locking busses.
    bool isLocked(void);

    /// Returns a \p LineAccessor object which can be used for writing on to the bus lines.
    /// See the \p LineAccessor documentation for more information on using a
    /// \p LineAccessor object.
    ///
    /// To map reality into software, there should be mechanism which provides bus write
    /// access only to that module which is driving the bus. This sort of
    /// control is achieved by using this function in conjuction with the \p Bus::lock
    /// function. The \p lcs::Bus::lock function locks a \p lcs::Bus object from being
    /// written on to by modules other than the one which has locked it. If a module which
    /// has locked the bus requests for a \p LineAccessor through this function
    /// (\p lcs::Bus::getLineAccessor), a <i> valid </i> \p LineAccessor object is returned.
    /// Otherwise, an <i> invalid </i> \p LineAccessor object is returned, which will lead
    /// to an occurrance of an exception when the bus lines are accessed through it.
    ///
    /// <p>
    /// Typically, this function should only be used by module implementors while
    /// over-riding the \p lcs::Module virtual member functions. Other users should use this
    /// function only if they are sure of what they are doing.
    ///
    /// <p>
    /// <b>NOTE</b>: A \p LineAccessor object should only be used if one needs to write on
    /// to the bus lines. Reading the bus lines is available through the function
    /// \p lcs::InputBus::operator[]. A user who is not implementing his/her own module
    /// should never need to use a \p LineAccessor object.
    ///
    /// \param mod Pointer to the \p lcs::Module object which has locked the \p Bus object
    /// using the \p Bus::lock function.
    ///
    typename Bus<bits>::LineAccessor getLineAccessor(Module *mod);

    /// Returns a part-bus formed from a set of consecutive lines of the original \p lcs::Bus
    /// object. If \p s+w goes beyond the range of the original bus width, then, only the
    /// lines within range are assigned lines of the new bus object.
    ///
    /// \param w The width of the part-bus.
    /// \param s The start bit from where the part-bus should be accumulated.
    template <int w>
    const Bus<w> partBus(int s) const;

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

    /// The operator to read the data lines. It is inherited from lcs::InputBus.
    ///
    using InputBus<bits>::operator[];

    /// An operator which sets an integer with the decimal equivalent of the binary
    /// value stored on the lines of the bus.
    ///
    using InputBus<bits>::operator>>;

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

    /// An overloaded operator to obtain a single line bus of type lcs::Bus which
    /// has one of the lines (inidicated using the parameter index) of the original bus.
    const Bus<1> operator()(int index) const;

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
Bus<bits>::Bus(int val)
   : InputBus<bits>()
{
    typename Bus<bits>::LineAccessor lines(getLineAccessor(NULL));
    lines = val;
}

template <int bits>
Bus<bits>::Bus(const Bus<bits> &bus)
   : InputBus<bits>(bus)
{}

template <int bits>
Bus<bits>::~Bus()
{}

template <int bits>
void Bus<bits>::lock(Module *mod, unsigned int delay)
{
    Line *data = InputBus<bits>::dataPtr->data;
    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        if (line.isLocked())
        {
            std::cerr << "Trying to lock an already locked bus.\n";
            std::cerr << "Can lead to a ShortCircuitException and/or undefined behaviour!\n";
            return;
        }
    }

    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        line.lock(mod, delay);
    }
}

template <int bits>
void Bus<bits>::unLock(Module *mod)
{
    Line *data = InputBus<bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        if (line.lockedModPtr->data != mod)
        {
            std::cerr << "Trying to illegally unlock a bus line!\n";
            return;
        }
    }

    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        line.lockedModPtr->data = NULL;
    }
}

template <int bits>
bool Bus<bits>::isLocked(void)
{
    Line *data = InputBus<bits>::dataPtr->data;

    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        if (line.isLocked())
            return true;
    }

    return false;
}

template <int bits>
typename Bus<bits>::LineAccessor Bus<bits>::getLineAccessor(Module *mod)
{
    Line *data = InputBus<bits>::dataPtr->data;
    bool valid = true;

    for (int i = 0; i < bits; i++)
    {
        Line line = data[i];
        if (line.lockedModPtr->data != mod && line.lockedModPtr->data != NULL)
        {
            valid = false;
            break;
        }
    }

    if (valid)
    {
        typename Bus<bits>::LineAccessor la(InputBus<bits>::dataPtr->data);
        return la;
    }
    else
    {
        std::cerr << "Requesting line access for a bus which has been locked!" << std::endl;
        std::cerr << "Can lead to a ShortCircuitException and /or undefined behaviour!\n";

        typename Bus<bits>::LineAccessor la(NULL);
        return la;
    }

}

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
const Bus<w> Bus<bits>::partBus(int s) const
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

template <int bits>
const Bus<1> Bus<bits>::operator()(int i) const
{
    return this->partBus<1>(i);
}

template <int bits>
Line& Bus<bits>::LineAccessor::operator[](int index)
{
    if (linePtr == NULL)
    {
        std::cerr << "Outputs of two modules have got short-circuited.\n";
        std::cerr << "Expect total chaos!\n";

        throw ShortCircuitException();
    }
    else
    {
        if (index >= bits || index < 0)
        {
            OutOfRangeException<int> ex(0, bits-1, index);
            throw ex;
        }

        return linePtr[index];
    }
}

template <int bits>
void Bus<bits>::LineAccessor::operator=(int value)
{
    if (linePtr == NULL)
    {
        std::cerr << "Outputs of two modules have got short-circuited.\n";
        std::cerr << "Expect total chaos!\n";

        throw ShortCircuitException();
    }

    for (int i = 0; i < bits; i++)
    {
        int rem = value % 2;

        if (rem == 1)
            linePtr[i] = HIGH;
        else
            linePtr[i] = LOW;

        value = value >> 1;
    }
}

template <int bits>
bool Bus<bits>::LineAccessor::isValid(void) const
{
    if (linePtr == NULL)
        return false;
    else
        return true;
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

#endif // __LCS_BUS_H__
