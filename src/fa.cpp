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

#include <fa.h>

using namespace lcs;

FullAdder::FullAdder(const Bus<1> &S, const Bus<1> Cout, const InputBus<1> &A,
                     const InputBus<1> &B, const InputBus<1> &Cin)
         : Module(), s(S), cout(Cout), a(A), b(B), c(Cin)
{
    a.drive(this);
    b.drive(this);
    c.drive(this);
}

FullAdder::~FullAdder()
{
    a.unDrive(this);
    b.unDrive(this);
    c.unDrive(this);
}

void FullAdder::propogate(void)
{
    LineState sum = (!a[0] & !b[0] & c[0]) | (!a[0] & b[0] & !c[0]) |
                    (a[0] & !b[0] & !c[0]) | (a[0] & b[0] & c[0]) ,
              carry = (!a[0] & b[0] & c[0]) | (a[0] & !b[0] & c[0]) |
                      (a[0] & b[0] & !c[0]) | (a[0] & b[0] & c[0]) ;

    s[0] = sum;
    cout[0] = carry;
}
