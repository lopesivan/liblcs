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

#include <bus.h>

using namespace lcs;
using namespace std;

void Line::drive(Module *mod)
{
    if (mod != NULL)
    {
        modList.append(mod);
    }
}

void Line::unDrive(Module *mod)
{
    modList.remove(mod);
}

Bus<1> Line::makeBus(void)
{
    Bus<1> bus;
    bus[0] = *this;

    return bus;
}

const Line& Line::operator=(const LineState& value)
{
    if (*val != value)
    {
        *val = value;

        ListIterator<Module*> iter = modList.getListIterator();
        iter.reset();
        while (iter.hasNext())
        {
            Module *mod = iter.next();
            mod->propogate();
        }
    }

    return *this;
}

const Line& Line::operator=(const Line &line)
{
    if (*refCount > 1)
    {
        (*refCount)--;

        val = line.val;
        modList = line.modList;
        refCount = line.refCount;

        (*refCount)++;
    }
    else
    {
        delete refCount; delete val;

        val = line.val;
        modList = line.modList;
        refCount = line.refCount;

        (*refCount)++;
    }

    return *this;
}

ostream& operator<<(const std::ostream &osObj, const lcs::Line &line)
{
    if (line() == HIGH)
        return cout << "1";
    else if (line() == LOW)
        return cout << "0";
    else if (line() == UNKNOWN)
        return cout << "x";
    else
        return cout << "z";
}

const Line& operator~(Line &line)
{
    LineState state = line();
    if (state == LOW)
        line = HIGH;
    else if (state == HIGH)
        line = LOW;

    return line;
}

const LineState operator!(const Line &line)
{
    LineState state = line();
    if (state == LOW)
        return HIGH;
    else if (state == HIGH)
        return LOW;
    else
        return line();
}
