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
#include "sort.h"
#include <iostream>
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
    T getFirstInQueue(void) const;

    /// Returns the last element in the queue.
    ///
    T getLastInQueue(void) const;

    /// Returns the length of the queue.
    ///
    int getSize(void) const { return *size_; }

    /// Purges the last element in the queue.
    ///
    void popLast(void);

    /// Destroys the queue.
    ///
    void purge(void);

    /// The explicit assignment operator.
    /// The left lvalue and the rvalue will represent the same queue after the assignment.
    Queue<T>& operator=(const Queue<T> &q);

protected:
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
T Queue<T>::getFirstInQueue(void) const
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
T Queue<T>::getLastInQueue(void) const
{
    if (*size_ == 0)
    {
        T dummy = T();
        return dummy;
    }
    else
    {
        return lastNodeDataPtr->node->data;
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
void Queue<T>::popLast(void)
{
    if (*size_ == 0)
        return;
    else if (*size_ == 1)
        deQueue();
    else
    {
        lastNodeDataPtr->node = lastNodeDataPtr->node->previousNodePtr;
        delete lastNodeDataPtr->node->nextNodePtr;
        lastNodeDataPtr->node->nextNodePtr = NULL;

        (*size_)--;
    }
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T> &q)
{
    if (refCount_ == q.refCount_)
        return *this;

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

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
// PriotityQueue
////////////////////////////////////////////////////////////////////////////////////

/// This is a class which encapsulates a priority queue. An element can be queued up at
/// the right place in the queue using a key value. Since the supported key values are
/// integers, a higher value can have a higher priority or a lower priority. A user
/// of this class can select the desired way using the template argument \p Order.
/// This argument can either 1. take a value \p lcs::ASCENDING to indicate that lower
/// key values have higher priority, or 2. take a value \p lcs::DESCENDING to indicate
/// that higher key values have higher priority.
///
/// This class is inherited from the class \p lcs::Queue through protected inheritance.
/// However, it supports all the member functions as in the class \p lcs::Queue.
template <typename T, SortOrder Order>
class PriorityQueue : protected Queue<T>
{
public:

    /// Adds an element into the queue based on the priority indicated by the key argument.
    ///
    /// \param e The element to be added into the queue.
    /// \param key The key which indicates the priority.
    void enQueue(T e, int key);

    /// Removes elements from the queue which have a priority equal and less than the one
    /// indicated by the key argument.
    ///
    /// \param key Elements in the queue having lesser or equal priority than that indicated
    /// by this argument will be removed from the queue.
    void removeLower(int key);

    using Queue<T>::deQueue;
    using Queue<T>::getSize;
    using Queue<T>::popLast;
    using Queue<T>::purge;
    using Queue<T>::getFirstInQueue;
    using Queue<T>::getLastInQueue;

protected:

    using Queue<T>::size_;
    using Queue<T>::dataPtr;
    using Queue<T>::lastNodeDataPtr;
};

template <typename T, SortOrder Order>
void PriorityQueue<T, Order>::enQueue(T e, int key)
{
    ListNode<T> *newNode = new ListNode<T>();
    newNode->data = e; newNode->key = key;

    if (*size_ == 0)
    {
        Queue<T>::dataPtr->node = newNode;
        Queue<T>::lastNodeDataPtr->node = newNode;

        (*size_)++;
        return;
    }

    if (Order == ASCENDING)
    {
        ListNode<T> *node = Queue<T>::dataPtr->node;
        while (node != NULL)
        {
            if (key < node->key)
            {
                newNode->previousNodePtr = node->previousNodePtr;
                newNode->nextNodePtr = node;

                node->previousNodePtr = newNode;

                if (newNode->previousNodePtr != NULL)
                    newNode->previousNodePtr->nextNodePtr = newNode;
                else
                    Queue<T>::dataPtr->node = newNode;

                (*size_)++;
                return;
            }

            node = node->nextNodePtr;
        }

        node = Queue<T>::lastNodeDataPtr->node;
        node->nextNodePtr = newNode;
        newNode->previousNodePtr = node;

        Queue<T>::lastNodeDataPtr->node = newNode;
        (*size_)++;

        return;
    }

    if (Order == DESCENDING)
    {
        ListNode<T> *node = Queue<T>::dataPtr->node;
        while (node != NULL)
        {
            if (key > node->key)
            {
                newNode->previousNodePtr = node->previousNodePtr;
                newNode->nextNodePtr = node;

                node->previousNodePtr = newNode;

                if (newNode->previousNodePtr != NULL)
                    newNode->previousNodePtr->nextNodePtr = newNode;
                else
                    Queue<T>::dataPtr->node = newNode;

                (*size_)++;
                return;
            }

            node = node->nextNodePtr;
        }

        node = Queue<T>::lastNodeDataPtr->node;
        node->nextNodePtr = newNode;
        newNode->previousNodePtr = node;

        Queue<T>::lastNodeDataPtr->node = newNode;
        (*size_)++;

        return;
    }
}

template <typename T, SortOrder Order>
void PriorityQueue<T, Order>::removeLower(int key)
{
    int count = 0;
    ListNode<T> *node = Queue<T>::dataPtr->node;

    int *size = Queue<T>::size_;

    while (node != NULL)
    {
        if (Order == ASCENDING)
        {
            if (node->key >= key)
            {
                while (*size != count)
                    popLast();

                break;
            }
        }
        else if (Order == DESCENDING)
        {
            if (node->key <= key)
            {
                while (*size != count)
                    popLast();

                break;
            }
        }

        count++;
        node = node->nextNodePtr;
    }
}

} // End of namespace lcs

#endif // __LCS_QUEUE_H__
