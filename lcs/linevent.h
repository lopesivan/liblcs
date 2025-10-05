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

#ifndef __LCS_LINE_EVENT_H__
#define __LCS_LINE_EVENT_H__

namespace lcs
{

/// An enumeration to define possible of lines events. A line event occurs when the state
/// of a line changes. A line event can occur in three different ways: 1. When the line
/// state changes, 2. When the state of the line is changing from \p lcs::LOW to
/// \p lcs::HIGH, and 3. When the state of the line is changing from \p lcs::HIGH to
/// \p lcs::LOW.
enum LineEvent
{
    /// The value which denotes a line state change event.
    ///
    LINE_STATE_CHANGE = 1024,

    /// The value which denotes positive edge event, or the event of the line state changing
    /// from \p lcs::LOW to \p lcs::HIGH event.
    LINE_POS_EDGE     = 1025,

    /// The value which denotes negetive edge event, or the event of the line state changing
    /// from \p lcs::HIGH to \p lcs::LOW event.
    LINE_NEG_EDGE     = 1026
};

}

#endif // __LCS_LINE_EVENT_H__
