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

#ifndef __CONTINUOUS_ASSIGNMENT_MODULE_H__
#define __CONTINUOUS_ASSIGNMENT_MODULE_H__

#include "module.h"

namespace lcs
{ // Start of namespace lcs

template <int bits>
class Bus;

/// A class (a \p lcs::Module derivative) whose objects are created as a result of a
/// continuous assignments. A continous assignment through the function \p lcs::Bus::cass
/// is translated into a module which is of type lcs::ContinuousAssignmentModule. The busses
/// involved in the continuous assignment expression are treated as input busses of
/// this module. The bus to which the continuous assignment is made is treated as the
/// output bus of the module. This module is hidden from libLCS users. The users cannot/will
/// not know about such a creation, and hence will never need to use this class explicitly.
template <int outbits, typename InExprType, unsigned int delay>
class ContinuousAssignmentModule : public Module
{
public:

    /// The only explicit and usefull constructor.
    ///
    ContinuousAssignmentModule(const Bus<outbits> &out, const InExprType &e);

    /// Destructor.
    ///
    virtual ~ContinuousAssignmentModule() {}

    /// The function which is the work horse of the module. When a line state of a line
    /// which is a part of an input bus to the module changes, that line calls this function
    /// to notify the module about the lcs::LINE_STATE_CHANGE event.
    virtual void onStateChange(int portId);

private:
    InExprType expr;
    Bus<outbits> output;
};

template <int outbits, typename InExprType, unsigned int delay>
ContinuousAssignmentModule<outbits, InExprType, delay>::
ContinuousAssignmentModule(const Bus<outbits> &out, const InExprType &e)
    : expr(e), output(out)
{
    expr.notify(this);
    onStateChange(0);
}

template <int outbits, typename InExprType, unsigned int delay>
void ContinuousAssignmentModule<outbits, InExprType, delay>::
onStateChange(int portId)
{
    int inbits = expr.width();

    if (inbits < outbits)
    {
        for (int i = 0; i < inbits; i++)
        {
            output[i] = (delay, expr[i]);
        }
    }
    else
    {
        for (int i = 0; i < outbits; i++)
        {
            output[i] = (delay, expr[i]);
        }
    }
}

} // End of namespace lcs

#endif // __CONTINUOUS_ASSIGNMENT_MODULE_H__
