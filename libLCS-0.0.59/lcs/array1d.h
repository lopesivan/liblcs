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

#ifndef __LCS_ARRAY_1D_H__
#define __LCS_ARRAY_1D_H__

#include <lcs/dataptr.h>
#include <lcs/outofrange_exception.h>

#ifndef NULL
#define NULL 0
#endif

namespace lcs
{ // Start of namespace lcs

/// A one dimensional array class.
///
template <typename T, unsigned int len>
class Array1D
{
public:

    /// Default constructor.
    ///
    Array1D(void);

    ///
    ///
    Array1D(T initValue);

    /// Copy constructor.
    ///
    Array1D(const Array1D<T, len> &a);

    /// Destructor.
    ///
    virtual ~Array1D();

    /// Returns the length of the array.
    ///
    int length(void) const { return len; }

    /// The assignment operator.
    ///
    Array1D<T, len>& operator=(const Array1D<T, len> &a);

    /// Retrieves the array element at the specified index.
    ///
    T& operator[](unsigned int index) throw (OutOfRangeException<unsigned int>);

    /// Retrieves the array element at the specified index.
    ///
    const T& operator[](unsigned int index) const throw (OutOfRangeException<unsigned int>);

protected:
    int *refCount_;

    ///
    ///
    DataPtr<T> *dataPtr;
};

template <typename T, unsigned int len>
Array1D<T, len>::Array1D(void)
{
    refCount_ = new int;
    *refCount_ = 1;

    dataPtr = new DataPtr<T>();
    dataPtr->data = new T [len];
}

template <typename T, unsigned int len>
Array1D<T, len>::Array1D(T initValue)
{
    refCount_ = new int;
    *refCount_ = 1;

    dataPtr = new DataPtr<T>();
    dataPtr->data = new T [len];

    T *data = dataPtr->data;
    for (unsigned int i = 0; i < len; i++)
    {
        data[i] = initValue;
    }
}

template <typename T, unsigned int len>
Array1D<T, len>::Array1D(const Array1D<T, len> &a)
          : refCount_(a.refCount_), dataPtr(a.dataPtr)
{
    (*refCount_)++;
}

template <typename T, unsigned int len>
Array1D<T, len>::~Array1D()
{
    if (*refCount_ == 1)
    {
        delete refCount_;
        delete [] dataPtr->data;
        delete dataPtr;
    }
    else if (*refCount_ > 1)
    {
        (*refCount_)--;
        refCount_ = NULL;
        dataPtr = NULL;
    }
}

template <typename T, unsigned int len>
Array1D<T, len>& Array1D<T, len>::operator=(const Array1D<T, len> &a)
{
    if (refCount_ == a.refCount_)
        return *this;

    if (*refCount_ <= 1)
    {
        delete [] dataPtr->data;
        delete refCount_;
        delete dataPtr;
    }
    else
    {
        (*refCount_)--;

    }

    refCount_ = a.refCount_;
    dataPtr = a.dataPtr;
    (*refCount_)++;

    return *this;
}

template <typename T, unsigned int len>
T& Array1D<T, len>::operator[](unsigned int index) throw (OutOfRangeException<unsigned int>)
{
    if (index >= len)
        throw OutOfRangeException<unsigned int>(0, len-1, index);

    T *data = dataPtr->data;
    return data[index];
}

template <typename T, unsigned int len>
const T& Array1D<T, len>::operator[](unsigned int index) const
                                                throw (OutOfRangeException<unsigned int>)
{
    if (index >= len)
        throw OutOfRangeException<unsigned int>(0, len-1, index);

    T *data = dataPtr->data;
    return data[index];
}

} // End of namespace lcs.

#endif // __LCS_ARRAY_1D_H__
