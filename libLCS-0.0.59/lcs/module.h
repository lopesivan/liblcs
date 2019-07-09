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

#ifndef __LCS_MODULE_H__
#define __LCS_MODULE_H__

namespace lcs
{ // Start of namespace lcs.

/// Every circuit element (like an AND gate) which needs to be driven by lcs::InputBus
/// objects has to be derived from this class Module. User of libLCS desirous of defining
/// his/her own element should subclass this class and redefine the virtual function
/// Module::propogate.
class Module
{
public:

    /// Default constructor.
    ///
    Module(void) {}

    /// Destructor.
    ///
    virtual ~Module() {}

    /// A subclass of the class Module should override this function so that it can be
    /// notified of a state change on its input busses. If a lcs::LINE_STATE_CHANGE event
    /// occurs on an input bus, the bus will call this function on the corresponding module
    /// object as a notification of the occurance of the event.
    virtual void onStateChange(int portId) {}

    /// A subclass of the class Module should override this function so that it can be
    /// notified of a positive edge on its input busses. If a lcs::LINE_POS_EDGE event
    /// occurs on an input bus, the bus will call this function on the corresponding module
    /// object as a notification of the occurance of the event.
    virtual void onPosEdge(int portId) {}

    /// A subclass of the class Module should override this function so that it can be
    /// notified of a negetive edge on its input busses. If a lcs::LINE_NEG_EDGE event
    /// occurs on an input bus, the bus will call this function on the corresponding module
    /// object as a notification of the occurance of the event.
    virtual void onNegEdge(int portId) {}

};

} // End of namespace lcs.

#endif // __LCS_MODULE_H__
