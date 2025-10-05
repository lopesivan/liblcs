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

#ifndef __LCS_LIST_H__
#define __LCS_LIST_H__

#include "listnode.h"
#include "iterator.h"

#ifndef NULL
#define NULL 0
#endif // NULL

namespace lcs
{ // Start of namespace lcs

/// An iterator class which can iterate over the elements of a lcs::List object.
///
template <class T>
class ListIterator : public Iterator<T>
{
public:

    /// Default constructor. An empty list is assumed unless a root node of the list is
    /// set using the ListIterator::setRootNode is used.
    ListIterator(void);

    /// The iterator is initialised with the first node of the list as the argument
    /// \p node.
    ListIterator(ListNode<T> *node);

    /// Copy constructor.
    ///
    ListIterator(const ListIterator<T> &li);

    /// Destructor.
    ///
    virtual ~ListIterator() {}

    /// Returns true if the list has further elements at from the current position of
    /// the iterator. Returns false if the iterator is at the end of the list.
    virtual bool hasNext(void);

    /// Returns the next element of the list if there exists one. Else, returns a dummy
    /// object of type \p T.
    virtual T next(void);

    /// Resets the iterator to the first element of the list.
    virtual void reset(void);

    /// Sets the root node of the list to iterate over. Use this function if you
    /// thoroughly understand what you are doing, or if the iterator was initialised
    /// through the default constructor.
    void setRootNode(ListNode<T> *node);

