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

#ifndef __LCS_TICK_LISTENER_H__
#define __LCS_TICK_LISTENER_H__

namespace lcs
{ // Start of namespace lcs

/// An abstract base class which should be subclassed to obtain class whose objects
/// can be notified of \p lcs::Clock pulses or \p lcs::SystemTimer time ticks.
class TickListener
{
public:

    /// Default constructor.
    ///
    TickListener() {}

    /// Destructor.
    ///
    virtual ~TickListener() {}

    /// This should be overloaded by derived classes. This function will be used to
    /// notify the \p lcs::Clock state changes or \p lcs::SystemTimer time ticks.
    virtual void onTick(void) = 0;
};

} // End of namespace lcs

#endif //  __LCS_TICK_LISTENER_H__
