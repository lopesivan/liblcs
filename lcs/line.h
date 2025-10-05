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

public:

    /// Returns <b>\p true </b> if the line is locked, else returns <b>\p false </b>.
    ///
    bool isLocked(void);

    /// Returns a single line \p lcs::Bus object encapsulating the line.
    ///
    Bus<1> makeBus(void);

    /// Overloaded assignment operator which sets the line state with a \p LineState
    /// variable.
    ///
    /// \param value The value to which the line state should be changed.
    ///
    void operator=(const LineState& value);

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

    /// Default constructor.
    ///
    Line(void);

    /// Copy constructor.
    ///
    Line(const Line& line);

    /// Destructor.
    ///
    virtual ~Line();

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

    /// This function locks a \p lcs::Line object for use with only that \p lcs::Module
    /// derivative which has locked the it. Since it is declared private, it cannot be used
    /// directly. One has to use \p lcs::Bus::lock instead.
    ///
    /// One can also set assignment delay  associated with the line using this function.
    /// Again, this feature cannot be used directly and one will have to use
    /// \p lcs::Bus::lock instead.
    ///
    /// \param mod The pointer to an module object (which is an instance of a derivative of
    /// \p lcs::Module class) by which the line should be locked.
    /// \param delay The assignment delay associated with the line.
    void lock(Module *mod, unsigned int delay);

    /// This functions unLocks a line from a module. Since it is declared private, it cannot
    /// be used directly. One has to use \p lcs::Bus::unLock instead. See \p lcs::Line::lock
    /// for more information.
    ///
    /// \param mod The pointer to an module object (which is an instance of a derivative of
    /// \p lcs::Module class) with which the line was locked earlier.
    void unLock(Module *mod);

    /// This is an internal function used by other functions of this class. As it is declared
    /// as private, a user of libLCS will never require to use this function.
    void setLineValue(const LineState &value);

    LineState *val, *purgedVal;
    int *refCount;
    unsigned int *delay_, *purgeTime, *purgedChangeTime;
    List< Module* > changeModList, posEdgeList, negEdgeList;
    List< int > changeModIdList, posEdgeIdList, negEdgeIdList;
    Queue<unsigned int> nextChangeTimeQueue, zeroDelayChangeTimeQueue;
    Queue<LineState> valueQueue, zeroDelayValueQueue;

    DataPtr< Module > *lockedModPtr;
};

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
