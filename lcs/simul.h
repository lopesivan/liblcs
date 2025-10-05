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

#ifndef __LCS_SIMUL_H__
#define __LCS_SIMUL_H__

#include "timeunit.h"
#include <string>

namespace lcs
{ // Start of namespace lcs

/// A class which provides static members which manage the simulation system for the digital
/// system built using libLCS.
class Simulation
{
public:

    /// Starts the simulation.
    ///
    static void start(void);

    /// Sets the stop time of the simulation. The simulation, when started using the call
    /// \p lcs::Simulation::start, runs until the stop time.
    ///
    /// \param stopTime The desired stop time in system time units.
    ///
    static void setStopTime(unsigned int stopTime);

    /// Sets the name of the VCD file which will be generated for simulation.
    ///
    /// \param filename The full desired pathname of the VCD file.
    ///
    static void setDumpFileName(const std::string &filename);

    /// Sets a unit for the system time. See \p lcs::TimeUnit to learn more about the available
    /// time units. The default is set to \p lcs::MICRO_SECOND.
    ///
    /// \param timeunit The desired unit for the system time.
    ///
    static void setTimeUnit(const TimeUnit &timeunit);

};

} // End of namespace lcs

#endif // __LCS_SIMUL_H__
