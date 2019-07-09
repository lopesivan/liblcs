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

#ifndef __LCS_LISTNODE_H__
#define __LCS_LISTNODE_H__

#ifndef NULL
#define NULL 0
#endif // NULL

namespace lcs
{ // Start of namespace lcs

/// A class to hold each node of a lcs::List object.
/// This class is not intended for use by a user willing to use the List class. ListNode is
/// hidden from the user of class lcs::List. Under normal circumstances, a user of libLCS
/// will not need to make use of this class directly.
template <class T>
class ListNode
{
public:
    /// Default Constructor.
    /// A copy constructor is not provided. Hence data is partially or fully prone to
    /// multiple references as the compiler synthesized copy constructor would only be a
    /// reference copier.
    ListNode(void);

    /// Destructor.
    /// Does not keep track of the data items having multiple references.
    /// Hence use the objects created with care.
    ~ListNode();

    /// Pointer to the next ListNode in the list.
    ///
    ListNode *nextNodePtr;

    /// Pointer to the previous ListNode in the list.
    ///
    ListNode *previousNodePtr;

    /// The member which actually holds the data.
    ///
    T data;

    /// A key to the node.
    ///
    int key;
};

template <class T>
ListNode<T>::ListNode(void)
{
    nextNodePtr = previousNodePtr = NULL;
    data = T();
}

template <class T>
ListNode<T>::~ListNode() { ; }

/// A simple class which encapsulates a pointer to a ListNode object.
/// This class can be used to create a (psuedo) double pointer to a ListNode object while
/// avoiding the use of **. Under normal circumstances, a user of libLCS will not need
/// to make use of this class directly.
template <class T>
class ListNodePtr
{
public:

    /// Default constructor.
    /// Initializes the pointer member to NULL.
    ListNodePtr(void) : node(NULL) {}

    /// The pointer to a ListNode object.
    ///
    ListNode<T> *node;
};

} // End of namespace lcs

#endif // __LCS_LISTNODE_H__
