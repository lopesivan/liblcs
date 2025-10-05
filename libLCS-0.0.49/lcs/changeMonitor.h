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

#ifndef __LCS_CHANGE_MONITOR_H__
#define __LCS_CHANGE_MONITOR_H__

#include "module.h"
#include "clock.h"
#include "systime.h"
#include "vcdman.h"
#include <string>

namespace lcs
{

/// This a class which can be used to monitor bus line states. A \p lcs::ChangeMonitor
/// class should be created using the constructor which takes a bus object as one of its
/// parameters. At every system time instance, the \p lcs::ChangeMonitor object records
/// line states of the bus with which it was instanstiated. If line states at the current
/// time instant differ from line states at the previous time instant, \p lcs::ChangeMonitor
/// object displays the new bus line states onto the \p stdout device.
///
/// \param width The width of the bus whose lines should be monitored.
template <int width = 1>
class ChangeMonitor : public TickListener
{
public:

    /// The only explicit constructor. Other constructors synthesized by the compiler
    /// are practically useless and should be avoided.
    ///
    /// \param b The bus whose lines should be monitored for a change.
    /// \param name The name with which the bus will be referred to in displaying the
    /// line states. If the values need to be dumped, then the name SHOULD NOT have
    /// spaces in it.
    /// \param dumpon If this parameter is \p true, then the line states of the bus being
    /// monitored are dumped into a VCD file.
    ChangeMonitor(const InputBus<width> &b, const std::string &name, DumpOption opt = DUMP_OFF);

    /// Destructor.
    ///
    virtual ~ChangeMonitor();

    /// A user of libLCS will never need to use this function. It is used by the system
    /// time keeper to notify a \p lcs::ChangeMonitor object about an occurance of a time
    /// step.
    virtual void onTick(void);

private:
    InputBus<width> bus;
    std::string busName;
    std::string old_value;
    DumpOption dump;
};

template <int width>
ChangeMonitor<width>::ChangeMonitor(const InputBus<width> &b, const std::string &name, DumpOption opt)
        : TickListener(), bus(b), busName(name), dump(opt)
{
    SystemTimer::notifyTick(this);
    old_value = bus.toStr();

    if (dump == DUMP_ON)
        VCDManager::registerChangeReporter(name, bus);
}

template <int width>
ChangeMonitor<width>::~ChangeMonitor()
{
    SystemTimer::stopTickNotification(this);
}

template <int width>
void ChangeMonitor<width>::onTick(void)
{
    unsigned int time = SystemTimer::getTime();

    std::string currValue = bus.toStr();

    if (currValue != old_value)
    {
        std::cout << "At time: " << time << ",\t" << busName << ": " << bus << std::endl;

        if (dump == DUMP_ON)
            VCDManager::noteChange(busName, bus);
    }

    old_value = currValue;
}

}

#endif // __LCS_CHANGE_MONITOR_H__
