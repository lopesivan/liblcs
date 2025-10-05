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

#ifndef __HIDDEN_MODULE_MANAGER_H__
#define __HIDDEN_MODULE_MANAGER_H__

#include "list.h"

namespace lcs
{ // Start of namespace lcs

class Module;

/// A class which provides static member functions to help manage modules which are
/// hidden from libLCS users.
class HiddenModuleManager
{
public:

    /// Registers a hidden module. It does not do anything more than storing a pointer to
    /// the module object in a list.
    static void registerModule(Module *mod) { modList.append(mod); }

    /// Destroys all the hidden modules registered.
    ///
    static void destroyModules(void);

private:

    static List<Module*> modList;
};

} // End of namespace lcs

#endif // __HIDDEN_MODULE_MANAGER_H__
