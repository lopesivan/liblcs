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

#ifndef __LCS_BIT_H__
#define __LCS_BIT_H__

#include "linestate.h"
#include "exprtype.h"
#include "list.h"
#include "module.h"

namespace lcs
{ // Start of namespace lcs

template <int w, ExprType Type, typename LExprType, typename RExprType>
class Expression;

template <int bits>
class BitBuff;

/// A class which is denotes a single bit. A bit object holds a \p lcs::LineState value.
/// Users of libLCS will not need to directly use \p Bit objects under normal circumstances. One has
/// to use objects of class \p lcs::BitBuffer, which bundles one or more \p Bit objects, as neccesary.
class Bit
{

template <int bits>
friend class BitBuff;

public:

    /// The default constructor.
    ///
    Bit(void);

    /// The copy constructor.
    ///
    Bit(const Bit &bit);

    /// Destructor.
    ///
    ~Bit();

    /// An assignment operator to assign the bit state with a \p lcs::LineState variable.
    ///
    /// \param rhs The RHS \p lcs::LineState variable for the assignment operator.
    void operator=(const LineState &rhs);

    /// Assigns the bit state with an expression object.
    ///
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    inline void operator=(const Expression<w, Type, LExprType, RExprType> &rhs) { operator=(rhs[0]); }

    /// Returns the logic state of the bit.
    ///
    const LineState state(void) { return *state_; }

private:

    /// The assignment operator.
    ///
    Bit& operator=(const Bit &rhs);

    /// A \p lcs::Module (or its derivative) should register to be notified by the bit about
    /// a bit state change using this function. However, since this function is declared as a
    /// private member, one cannot use this function directly. See \p lcs::BitBuff::notify
    /// for more information. The \p lcs::Bit::notify is called by the function
    /// \p lcs::BitBuff::notify.
    void notify(Module *mod);

    /// A lcs::Module (or its derivative) can de-register itself from being notified by the
    /// bit using this function. See \p lcs::Bit::notify for more information. As with
    /// \p lcs::Bit::notify, even this function is declared private and hence cannot be
    /// used directly. One has to use \p lcs::BitBuff::stopNotification, which internally makes
    /// a call to \p lcs::Bit::stopNotification.
    void stopNotification(Module *mod);

private:
    LineState *state_;
    int *refcount_;

    List<Module*> modList;
};

} // End of namespace lcs

#endif // __LCS_BIT_H__
