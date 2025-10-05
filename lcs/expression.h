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

#ifndef __LCS_EXPRESSION_H__
#define __LCS_EXPRESSION_H__

#include "bit.h"
#include "line.h"
#include "exprtype.h"
#include "linestate.h"
#include "outofrange_exception.h"
#include "delexprpair.h"
#include "contassmod.h"
#include "hiddenmodman.h"
#include <iostream>

namespace lcs
{ // Start of namespace lcs

template <int bits>
class BitBuff;

template <int bits>
class InputBus;

template <int bits>
class Bus;

class Module;

// ###############################################################################
// ###############################################################################

///
///
template <ExprType>
inline const LineState binaryLineStateOperation(const LineState &l, const LineState &r);

///
///
template <>
inline const LineState binaryLineStateOperation<BITWISE_AND_EXPR>(
                                            const LineState &l, const LineState &r
                                                               )
{
    return l&r;
}

///
///
template <>
inline const LineState binaryLineStateOperation<BITWISE_OR_EXPR>(
                                            const LineState &l, const LineState &r
                                                              )
{
    return l|r;
}

///
///
template <>
inline const LineState binaryLineStateOperation<BITWISE_XOR_EXPR>(
                                            const LineState &l, const LineState &r
                                                               )
{
    return l^r;
}

// ################################################################################
// GENERIC Expression class
// ################################################################################

/// A class whose objects are created as temporary objects during operations
/// on \p lcs::Bus and \p InputBus objects. A user of libLCS will never need to use this
/// class explicitly. Implicitly, this class is used everytime a continuous assignment is
/// requested using the function \p lcs::Bus::cass.
template <int bits, ExprType Type, typename LExprType, typename RExprType>
class Expression
{
public:

    /// The only usefull constructor. Default constructor is practically useless.
    ///
    Expression(const LExprType &le, const RExprType &re);

    /// Copy constructor.
    ///
    Expression(const Expression<bits, Type, LExprType, RExprType> &e);

    /// Returns the bit state (which is a result of the operation performed by the
    /// expression) at the index specified.
    const LineState operator[](int index) const throw (OutOfRangeException<int>);

    /// Returns the width for which an \p lcs::Expression object is valid.
    ///
    int width() const { return bits; }

    /// This function can be used by a \p lcs::Module derivative to be notified of line
    /// state changes in the busses used in the expression.
    void notify(Module *mod) { lExpr.notify(mod); rExpr.notify(mod); }

private:
    LExprType lExpr;
    RExprType rExpr;
};

template <int bits, ExprType Type, typename LExprType, typename RExprType>
Expression<bits, Type, LExprType, RExprType>::Expression(
                                                const LExprType &le, const RExprType &re
                                                        )
    : lExpr(le), rExpr(re)
{}

template <int bits, ExprType Type, typename LExprType, typename RExprType>
Expression<bits, Type, LExprType, RExprType>::Expression(
                  const Expression<bits, Type, LExprType, RExprType> &e
                                                                     )
    : lExpr(e.lExpr), rExpr(e.rExpr)
{}

template <int bits, ExprType Type, typename LExprType, typename RExprType>
const LineState Expression<bits, Type, LExprType, RExprType>::operator[](int i)
            const throw (OutOfRangeException<int>)
{
    if (i >= bits || i < 0)
        throw OutOfRangeException<int>(0, bits-1, i);

    int lwidth = lExpr.width(), rwidth = rExpr.width();

    if (i < lwidth && i < rwidth)
        return binaryLineStateOperation<Type>(lExpr[i], rExpr[i]);
    else if (lwidth < rwidth)
        return rExpr[i];
    else if (rwidth < lwidth)
        return lExpr[i];
    else
        return UNKNOWN;

}

// #####################################################################################
// BUS_EXPR
// #####################################################################################

/// A class whose objects are created as temporary objects corresponding to an expression
/// consisting of only an \p lcs::InputBus object. A user of libLCS will never need to use this
/// class explicitly. Implicitly, this class is used everytime a continuous assignment is
/// requested using the function \p lcs::Bus::cass.
template <int bits>
class Expression<bits, BUS_EXPR, void, void>
{
public:

    /// The only usefull constructor. The default constructor is practically useless.
    ///
    Expression(const InputBus<bits> &b);

