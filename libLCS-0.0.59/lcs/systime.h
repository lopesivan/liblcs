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

#ifndef __LCS_SYS_TIME_H__
#define __LCS_SYS_TIME_H__

#include "list.h"
#include "queue.h"
#include "tickListener.h"

namespace lcs
{ // Start of namespace lcs

class Clock;
class Line;

/// A class which manages the timing for the simulation of the digital system built using
/// libLCS. This class is a time keeper which keeps the various other objects in the system
/// updated of the system time.
class SystemTimer
{

/// \p lcs::Simulation is friend of this class and manages the duties of starting and
/// stopping the system time keeper.
friend class Simulation;

public:

    /// Returns the system time in system time units
    ///
    static unsigned int getTime(void);

    /// An object which is an instance of a derivative of the class \p lcs::TickListener
    /// can register with the \p SystemTimer to be notified of time ticks using this
    /// function.
    ///
    /// \param tl Pointer to the object which is an instance of a derivative of the class
    /// \p lcs::TickListener
    ///
    static void notifyTick(TickListener *tl);

    /// A \p lcs::Line object should request for time tick notification through this
    /// function.
    ///
    /// \param l The pointer to a \p lcs::Line object which desires to be notified of
    /// time ticks.
    static void notifyLine(List<Line*> *refList);

    // This function is used internally to simulate concurrancy of various modules.
    // For example, it is used by the class Line to update its state if it were an output
    // line of a module which has zero propogation delay.
    static void notifyHiddenTick(List<Line*> *refList);

    // This function is used by zero delay Line objects. It returns the time of the
    // hidden timer.
    static unsigned int getHiddenTime(void);

    /// A \p lcs::Line object which requested time tick notification should de-register
    /// through a call to this function when it does not need tick notification anymore.
    ///
    /// \param ln The pointer to a \p lcs::Line object which wants to de-register.
    static void stopLineNotification(List<Line*> *refList);

    /// The singleton \p lcs::Clock class object requests time tick notification through
    /// this function call.
    static void notifyClock(Clock *clk);

    /// The signleton \p lcs::Clock class object de-registers from being notified of a
    /// time tick through this function call.
    static void stopClockNotification(void);

    /// A \p lcs::TickListener derivative which registered to be notified of a time
    /// tick should de-register using this function when notification is no longer
    /// required. Hence, this function should be called before the object ceases to
    /// exist in the simulation system. Else, a segmentation fault is bound to occur.
    static void stopTickNotification(TickListener *tl);

private:

    static void start(void);

    static void reset(void);

    static void setStopTime(const unsigned int stopTime);

    static void updateHiddenDelays(void);

    static unsigned int stoptime;

    static unsigned int time;

    static unsigned int hiddenTime;

    static Clock *clock;

    static List< TickListener* > tlList;

    static List< List<Line*>* > refLists;

    static Queue< List<Line*>* > hiddenQueue;
};

} // End of namespace lcs

#endif // __LCS_SYS_TIME_H__
