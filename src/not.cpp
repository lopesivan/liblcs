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

#include <not.h>

using namespace lcs;

Not::Not(const Bus<1> &output, const InputBus<1> &input)
    : Module(), outBus(output), inBus(input)
{
    inBus.drive(this);
}

Not::~Not()
{
    inBus.unDrive(this);
}

void Not::propogate(void)
{
    LineState line = inBus[0];
    if (line == UNKNOWN | line == LOW | line == HIGH_IMPEDENCE)
        outBus[0] = HIGH;
    else
        outBus[0] = LOW;
}
