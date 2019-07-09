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

#ifndef __LCS_LINE_H__
#define __LCS_LINE_H__

#include "exprtype.h"
#include "linestate.h"
#include "linevent.h"
#include "module.h"
#include "dataptr.h"
#include "list.h"
#include "queue.h"
#include "tickListener.h"
#include <iostream>

namespace lcs
{ // Start of namespace lcs

template <int bits, ExprType Type, typename LExprType, typename RExprType>
class Expression;

template <int w>
class InputBus;

template <int w>
class Bus;

/// This is a class which encapsulates a data line. A \p Line object is used to denote
/// one bit of a \p lcs::Bus and \p lcs::InputBus objects. Various operators are supported
/// to facilitate bit operations on \p Line objects.
class Line
{

/// \p lcs::Bus classes of all sizes are friends of this class.
///
template <int w>
friend class Bus;

/// \p lcs::InputBus classes of all sizes are friends of this class.
///
template <int w>
friend class InputBus;

/// \p lcs::Clock is a friend of this class.
///
friend class Clock;

public:

    /// Default constructor.
    ///
    Line(void);

    /// Copy constructor.
    ///
    Line(const Line& line);

    /// Destructor.
    ///
    virtual ~Line();

    /// Overloaded assignment operator which sets the line state with a \p LineState
    /// variable.
    ///
    /// \param value The value to which the line state should be changed.
    ///
    void operator=(const LineState& value);

    /// Assignment operator to facilitate assignment with an expression of bitwise operations.
    ///
    template <int bits, ExprType Type, typename LExprType, typename RExprType>
    inline void operator=(const Expression<bits, Type, LExprType, RExprType> &expr);

    /// Assignment oparator to facilitate assignment with an \p lcs::InputBus object.
    ///
    template <int bits>
    inline void operator=(const InputBus<bits> &bus);

    /// Returns the line state.
    ///
    const LineState operator()(void) const { return *val; }

    /// Overloaded bit OR operator.
    /// Performs a OR operation of the line states of two \p Line objects.
    const LineState operator|(const Line& line) const { return *val|*(line.val); }

    /// Overloaded bit AND operator.
    /// Performs a AND operation of the line states of two \p Line objects.
    const LineState operator&(const Line& line) const { return *val&*(line.val); }

    /// Overloaded bit XOR operator.
    /// Performs a XOR operation of the line states of two \p Line objects.
    const LineState operator^(const Line& line) const { return *val^*(line.val); }

    /// Overloaded logical equality operator.
    /// Compares the line state with a \p LineState variable.
    ///
    /// \param state The \p LineState variable with which the comparison should be made.
    bool operator==(const LineState& state) const
    {
        if (*val == state) return true; else return false;
    }

    /// Overloaded logical inequality operator.
    /// Compares the line state with a \p LineState variable.
    ///
    /// \param state The \p LineState variable with which the comparison should be made.
    ///
    bool operator!=(const LineState& state) const
    {
        if (*val != state) return true; else return false;
    }

    /// This function is used by the system time keeper to notify the line to let it
    /// update it's state after a certain delay. Under normal circumstances, a user of
    /// libLCS will never require to call this function.
    void update(void);

    // This function is called by the class SysmtemTimer. When a call is made, this function
    // updates the line state with the new value. This function is relevant only if the line
    // is a zero delay line. The documentation is not done in the doxygen format as I
    // would not like to let it show up the API reference.
    void hiddenUpdate(void);

private:

    /// Explicit assignment operator.
    /// The LHS Line object and the RHS Line object will both share the same line data
    /// after assignment.
    const Line& operator=(const Line &line);

    /// A \p lcs::Module (or its derivative) should register to be notified by the line about
    /// line events using this function. However, since this function is declared as a
    /// private member, one cannot use this function directly. See \p lcs::InputBus::notify
    /// for more information. The \p lcs::Line::notify is called by the function
    /// \p lcs::InputBus::notify.
    ///
    /// \param event The event of whose occurance the module should be notified of.
    /// \param mod Pointer to the \p lcs::Module object which has to be driven by the line.
    /// \param portId The id of the module port to which the line is connected.
    void notify(const LineEvent &event, Module *mod, const int &portId);

    /// A lcs::Module (or its derivative) can de-register itself from being driven by the
    /// line using this function. See \p lcs::Line::notify for more information. As with
    /// \p lcs::Line::notify, even this function is declared private and hence cannot be
    /// used directly. One has to use \p lcs::Bus::stopNotification, which internally makes
    /// a call to \p lcs::Line::stopNotification.
    ///
    /// \param event The event of whose occurance the module was being notified of.
    /// \param mod Pointer to the lcs::Module object which should be de-registered from
    /// being notifed by the line.
    /// \param portId The id of the module port to which the line is connected.
    void stopNotification(const LineEvent& event, Module *mod, const int &portId);

    /// This is an internal function used by other functions of this class. As it is declared
    /// as private, a user of libLCS will never require to use this function.
    void setLineValue(const LineState &value);

    /// Sets the assignement delay for the line.
    ///
    void setAssDelay(unsigned int delay);

    LineState *val;
    int *refCount;
    unsigned int *delay_;
    List< Line* > *refList;
    List< Module* > changeModList, posEdgeList, negEdgeList;
    List< int > changeModIdList, posEdgeIdList, negEdgeIdList;
    PriorityQueue<unsigned int, ASCENDING> nextChangeTimeQueue;
    Queue<unsigned int> zeroDelayChangeTimeQueue;
    PriorityQueue<LineState, ASCENDING> valueQueue;
    Queue<LineState> zeroDelayValueQueue;
};

template <int bits, ExprType Type, typename LExprType, typename RExprType>
void Line::operator=(const Expression<bits, Type, LExprType, RExprType> &expr)
{
    operator=(expr[0]);
}

template <int bits>
inline void Line::operator=(const InputBus<bits> &bus)
{
    operator=(Expression<bits, BUS_EXPR, void, void>(bus));
}

} // End of namespace lcs

/// \function
/// Overloaded bit-shift operator to display a lcs::Line object's state on std::stdout.
///
std::ostream& operator<<(const std::ostream &osObj, const lcs::Line &line);

/// \function
/// Inplace negation operator.
/// The line state of the lcs::Line object is toggled if known. The state is left
/// unchanged if it is lcs::UNKNOWN or lcs::HIGH_IMPEDENCE.
const lcs::Line& operator~(lcs::Line &line);

/// \function
/// Returns the negation of the line state.
/// Returns the line state if the state is lcs::UNKNOWN or lcs::HIGH_IMPEDENCE.
const lcs::LineState operator!(const lcs::Line &line);

#endif // __LCS_LINE_H__
