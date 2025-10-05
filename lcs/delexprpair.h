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

#ifndef __LCS_DEL_EXPR_PAIR_H__
#define __LCS_DEL_EXPR_PAIR_H__

#include "linestate.h"

namespace lcs
{ // Start of namespace lcs

template <int bits, ExprType Type, typename LExprType, typename RExprType>
class Expression;

/// A class whose objects facilitate assignment delays when an expression is being assigned to
/// a bus or a bus line. This class will never need to be used by a user of libLCS directly.
///
/// See \p lcs::Expression<1, LINE_EXPR, void, void>::operator= and
/// \p lcs::Bus::operator= for more information on incorporating assignment delays.
template <int bits, ExprType Type, typename LExprType, typename RExprType>
class DelayExprPair
{
public:

    /// The only useful constructor.
    ///
    DelayExprPair(unsigned int delay, const Expression<bits, Type, LExprType, RExprType> &expr)
                 : delay_(delay), expr_(expr) {}

    /// Copy constructor.
    ///
    DelayExprPair(const DelayExprPair<bits, Type, LExprType, RExprType> &obj)
                 : delay_(obj.delay_), expr_(obj.expr_) {}

    /// Returns the delay associated with the delay-expression pair.
    ///
    unsigned int getDelay(void) const { return delay_; }

    /// Returns the expression associated with the delay-expression pair.
    ///
    const Expression<bits, Type, LExprType, RExprType> getExpr(void) const { return expr_; }

private:
    unsigned int delay_;
    Expression<bits, Type, LExprType, RExprType> expr_;
};

/// A class whose objects facilitate assignment delays when a \p lcs::LineState is being assigned
/// to a bus or a bus line. This class will never need to be used by a user of libLCS directly.
///
/// See \p lcs::Expression<1, LINE_EXPR, void, void>::operator= and
/// \p lcs::Bus::operator= for more information on incorporating assignment delays.
class DelayStatePair
{
public:

    /// The only useful constructor.
    ///
    DelayStatePair(unsigned int delay, const LineState state) : delay_(delay), state_(state) {}

    /// Copy constructor.
    ///
    DelayStatePair(const DelayStatePair &dsp) : delay_(dsp.delay_), state_(dsp.state_) {}

    /// Returns the state associated with the delay-state pair.
    ///
    const LineState getState(void) const { return state_; }

    /// Returns the delay associated with the delay-state pair.
    ///
    unsigned int getDelay(void) const { return delay_; }

private:
    unsigned int delay_;
    LineState state_;
};

} // End of namespace lcs

template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
const lcs::DelayExprPair<bits, Type, LExprType, RExprType> operator,(
                                                                unsigned int delay,
                                    const lcs::Expression<bits, Type, LExprType, RExprType> &expr
                                                                )
{
    return lcs::DelayExprPair<bits, Type, LExprType, RExprType>(delay, expr);
}

inline const lcs::DelayStatePair operator,(unsigned int delay, const lcs::LineState state)
{
    return lcs::DelayStatePair(delay, state);
}

#endif // __LCS_DEL_EXPR_PAIR_H__