    ///
    ///
    void operator=(const ListIterator<T> &li);

private:
    ListNode<T> *initNode, *currentNode;
};

template <class T>
ListIterator<T>::ListIterator(void)
            : Iterator<T>(), initNode(NULL), currentNode(NULL)
{ }

template <class T>
ListIterator<T>::ListIterator(ListNode<T> *node)
            : Iterator<T>(), initNode(node), currentNode(node)
{ }

template <class T>
ListIterator<T>::ListIterator(const ListIterator<T> &li)
            : Iterator<T>(), initNode(li.initNode), currentNode(li.currentNode)
{ }

template <class T>
void ListIterator<T>::setRootNode(ListNode<T> *node)
{
    initNode = node;
    currentNode = node;
}

template <class T>
bool ListIterator<T>::hasNext(void)
{
    if (currentNode != NULL)
        return true;
    else
        return false;
}

template <class T>
T ListIterator<T>::next(void)
{
    if (currentNode != NULL)
    {
        ListNode<T> *nodePtr = currentNode;
        currentNode = currentNode->nextNodePtr;

        return nodePtr->data;
    }
    else
    {
        T dummy = T();
        return dummy;
    }
}

template <class T>
void ListIterator<T>::reset(void)
{
    currentNode = initNode;
}

template <class T>
void ListIterator<T>::operator=(const ListIterator<T> &li)
{
    initNode = li.initNode;
    currentNode = li.currentNode;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

/// A class encapsulating a doubly linked list.
///
template <class T>
class List
{
public:
    /// Default Constructor.
    ///
    List(void);

    /// Copy constructor.
    /// It is only a reference copier.
    List(const List<T> &l);

    /// Destructor.
    /// It does consider the number of references to the object before destruction.
    ~List();

    /// Appends a node to the end of the list.
    /// It is equivalent to push.
    void append(T node);

    /// Pushes a node to the end of the list.
    /// It is equivalent to append. Infact <I>push</I> calls the function <I>append</I>.
    void push(T node);

    /// Pops a node from the end of the list.
    /// Does nothing in case of an empty list. \n
    ///
    void pop(void);

    /// Returns an iterator for the list.
    ///
    ListIterator<T> getListIterator(void);

    /// Removes the first list element equal to the function argument \p e.
    ///
    void removeFirstMatch(T e);

    /// Destroys the list.
    ///
    void destroy(void);

    /// Returns the data in the last node of the list.
    ///
    T last() const;

    /// Returns the data in the first node of the list.
    ///
    T first()const;

    /// Checks if an element is present in the list. Returns \p true if present, \p false
    /// if not. The type T should support the operator '==' for this function to compile
    /// successfully.
    bool isPresent(const T &t);

    /// Returns the number of data nodes in the list.
    ///
    int getSize(void);

    /// Assignment operator.
    /// It is only a reference copier. i.e., after an assignment operation, the l-value and
    /// the r-value share the same data.
    void operator=(const List<T> &l);

private:
    int *size;
    int *refCount;
    ListNodePtr<T> *dataPtr;
    ListNodePtr<T> *lastNodeDataPtr;
};

// List implementation...

// Constructor...
template <class T>
List<T>::List(void)
{
    dataPtr = new ListNodePtr<T>();
    lastNodeDataPtr = new ListNodePtr<T>();

    size = new int; refCount = new int;
    *size = 0; *refCount = 1;
}

template <class T>
List<T>::List(const List<T> &l)
        : size(l.size), refCount(l.refCount), dataPtr(l.dataPtr),
          lastNodeDataPtr(l.lastNodeDataPtr)
{
    (*refCount)++;
}

template <class T>
List<T>::~List()
{
    if (*refCount <= 1)
    {
        destroy();
        delete size; delete refCount;
        delete dataPtr;
    }
    else
        (*refCount)--;
}

template <class T>
void List<T>::append(T node)
{
    ListNode<T> *newNodePtr = new ListNode<T>();

    if (*size == 0)
    {
        dataPtr->node = newNodePtr;
    }
    else
    {
        newNodePtr->previousNodePtr = lastNodeDataPtr->node;
        lastNodeDataPtr->node->nextNodePtr = newNodePtr;
    }

    lastNodeDataPtr->node = newNodePtr;
    lastNodeDataPtr->node->data = node;
    (*size)++;

    return;
}

template <class T>
ListIterator<T> List<T>::getListIterator(void)
{
    return ListIterator<T>(dataPtr->node);
}

template <class T>
void List<T>::pop(void)
{
    if (*size == 0)
        return;
    else if (*size == 1)
    {
        delete dataPtr->node;
        dataPtr->node = NULL;
        lastNodeDataPtr->node = NULL;
        (*size)--;

        return;
    }

    lastNodeDataPtr->node = lastNodeDataPtr->node->previousNodePtr;
    delete lastNodeDataPtr->node->nextNodePtr;
    lastNodeDataPtr->node->nextNodePtr = NULL;
    (*size)--;

    return;
}

template <class T>
void List<T>::push(T node)
{
    append(node);
    return;
}

template <class T>
void List<T>::destroy(void)
{
    int s = *size;

    for (int i = 1; i <= s; i++)
        pop();

    return;
}

template <class T>
T List<T>::last() const
{
    if (*size == 0)
    {
        T dummy = T();
        return dummy;
    }
    else
    {
        T lastNodeData = T(lastNodeDataPtr->node->data);
        return lastNodeData;
    }
}

template <class T>
T List<T>::first() const
{
    if (*size == 0)
    {
        T dummy = T();
        return dummy;
    }
    else
    {
        T firstNodeData = T(dataPtr->node->data);
        return firstNodeData;
    }
}

template <class T>
bool List<T>::isPresent(const T &t)
{
    ListNode<T> *node = dataPtr->node;
    while (node != NULL)
    {
        if (node->data == t)
            return true;

        node = node->nextNodePtr;
    }

    return false;
}

template <class T>
int List<T>::getSize(void)
{
    return *size;
}

template <class T>
void List<T>::removeFirstMatch(T e)
{
    ListNode<T> *node = dataPtr->node;
    while (node != NULL)
    {
        if (node->data == e)
        {
            if (node->previousNodePtr == NULL)
            {
                dataPtr->node = node->nextNodePtr;
                delete node;
                (*size)--;

                node = dataPtr->node;
                if (node == NULL)
                    lastNodeDataPtr->node = NULL;
                else
                    node->previousNodePtr = NULL;

            }
            else
            {
                node->previousNodePtr->nextNodePtr = node->nextNodePtr;

                ListNode<T> *tempNode = node->nextNodePtr;

                if (tempNode == NULL)
                {
                    lastNodeDataPtr->node = node->previousNodePtr;
                }
                else
                    tempNode->previousNodePtr = node->previousNodePtr;

                delete node;
                (*size)--;
                node = tempNode;
            }

            return;
        }
        else
        {
            node = node->nextNodePtr;
        }
    }
}

template <class T>
void List<T>::operator=(const List<T> &l)
{
    if (refCount == l.refCount)
        return;

    if (*refCount <= 1)
    {
        destroy();
        delete size; delete refCount;
        delete dataPtr;

        dataPtr = l.dataPtr;
        lastNodeDataPtr = l.lastNodeDataPtr;
        size = l.size;
        refCount = l.refCount;
        (*refCount)++;
    }
    else
    {
        (*refCount)--;

        dataPtr = l.dataPtr;
        lastNodeDataPtr = l.lastNodeDataPtr;
        size = l.size;
        refCount = l.refCount;
        (*refCount)++;
    }

    return;
}

} // End of namespace lcs

#endif  // __LCS_LIST_H__