    /// Copy Constructor.
    ///
    Expression(const Expression<bits, BUS_EXPR, void, void> &e);

    /// This function can be used by a \p lcs::Module derivative to be notified of line
    /// state changes in the busses used in the expression.
    void notify(Module *mod) { bus.notify(mod, LINE_STATE_CHANGE, 0); }

    /// Returns the bit state (which is a result of the operation performed by the
    /// expression) at the index specified.
    const LineState operator[](int index) const throw (OutOfRangeException<int>);

    /// Returns the width for which an \p lcs::Expression object is valid.
    ///
    int width() const { return bits; }

private:
    InputBus<bits> bus;
};

template <int bits>
Expression<bits, BUS_EXPR, void, void>::Expression(const InputBus<bits> &b)
        : bus(b) {}

template <int bits>
Expression<bits,BUS_EXPR,void,void>::Expression(const Expression<bits,BUS_EXPR,void,void> &e)
        : bus(e.bus) {}

template <int bits>
const LineState Expression<bits,BUS_EXPR,void,void>::operator[](int index)
                                                    const throw (OutOfRangeException<int>)
{
    Line *data = bus.dataPtr->data;
    Line line = data[index];
    return line();
}

// #####################################################################################
// BITBUFF_EXPR
// #####################################################################################

/// A class whose objects are created as temporary objects corresponding to an expression
/// consisting of only an \p lcs::BitBuffer object. A user of libLCS will never need to use this
/// class explicitly. Implicitly, this class is used everytime a continuous assignment is
/// requested using the function \p lcs::Bus::cass.
template <int bits>
class Expression<bits, BITBUFF_EXPR, void, void>
{
public:
    Expression(const BitBuff<bits> &buf);

    /// Copy constructor.
    ///
    Expression(const Expression<bits, BITBUFF_EXPR, void, void> &e);

    /// Returns the bit state (which is a result of the one's complement operation performed
    /// by the expression) at the index specified.
    const LineState operator[](int index) const throw (OutOfRangeException<int>);

    /// Assignment operator for assignment with an expression.
    ///
    template <int w, ExprType Type, typename LExprType, typename RExprType>
    void operator=(const Expression<w, Type, LExprType, RExprType> &expr);

    /// Assignment operator for assignment with a similar expression.
    ///
    void operator=(const Expression<bits, BITBUFF_EXPR, void, void> &expr);

    /// Assignment operator for assignment from with a \p lcs::LineState value.
    ///
    void operator=(const LineState &l);

    /// Returns the width for which an \p lcs::Expression object is valid.
    ///
    int width(void) const { return bits; }

    /// This function can be used by a \p lcs::Module derivative to be notified of line
    /// state changes in the busses used in the expression.
    void notify(Module *mod) { buf.notify(mod); }

private:
    BitBuff<bits> buf;
};

template <int bits>
Expression<bits, BITBUFF_EXPR, void, void>::Expression(const BitBuff<bits> &b)
        : buf(b) {}

template <int bits>
Expression<bits,BITBUFF_EXPR,void,void>::Expression(const Expression<bits,BITBUFF_EXPR,void,void> &e)
        : buf(e.buf) {}

template <int bits>
const LineState Expression<bits,BITBUFF_EXPR,void,void>::operator[](int index)
                                                    const throw (OutOfRangeException<int>)
{
    if (index < 0 || index >= bits)
        throw OutOfRangeException<int>(0, bits, index);

    Bit *data = buf.dataPtr->data;
    Bit b = data[index];
    return b.state();
}

template <int bits>
template <int w, ExprType Type, typename LExprType, typename RExprType>
void Expression<bits,BITBUFF_EXPR,void,void>::operator=(const Expression<w,Type,LExprType,RExprType> &expr)
{
    Bit *data = buf.dataPtr->data;

    if (bits < w)
    {
        for (int i = 0; i < bits; i++)
            data[i] = expr[i];
    }
    else
    {
        for (int i = 0; i < w; i++)
            data[i] = expr[i];
    }
}

template <int bits>
void Expression<bits,BITBUFF_EXPR,void,void>::operator=(
                                const Expression<bits, BITBUFF_EXPR, void, void> &expr
                                                        )
{
    Bit *data = buf.dataPtr->data;

    for (int i = 0; i < bits; i++)
        data[i] = expr[i];
}

template <int bits>
void Expression<bits,BITBUFF_EXPR,void,void>::operator=(const LineState &l)
{
    Bit *data = buf.dataPtr->data;

    for (int i = 0; i < bits; i++)
        data[i] = l;
}

// #####################################################################################
// ONES_COMPLIMENT_EXPR
// #####################################################################################

/// A class whose objects are created as temporary objects which are a result of a one's
/// compliment operation. A user of libLCS will never need to use this
/// class explicitly. Implicitly, this class is used everytime a continuous assignment is
/// requested using the function \p lcs::Bus::cass.
template <int bits, typename InExprType>
class Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType>
{
public:

