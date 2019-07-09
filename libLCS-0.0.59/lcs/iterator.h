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

#ifndef __LCS_ITERATOR_H__
#define __LCS_ITERATOR_H__

namespace lcs
{ // Start of namespace lcs

/// An abstract base class which can be subclassed to generate iterators for container
/// classes. Under normal circumstances, a user of libLCS will not require to use this
/// class directly.
template <class T>
class Iterator
{
public:

    /// The default constructor.
    ///
    Iterator() {}

    /// Destructor.
    ///
    virtual ~Iterator() {}

    /// Indicates if there are any more elements in a container at the current state of
    /// iteration. Returns <b>\p true </b> if yes, <b>\p false </b> if not.
    virtual bool hasNext() = 0;

    /// Returns the next element in the container if it exists. Else, the behaviour is
    /// undefined.
    virtual T next() = 0;

    /// Resets the iterator to the first element of the container class.
    ///
    virtual void reset() = 0;
};

} // End of namespace lcs

#endif // __LCS_ITERATOR_H__
