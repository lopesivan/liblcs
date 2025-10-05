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

#ifndef __LCS_VCD_MAN_H__
#define __LCS_VCD_MAN_H__

#include <fstream>
#include <string>
#include <iostream>
#include "timeunit.h"
#include "systime.h"
#include "linestate.h"

namespace lcs
{ // Start of namespace lcs

/// An enumerated set of options which indicate if changes in the values of a variable should be
/// dumped into a VCD file or not.
enum DumpOption
{
    /// Indicates that the variable values should not be dumped into a VCD file.
    ///
    DUMP_OFF = 5020,

    /// Indicates that the variable values should be dumped into a VCD file.
    ///
    DUMP_ON  = 5021
};

class Simulation;

/// This class manages the generation of a VCD file for the simulation. However, the user
/// should not/cannot use this class and/or its methods direcly. One should use the class
/// \p lcs::ChangeMonitor and the static members of the class \p lcs::Simulation.
///
/// <b>ADVICE</b>: DO NOT use this class and its members unless you are sure of what you
/// are doing!
class VCDManager
{

/// The class \p lcs::Simulation is a friend.
///
friend class Simulation;

public:

    /// This function should be used to notify the manager about a variable which has to be dumped.
    /// If a variable is not registered with the manager, then the VCD file generated will contain
    /// errors.
    ///
    /// \param varName The name of the variable. It should not have any spaces. Spaces will lead to
    /// errors in the VCD file!
    ///
    /// \param var The variable which has to be registered.
    template <typename T>
    static void registerChangeReporter(const std::string &varName, const T &var);

    /// This function should be used to notify the value change of a variable which has been
    /// registered with the manager. When this function is called, the value of the variable is
    /// dumped into the VCD file. Value changes of only those variables which have been registered
    /// previously should be notified. Notifying without previous registertion (using the function
    /// \p lcs::VCDManager::registerChangeReporter) will lead to errors in the VCD file.
    ///
    /// \param varName The name of the variable which has changed. This name should exactly be
    /// the same as that which was used while registering.
    ///
    /// \param var The variable whose value has changed.
    template <typename T>
    static void noteChange(const std::string &varName, const T &var);

private:

    /// Sets the name of the VCD file. It is private, and hence cannot be used directly.
    /// A user will have to use \p lcs::Simulation::setDumpFileName instead.
    static void setDumpFileName(const std::string &filename) { fname_ = filename; }

    /// Sets the time unit for the system time unit. It is private, and hence cannot be used
    /// directly. A user will have to use \p lcs::Simulation::setTimeUnit instead.
    static void setTimeUnit(const TimeUnit &timeunit) { unit_ = timeunit;}

    /// Initialises the manager.
    ///
    static void init(void);

    /// Terminates the manager.
    ///
    static void terminate(void);

    static std::ofstream out_;
    static std::string fname_;
    static int varcount_;
    static TimeUnit unit_;
    static List<std::string> varNameList_;
    static List<std::string> initValList_;
    static List<int> varSizeList_;
    static int currtime_;
};

template <typename T>
void VCDManager::registerChangeReporter(const std::string &varName, const T &var)
{
    int vwidth = varName.length();
    for (int i = 0; i < vwidth; i++)
    {
        if (varName[i] == ' ')
        {
            std::cerr << "The variable name '" << varName << "'has a space in it.\n"
                      << "It will not be accepted for dumping into the VCD file!\n\n";

            return;
        }
    }

    varcount_++;

    varNameList_.append(varName);
    varSizeList_.append(var.width());
    initValList_.append(var.toStr());
}

template <typename T>
void VCDManager::noteChange(const std::string &varName, const T &var)
{
    if (varcount_ < 0) return;

    if (currtime_ < 0)
    {
        out_ << "#0" << std::endl;

        ListIterator<std::string> varIter = varNameList_.getListIterator();
        ListIterator<std::string> initIter = initValList_.getListIterator();
        ListIterator<int> sizeIter = varSizeList_.getListIterator();

        varIter.reset(); sizeIter.reset(); initIter.reset();
        while (varIter.hasNext())
        {
            int size = sizeIter.next();
            std::string name = varIter.next();
            std::string initVal = initIter.next();

            if (size == 1)
                out_ << initVal << name << std::endl;
            else
                out_ << 'b' << initVal << " " << name << std::endl;
        }
    }

    unsigned int time = SystemTimer::getTime();
    if (currtime_ < static_cast<int>(time))
    {
        out_ << "#" << time << std::endl;
        currtime_ = static_cast<int>(time);
    }

    int size = var.width();
    if (size != 1)
        out_ << 'b';

    for (int i = size-1; i >= 0 ; i--)
    {
        if (lcs::HIGH == var[i]) out_ << "1";
        if (lcs::LOW == var[i]) out_ << "0";
        if (lcs::UNKNOWN == var[i]) out_ << "x";
        if (lcs::HIGH_IMPEDENCE == var[i]) out_ << "z";
    }

    if (size == 1)
        out_ << varName << std::endl;
    else
        out_ << " " << varName << std::endl;

}

} // End of namespace lcs

#endif // __LCS_VCD_MAN_H__
