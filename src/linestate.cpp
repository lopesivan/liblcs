//////////////////////////////////////////////////////////////////////////////////
// This file is distributed as part of the libLCS library.
// libLCS is C++ Logic Circuit Simulation library.
//
// Copyright (c) 2006-2007, B. R. Siva Chandra, India
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// In case you would like to contact the author, use the following e-mail
// address: sivachandra_br@yahoo.com
//////////////////////////////////////////////////////////////////////////////////

#include <linestate.h>

using namespace lcs;
using namespace std;

const LineState operator&(const LineState& op1, const LineState& op2)
{
    if (op1 == LOW && op2 == LOW)
        return LOW;
    else if (op1 == LOW && op2 == HIGH)
        return LOW;
    else if (op1 == HIGH && op2 == LOW)
        return LOW;
    else if (op1 == HIGH && op2 == HIGH)
        return HIGH;
    else
        return UNKNOWN;
}

const LineState operator|(const LineState& op1, const LineState& op2)
{
    if (op1 == LOW && op2 == LOW)
        return LOW;
    else if (op1 == LOW && op2 == HIGH)
        return HIGH;
    else if (op1 == HIGH && op2 == LOW)
        return HIGH;
    else //if (op1 == HIGH && op2 == HIGH)
        return HIGH;
}

const LineState operator^(const LineState& op1, const LineState& op2)
{
    if (op1 == LOW && op2 == LOW)
        return LOW;
    else if (op1 == LOW && op2 == HIGH)
        return HIGH;
    else if (op1 == HIGH && op2 == LOW)
        return HIGH;
    else //if (op1 == HIGH && op2 == HIGH)
        return LOW;
}

const lcs::LineState operator!(const lcs::LineState &s)
{
    if (s == HIGH)
        return LOW;
    else if (s == LOW)
        return HIGH;
    else
        return s;
}
