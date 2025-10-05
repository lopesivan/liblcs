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

#ifndef __LCS_LINE_H__
#define __LCS_LINE_H__

#include "linestate.h"
#include "module.h"
#include "list.h"
#include <iostream>

namespace lcs
{ // Start of namespace lcs

template <int w>
class Bus;

/// A class to encapsulate a data line. A \p Line object is used to denote one bit of a
/// lcs::Bus object. Various operators are supported to facilitate bit operations.
class Line
{

/// InputBus classes of any size are friends of this class.
///
template <int w>
friend class InputBus;
public:

    /// Default constructor.
    ///
    Line(void) : modList(List<Module*>())
    {
        val = new LineState; *val = UNKNOWN;
        refCount = new int;
        *refCount = 1;
    }

    /// Copy constructor.
    ///
    Line(const Line& line) : val(line.val), refCount(line.refCount), modList(line.modList)
    {
        (*refCount)++;
    }

    /// Destructor.
    ///
    virtual ~Line()
    {
        if (*refCount <= 1)
        {
            delete val; delete refCount;
        }
        else
            (*refCount)--;
    }

    /// Returns a single line lcs::Bus object encapsulating the line.
    ///
    Bus<1> makeBus(void);

    /// Overloaded assignment operator which sets the line state using a LineState variable.
    ///
    /// \param value The value to which the line state should be changed.
    ///
    const Line& operator=(const LineState& value);

    /// Overloaded assignment operator.
    /// The LHS Line object and the RHS Line object will both share the same line data
    /// after assignment.
    const Line& operator=(const Line &line);

    /// Returns the line state.
    ///
    const LineState operator()(void) const { return *val; }

    /// Overloaded bit 'or' operator.
    /// Performs a 'or' operation of the line states of two Line objects.
    const LineState operator|(const Line& line) const { return *val|*(line.val); }

    /// Overloaded bit 'and' operator.
    /// Performs a 'and' operation of the line states of two Line objects.
    const LineState operator&(const Line& line) const { return *val&*(line.val); }

    /// Overloaded bit 'xor' operator.
    /// Performs a 'xor' operation of the line states of two Line objects.
    const LineState operator^(const Line& line) const { return *val^*(line.val); }

    /// Overloaded logical equality operator.
    /// Compares the line state with a LineState variable.
    ///
    /// \param state The LineState variable with which the comparison should be made.
    bool operator==(const LineState& state) const
    {
        if (*val == state) return true; else return false;
    }

    /// Overloaded logical inequality operator.
    /// Compares the line state with a LineState variable.
    ///
    /// \param state The LineState variable with which the comparison should be made.
    ///
    bool operator!=(const LineState& state) const
    {
        if (*val != state) return true; else return false;
    }

private:

    /// A lcs::Module (or its derivative) should register to be driven by the line using this
    /// function. If registered, a change in the line state will call the function
    /// lcs::Module::propogate.
    ///
    /// \param mod Pointer to the lcs::Module object which has to be driven by the line.
    ///
    void drive(Module *mod);

    /// A lcs::Module (or its derivative) can de-register itself from being driven by the
    /// line using this function. See Line::drive for more information.
    ///
    /// \param mod Pointer to the lcs::Module object which should be de-registered from
    /// being driven by the line.
    ///
    void unDrive(Module *mod);

    LineState *val;
    int *refCount;
    List< Module* > modList;
};

} // End of namespace lcs

/// \function
/// Overloaded bit-shift operator to display a lcs::Line object's state on std::stdout.
///
std::ostream& operator<<(const std::ostream &osObj, const lcs::Line &line);

/// \function
/// Inplace negation operator.
/// The line state of the lcs::Line object is toggled if known. The state is left
/// unchanged if it is lcs::UNKNOWN or lcs::HIGH_IMPEDENCE.
const lcs::Line& operator~(lcs::Line &line);

/// \function
/// Returns the negation of the line state.
/// Returns the line state if the state is lcs::UNKNOWN or lcs::HIGH_IMPEDENCE.
const lcs::LineState operator!(const lcs::Line &line);

#endif // __LCS_LINE_H__
