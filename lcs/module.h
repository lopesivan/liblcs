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

    /// Propogates the data line states of the inputs bus(ses) of the module to the output
    /// bus(ses) of this module. A subclass of the class Module should override this
    /// function to perform the real logic operations on input data lines.
    virtual void propogate(void) {}

};

} // End of namespace lcs.

#endif // __LCS_MODULE_H__
