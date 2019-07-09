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

#ifndef __LCS_EXPR_TYPE_H__
#define __LCS_EXPR_TYPE_H__

namespace lcs
{ // Start of namespace lcs.

/// An enumeration enumerating types for the various kinds of operations which can
/// be used in an expression. The expression should consist of operands of type \p lcs::InputBus (and
/// hence, can be of type \p lcs::Bus) or \p lcs::BitBuffer, or bit-selects of busses or bit buffers.
enum ExprType
{
    /// Indicates an expression which consists of a single bus.
    ///
    BUS_EXPR               = 3072,

    /// Indicates an expression which is a bitwise AND operation.
    ///
    BITWISE_AND_EXPR       = 3073,

    /// Indicates an expression which is a bitwise OR operation.
    ///
    BITWISE_OR_EXPR        = 3074,

    /// Indicates an expression which is a bitwise XOR operation.
    ///
    BITWISE_XOR_EXPR       = 3075,

    /// Indicates an expression which is a one's complement operation.
    ///
    ONES_COMPLEMENT_EXPR   = 3076,

    /// Indicates an expression which consists of a single bit buffer.
    ///
    BITBUFF_EXPR           = 3077,
};

} // End of namespace lcs.

#endif // __LCS_EXPR_TYPE_H__
