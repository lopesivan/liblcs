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

#ifndef __DEL_BUS_PAIR_H__
#define __DEL_BUS_PAIR_H__

#include "inbus.h"

namespace lcs
{ // Start of namespace lcs

/// A class whose objects facilitate assignment delays when a bus is being assigned to
/// a bus or a bus line. This class will never need to be used by a user of libLCS directly.
///
/// See \p lcs::Bus::operator= for more information on incorporating assignment delays.
template <int w>
class DelayBusPair
{
public:

    /// The only usefull constructor.
    ///
    /// \param delay The delay associated with the pair
    /// \param bus The bus associated with the pair
    DelayBusPair(unsigned int delay, const InputBus<w> &bus)
                : _d(delay), _bus(bus) {}

    /// The copy constructor
    ///
    DelayBusPair(const DelayBusPair<w> &obj)
                : _d(obj._d), _bus(obj._bus) {}

    /// Destructor.
    ///
    ~DelayBusPair() {}

    /// Returns the delay associated with the delay-bus pair.
    ///
    unsigned int getDelay(void) const { return _d; }

    /// Returns the bus associated with the delay-bus pair.
    ///
    const InputBus<w> getBus() const { return _bus; }

private:
    unsigned int _d;
    InputBus<w> _bus;
};

} // End of namespace lcs

template <int w>
const lcs::DelayBusPair<w> operator,(unsigned int delay, const lcs::InputBus<w> &bus)
{
    return lcs::DelayBusPair<w>(delay, bus);
}

#endif // __DEL_BUS_PAIR_H__