    /// The only usefull constructor. The default constructor is practically useless.
    ///
    Expression(const InExprType &e);

    /// Copy constructor.
    ///
    Expression(const Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType> &e);

    /// Returns the bit state (which is a result of the one's complement operation performed
    /// by the expression) at the index specified.
    const LineState operator[](int index) const throw (OutOfRangeException<int>);

    /// Returns the width for which an \p lcs::Expression object is valid.
    ///
    int width(void) const { return bits; }

    /// This function can be used by a \p lcs::Module derivative to be notified of line
    /// state changes in the busses used in the expression.
    void notify(Module *mod) { inExpr.notify(mod); }

public:
    InExprType inExpr;
};

template <int bits, typename InExprType>
Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType>::Expression(const InExprType &e)
            : inExpr(e)
{}

template <int bits, typename InExprType>
Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType>::Expression(
                            const Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType> &e
                                                                    )
            : inExpr(e.inExpr)
{}

template <int bits, typename InExprType>
const LineState Expression<bits, ONES_COMPLEMENT_EXPR, void, InExprType>::
                            operator[](int i) const throw (OutOfRangeException<int>)
{
    if (i >= bits || i < 0)
        throw OutOfRangeException<int>(0, bits-1, i);

    return ~inExpr[i];
}

// #####################################################################################
// #####################################################################################

} // End of namespace lcs

///
///
template <int bits, lcs::ExprType InType, typename InLExprType, typename InRExprType>
const lcs::Expression<
                        bits,
                        lcs::ONES_COMPLEMENT_EXPR,
                        void,
                        lcs::Expression<bits, InType, InLExprType, InRExprType>
                     >
operator~(const lcs::Expression<bits, InType, InLExprType, InRExprType> &expr)
{
    lcs::Expression<
                    bits,
                    lcs::ONES_COMPLEMENT_EXPR,
                    void,
                    lcs::Expression<bits, InType, InLExprType, InRExprType>
                   > e(expr);

    return e;
}

///
///
template <int lbits, int rbits, lcs::ExprType LExprType, lcs::ExprType RExprType,
          typename LLExpr, typename LRExpr, typename RLExpr, typename RRExpr>
const lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_AND_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                     >
operator&(
              const lcs::Expression<lbits, LExprType, LLExpr, LRExpr> &lExpr,
              const lcs::Expression<rbits, RExprType, RLExpr, RRExpr> &rExpr
         )
{
    lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_AND_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <int lbits, int rbits, lcs::ExprType LExprType, lcs::ExprType RExprType,
          typename LLExpr, typename LRExpr, typename RLExpr, typename RRExpr>
const lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_OR_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                     >
operator|(
              const lcs::Expression<lbits, LExprType, LLExpr, LRExpr> &lExpr,
              const lcs::Expression<rbits, RExprType, RLExpr, RRExpr> &rExpr
         )
{
    lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_OR_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <int lbits, int rbits, lcs::ExprType LExprType, lcs::ExprType RExprType,
          typename LLExpr, typename LRExpr, typename RLExpr, typename RRExpr>
const lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_XOR_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                     >
operator^(
              const lcs::Expression<lbits, LExprType, LLExpr, LRExpr> &lExpr,
              const lcs::Expression<rbits, RExprType, RLExpr, RRExpr> &rExpr
         )
{
    lcs::Expression<
                       (lbits > rbits ? lbits : rbits),
                       lcs::BITWISE_XOR_EXPR,
                       lcs::Expression<lbits, LExprType, LLExpr, LRExpr>,
                       lcs::Expression<rbits, RExprType, RLExpr, RRExpr>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <int bits>
const lcs::Expression<
                        bits,
                        lcs::ONES_COMPLEMENT_EXPR,
                        void,
                        lcs::Expression<bits, lcs::BUS_EXPR, void, void>
                     >
operator~(const lcs::InputBus<bits> &bus)
{
    lcs::Expression<bits, lcs::BUS_EXPR, void, void> busExpr(bus);
    lcs::Expression<
                        bits,
                        lcs::ONES_COMPLEMENT_EXPR,
                        void,
                        lcs::Expression<bits, lcs::BUS_EXPR, void, void>
                   > e(busExpr);

    return e;
}

///
///
template <int bits>
const lcs::Expression<
                        bits,
                        lcs::ONES_COMPLEMENT_EXPR,
                        void,
                        lcs::Expression<bits, lcs::BITBUFF_EXPR, void, void>
                     >
operator~(const lcs::BitBuff<bits> &buf)
{
    lcs::Expression<bits, lcs::BITBUFF_EXPR, void, void> buffExpr(buf);
    return ~buffExpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator&(const lcs::InputBus<lbits> &lbus, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                    (lbits > rbits ? lbits : rbits),
                    lcs::BITWISE_AND_EXPR,
                    lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                    lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator|(const lcs::InputBus<lbits> &lbus, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                    (lbits > rbits ? lbits : rbits),
                    lcs::BITWISE_OR_EXPR,
                    lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                    lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator^(const lcs::InputBus<lbits> &lbus, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                    (lbits > rbits ? lbits : rbits),
                    lcs::BITWISE_XOR_EXPR,
                    lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                    lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator&(const lcs::InputBus<lbits> &lbus,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator&(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator^(const lcs::InputBus<lbits> &lbus,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator^(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator|(const lcs::InputBus<lbits> &lbus,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lExpr(lbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator|(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rExpr(rbus);

    lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                   > e(lExpr, rExpr);

    return e;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator&(const lcs::BitBuff<lbits> &lbuf,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lExpr(lbuf);
    return lExpr&rExpr;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_AND_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator&(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::BitBuff<rbits> &rbuf)
{
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rExpr(rbuf);
    return lExpr&rExpr;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator^(const lcs::BitBuff<lbits> &lbuf,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lExpr(lbuf);
    return lExpr^rExpr;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_XOR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator^(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::BitBuff<rbits> &rbuf)
{
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rExpr(rbuf);
    return lExpr^rExpr;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType RExprType,
            typename RLExprType, typename RRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                        lcs::Expression<rbits, RExprType, RLExprType, RRExprType>
                     >
operator|(const lcs::BitBuff<lbits> &lbuf,
          const lcs::Expression<rbits, RExprType, RLExprType, RRExprType> &rExpr)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lExpr(lbuf);
    return lExpr|rExpr;
}

///
///
template <
            int lbits, int rbits, lcs::ExprType LExprType,
            typename LLExprType, typename LRExprType
         >
const lcs::Expression<
                        (lbits > rbits ? lbits : rbits),
                        lcs::BITWISE_OR_EXPR,
                        lcs::Expression<lbits, LExprType, LLExprType, LRExprType>,
                        lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator|(const lcs::Expression<lbits, LExprType, LLExprType, LRExprType> &lExpr,
          const lcs::BitBuff<rbits> &rbuf)
{
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rExpr(rbuf);
    return lExpr|rExpr;
}

///
///
template <int lbits, int rbits, lcs::ExprType LExprType, lcs::ExprType RExprType,
          typename LLExpr, typename LRExpr, typename RLExpr, typename RRExpr>
bool operator==(const lcs::Expression<lbits, LExprType, LLExpr, LRExpr> &lhs,
                const lcs::Expression<rbits, RExprType, RLExpr, RRExpr> &rhs)
{
    if (lbits != rbits)
        return false;

    bool e = true;
    for (int i = 0; i < lbits; i++)
    {
        if (lhs[i] != rhs[i])
        {
            e = false;
            break;
        }
    }

    return e;
}

///
///
template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
bool operator==(const lcs::LineState &lhs,
                const lcs::Expression<bits, Type, LExprType, RExprType> &rhs)
{
    if (bits > 1)
        return false;

    if (rhs[0] == lhs)
        return true;
    else
        return false;
}

///
///
template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
bool operator==(const lcs::Expression<bits, Type, LExprType, RExprType> &lhs,
                            const lcs::LineState &rhs)
{
    if (bits > 1)
        return false;

    if (lhs[0] == rhs)
        return true;
    else
        return false;
}

///
///
template <int lbits, int rbits, lcs::ExprType LExprType, lcs::ExprType RExprType,
          typename LLExpr, typename LRExpr, typename RLExpr, typename RRExpr>
bool operator!=(const lcs::Expression<lbits, LExprType, LLExpr, LRExpr> &lhs,
                const lcs::Expression<rbits, RExprType, RLExpr, RRExpr> &rhs)
{
    if (lbits != rbits)
        return true;

    bool e = false;
    for (int i = 0; i < lbits; i++)
    {
        if (lhs[i] != rhs[i])
        {
            e = true;
            break;
        }
    }

    return e;
}

///
///
template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
bool operator!=(const lcs::LineState &lhs,
                const lcs::Expression<bits, Type, LExprType, RExprType> &rhs)
{
    if (bits > 1)
        return true;

    if (rhs[0] != lhs)
        return true;
    else
        return false;
}

///
///
template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
bool operator!=(const lcs::Expression<bits, Type, LExprType, RExprType> &lhs,
                            const lcs::LineState &rhs)
{
    if (bits > 1)
        return true;

    if (lhs[0] != rhs)
        return true;
    else
        return false;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_AND_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator&(const lcs::BitBuff<lbits> &lbuff, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr&rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_OR_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator|(const lcs::BitBuff<lbits> &lbuff, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr|rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_XOR_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator^(const lcs::BitBuff<lbits> &lbuff, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr^rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_AND_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator&(const lcs::BitBuff<lbits> &lbuff, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rexpr(rbus);

    return lexpr&rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_OR_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator|(const lcs::BitBuff<lbits> &lbuff, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rexpr(rbus);

    return lexpr|rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_XOR_EXPR,
                         lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BUS_EXPR, void, void>
                     >
operator^(const lcs::BitBuff<lbits> &lbuff, const lcs::InputBus<rbits> &rbus)
{
    lcs::Expression<lbits, lcs::BITBUFF_EXPR, void, void> lexpr(lbuff);
    lcs::Expression<rbits, lcs::BUS_EXPR, void, void> rexpr(rbus);

    return lexpr^rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_AND_EXPR,
                         lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator&(const lcs::InputBus<lbits> &lbus, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lexpr(lbus);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr&rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_OR_EXPR,
                         lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator|(const lcs::InputBus<lbits> &lbus, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lexpr(lbus);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr|rexpr;
}

///
///
template <int lbits, int rbits>
const lcs::Expression<
                         (lbits > rbits ? lbits : rbits),
                         lcs::BITWISE_XOR_EXPR,
                         lcs::Expression<lbits, lcs::BUS_EXPR, void, void>,
                         lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void>
                     >
operator^(const lcs::InputBus<lbits> &lbus, const lcs::BitBuff<rbits> &rbuff)
{
    lcs::Expression<lbits, lcs::BUS_EXPR, void, void> lexpr(lbus);
    lcs::Expression<rbits, lcs::BITBUFF_EXPR, void, void> rexpr(rbuff);

    return lexpr^rexpr;
}

///
///
template <int bits, lcs::ExprType Type, typename LExprType, typename RExprType>
std::ostream& operator<<(const std::ostream& osobj,
                                const lcs::Expression<bits, Type, LExprType, RExprType> &expr)
{
    lcs::LineState l;

    for (int i = 0; i < bits-1; i++)
    {
        l = expr[bits-1-i];

        if (l == lcs::LOW)
            std::cout << '0';
        else if (l == lcs::HIGH)
            std::cout << '1';
        else if (l == lcs::UNKNOWN)
            std::cout << 'x';
        else if (l == lcs::HIGH_IMPEDENCE)
            std::cout << 'z';
    }

    l = expr[0];
    if (l == lcs::LOW)
        return std::cout << '0';
    else if (l == lcs::HIGH)
        return std::cout << '1';
    else if (l == lcs::UNKNOWN)
        return std::cout << 'x';
    else
        return std::cout << 'z';
}

#endif // __LCS_EXPRESSION_H__
