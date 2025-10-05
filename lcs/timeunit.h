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

#ifndef __LCS_TIME_UNIT_H__
#define __LCS_TIME_UNIT_H__

namespace lcs
{ // Start of namespace lcs

/// An enumeration which specifies types for the unit which can be associated with the
/// system time.
enum TimeUnit
{

    /// Indicates a femto second.
    ///
    FEMTO_SECOND   = 4096,

    /// Indicates a pico second.
    ///
    PICO_SECOND    = 4097,

    /// Indicates a nano second.
    ///
    NANO_SECOND    = 4098,

    /// Indicates a micro second.
    ///
    MICRO_SECOND   = 4099,

    /// Indicates a milli second.
    ///
    MILLI_SECOND   = 4100,

    /// Indicates a second.
    ///
    SECOND         = 4101
};

} // End of namespace lcs

#endif // __LCS_TIME_UNIT_H__
