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

#ifndef __LCS_CLOCK_H__
#define __LCS_CLOCK_H__

#include "inbus.h"
#include "nullclock_exception.h"
#include "multclock_exception.h"
#include "tickListener.h"

namespace lcs
{ // Start of namespace lcs

/// This is a clock class which should be used as a pulse generator for the digital system
/// designed using libLCS. Currently, the implementation is such that it permits the use of
/// only one clock in a system. This feature is incorporated by making the default constructor
/// private and providing a static member function which returns a \p Clock object. The
/// returned \p Clock object is instantiated by a shallow copy-constructor.
///
/// The pulse width of the clock can be altered using the \p lcs::Clock::setPulseWidth
/// function. The pulse width is measured in system time units. By default, the pulse
/// width is set to 100 system time units.
///
/// See \p lcs::SystemTimer for more information.
class Clock : public InputBus<1>
{
public:

    /// Copy constructor.
    ///
    Clock(const Clock& clk);

    /// Destructor.
    ///
    ~Clock();

    /// Returns a clock object. The returned object is a shallow copy of the single
    /// \p Clock object which can exist.
    static Clock getClock(void) throw (NullClockException);

    /// This function is used by \p lcs::SystemTimer to notify the clock of the system time.
    /// This function will never need to be used by a user of libLCS.
    virtual void tick();

    /// Sets the pulse width of the clock in system time units.
    ///
    /// \param width The desired pulse width in system time units.
    static void setPulseWidth(unsigned int width);

    /// This function can used by a \p lcs::TickListener derivative to get notified
    /// of a clock state change.
    static void notifyTick(TickListener *tl);

    /// A \p lcs::TickListener derivative which registered to be notified of a clock
    /// state change should de-register using this function when notification is no longer
    /// required. This function SHOULD be called just before the object ceases to
    /// exist in the simulation system. Else, a segmentation fault is bound to occur.
    static void stopTickNotification(TickListener *tl);

private:

    Clock() throw (MultipleClockException);

    static Clock *clock;

    static unsigned int *pulsewidth;
    static List<TickListener*> *tlList;
};

} // End of namespace lcs

#endif // __LCS_CLOCK_H__
