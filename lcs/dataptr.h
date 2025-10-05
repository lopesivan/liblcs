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

#ifndef __LCS_DATA_PTR_H__
#define __LCS_DATA_PTR_H__

#ifndef NULL
#define NULL 0
#endif

namespace lcs
{ // Start of namespace lcs

/// A simple template class which encapsulates a pointer to an object of any class.
/// This class can be used to create a (psuedo) double pointer to an object (of some
/// class) while avoiding the use of **. Under normal circumstances, a user of libLCS
/// will not need to make use of this class directly.
template <typename T>
class DataPtr
{
public:

    /// Default constructor.
    /// Initializes the pointer member to NULL.
    DataPtr();

    /// The pointer member.
    ///
    T *data;
};

template <typename T>
DataPtr<T>::DataPtr(void)
           : data(NULL)
{}

} // End of namespace lcs

#endif // __LCS_DATA_PTR_H__
