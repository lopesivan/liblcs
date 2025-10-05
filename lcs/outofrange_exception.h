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

#ifndef __LCS_OUT_OF_RANGE_EXCEPTION_H__
#define __LCS_OUT_OF_RANGE_EXCEPTION_H__

#include "exception.h"
#include <sstream>

namespace lcs
{ // Start of namespace lcs.

/// An exception class whose objects are thrown when data beyond the allowed range is
/// accessed from indexed data access classes. An example of a call which can lead to
/// this exception being thrown is the function \p lcs::InputBus::operator[].
///
/// \param T should be a type which supports relational operators like <, >, etc.
template <typename T>
class OutOfRangeException : public Exception
{
public:

    /// Default constructor.
    /// Exception objects initialised through this constructor should set the
    /// exception specific data using the \p OutOfRangeException::setData function. If
    /// the data is not set, then wrong information is reported by the
    /// \p OutOfRangeException::getMessage function.
    OutOfRangeException();

    /// Data initialising constructor.
    /// The exception should occur if the \p requestedIndex is out the allowed range
    /// (\p allowedLowerLimit, \p allowedUpperLimit).
    ///
    /// \param allowedLowerLimit The allowed lower limit for the data access index.
    /// \param allowedUpperLimit The allowed upper limit for the data access index.
    /// \param requestedIndex The requested data index which triggered the exception.
    OutOfRangeException(T allowedLowerLimit, T allowedUpperLimit, T requestedIndex);

    /// Virtual destructor.
    ///
    virtual ~OutOfRangeException() {}

    /// Returns the exception specific message string generated using the exception data.
    ///
    virtual std::string getMessage(void);

    /// Sets the exception specific data.
    /// The exception should occur if the \p requestedIndex is out the allowed range
    /// (\p allowedLowerLimit, \p allowedUpperLimit).
    ///
    /// \param allowedLowerLimit The allowed lower limit for the data access index.
    /// \param allowedUpperLimit The allowed upper limit for the data access index.
    /// \param requestedIndex The requested data index which triggered the exception.
    void setData(T allowedLowerLimit, T allowedUpperLimit, T requestedIndex);

    /// Returns the allowed lower limit.
    ///
    T getAllowedLowerLimit() { return lowerLimit; }

    /// Returns the allowed upper limit.
    ///
    T getAllowedUpperLimit() { return upperLimit; }

private:
    T lowerLimit, upperLimit, index;
};

template <typename T>
OutOfRangeException<T>::OutOfRangeException(void)
                      : Exception(), lowerLimit(0), upperLimit(0), index(-1)
{}

template <typename T>
OutOfRangeException<T>::OutOfRangeException(T allowedLowerLimit, T allowedUpperLimit,
                                            T requestedIndex)
                      : Exception(), lowerLimit(allowedLowerLimit),
                                     upperLimit(allowedUpperLimit),
                                     index(requestedIndex)
{
    if (lowerLimit > upperLimit)
    {
        lowerLimit = allowedUpperLimit;
        upperLimit = allowedLowerLimit;
    }
}

template <typename T>
std::string OutOfRangeException<T>::getMessage()
{
    std::stringstream ss;

    ss << "Out of range exception has occured... \n"
       << "Allowed Lower Limit: " << lowerLimit << "\n"
       << "Allowed Upper Limit: " << upperLimit << "\n"
       << "Requested Index    : " << index << "\n";

    return ss.str();
}

template <typename T>
void OutOfRangeException<T>::setData(T allowedLowerLimit, T allowedUpperLimit,
                                     T requestedIndex)
{
    lowerLimit = allowedLowerLimit;
    upperLimit = allowedUpperLimit;
    index = requestedIndex;

    if (lowerLimit > upperLimit)
    {
        lowerLimit = allowedUpperLimit;
        upperLimit = allowedLowerLimit;
    }
}

} // End of namespace lcs.

#endif // __LCS_OUT_OF_RANGE_EXCEPTION_H__
