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

#ifndef __LCS_LINE_STATE_H__
#define __LCS_LINE_STATE_H__

#include <iostream>

namespace lcs
{ // Start of namespace lcs

/// An enumerated set of data types which denote the state of a data line. A line can have
/// four possible states: LOW, HIGH, UNKNOWN, HIGH_IMPEDENCE. Each of these states denote a
/// single bit value.Click on each state to know more about them.
enum LineState
{
    /// A state which denotes a logic level of 0.
    /// It is displayed on the screen as the character '0'.
    LOW                   =  0 ,

    /// A state which denotes a logic level of 1.
    /// It is displayed on the screen as the character '1'.
    HIGH                  =  1 ,

    /// A state which denotes an unknown logic level.
    /// It is displayed on the screen as the character 'x'.
    UNKNOWN               =  2,

    /// A state which denotes a high imepedence line.
    /// It is displayed on the screen as the character 'z'.
    HIGH_IMPEDENCE        =  3
};

} // End of namespace lcs

/// \fn
/// An overloaded operator to perform a bitwise AND operation.
///
const lcs::LineState operator&(const lcs::LineState& op1, const lcs::LineState& op2);

/// \fn
/// An overloaded operator to perform a bitwise OR operation.
///
const lcs::LineState operator|(const lcs::LineState& op1, const lcs::LineState& op2);

/// \fn
/// An overloaded operator to perform a bitwise XOR operation.
///
const lcs::LineState operator^(const lcs::LineState& op1, const lcs::LineState& op2);

/// \fn
/// An overloaded operator to perform a bitwise NOT operation.
/// The opposite of the current line state is returned.
const lcs::LineState operator~(const lcs::LineState &s);

#endif // __LCS_LINE_STATE_H__
