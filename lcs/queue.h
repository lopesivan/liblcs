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

#ifndef __LCS_QUEUE_H__
#define __LCS_QUEUE_H__

#include "listnode.h"

#ifndef NULL
#define NULL 0
#endif // NULL

namespace lcs
{ // Start of namespace lcs

/// This is a class which encapsulates a <i>first-in-first-out</i> queue data structure.
///
template <class T>
class Queue
{
public:

    /// Default constructor.
    ///
    Queue(void);

    /// Copy constructor. This is only a shallow coppier. The new object and the original
    /// object will denote the same queue.
    Queue(const Queue<T> &q);

    /// Destructor.
    ///
    ~Queue();

    /// Adds an element to the end of the queue.
    ///
    /// \param element The element to be added to the end of the queue.
    void enQueue(T element);

    /// Removes the first element from the queue.
    ///
    void deQueue(void);

    /// Returns the first element in the queue.
    ///
    T getFirstInQueue(void);

    /// Returns the length of the queue.
    ///
    int getSize(void) { return *size_; }

    /// Destroys the queue.
    ///
    void purge(void);

    /// The explicit assignment operator.
    /// The left lvalue and the rvalue will represent the same queue after the assignment.
    void operator=(Queue q);

private:
    ListNodePtr<T> *dataPtr, *lastNodeDataPtr;
    int *size_;
    int *refCount_;
};

template <class T>
Queue<T>::Queue(void)
{
    dataPtr = new ListNodePtr<T>();
    dataPtr->node = NULL;

    lastNodeDataPtr = new ListNodePtr<T>();
    lastNodeDataPtr->node = NULL;

    size_ = new int;
    *size_ = 0;

    refCount_ = new int;
    *refCount_ = 1;
}

template <class T>
Queue<T>::Queue(const Queue<T> &q)
        : dataPtr(q.dataPtr), lastNodeDataPtr(q.lastNodeDataPtr),
          size_(q.size_), refCount_(q.refCount_)
{
    (*refCount_)++;
}

template <class T>
Queue<T>::~Queue()
{
    if (*refCount_ > 1)
    {
        dataPtr = NULL; lastNodeDataPtr = NULL;
        size_ = NULL;

        (*refCount_)--;
        refCount_ = NULL;
    }
    else
    {
        purge();
        dataPtr = lastNodeDataPtr = NULL;

        delete size_;
        delete refCount_;
        delete dataPtr;
        delete lastNodeDataPtr;
    }
}

template <class T>
void Queue<T>::enQueue(T e)
{
    ListNode<T> *newNodePtr = new ListNode<T>;
    newNodePtr->data = e;
    newNodePtr->nextNodePtr = newNodePtr->previousNodePtr = NULL;

    if (*size_ == 0)
    {
        dataPtr->node = newNodePtr;
        lastNodeDataPtr->node = newNodePtr;
    }
    else
    {
        ListNode<T> *lastNodePtr = lastNodeDataPtr->node;

        lastNodePtr->nextNodePtr = newNodePtr;
        newNodePtr->previousNodePtr = lastNodePtr;
        lastNodeDataPtr->node = newNodePtr;
    }

    (*size_)++;
    return;
}

template <class T>
void Queue<T>::deQueue(void)
{
    if (*size_ == 0)
        return;

    if (*size_ == 1)
    {
        ListNode<T> *firstNodePtr = dataPtr->node;

        delete firstNodePtr;

        dataPtr->node = NULL;
        lastNodeDataPtr->node = NULL;

        *size_ = 0;
        return;
    }

    ListNode<T> *firstNodePtr = dataPtr->node;

    firstNodePtr = firstNodePtr->nextNodePtr;
    delete firstNodePtr->previousNodePtr;
    firstNodePtr->previousNodePtr = NULL;

    dataPtr->node = firstNodePtr;

    (*size_)--;
}

template <class T>
T Queue<T>::getFirstInQueue(void)
{
    if (*size_ == 0)
    {
        T dummy = T();
        return dummy;
    }
    else
    {
        return dataPtr->node->data;
    }
}

template <class T>
void Queue<T>::purge(void)
{
    int s = *size_;
    for (int i = 0; i < s; i++)
        deQueue();

    *size_ = 0;
    return;
}

template <class T>
void Queue<T>::operator=(Queue q)
{
    if (*refCount_ <= 1)
    {
        purge();

        delete size_;
        delete refCount_;
        delete dataPtr;
        delete lastNodeDataPtr;

        refCount_ = q.refCount_;
        dataPtr = q.dataPtr;
        lastNodeDataPtr = q.lastNodeDataPtr;
        size_ = q.size_;

        (*refCount_)++;
    }
    else
    {
        (*refCount_)--;

        refCount_ = q.refCount_;
        dataPtr = q.dataPtr;
        lastNodeDataPtr = q.lastNodeDataPtr;
        size_ = q.size_;

        (*refCount_)++;
    }

    return;
}

} // End of namespace lcs

#endif // __LCS_QUEUE_H__

