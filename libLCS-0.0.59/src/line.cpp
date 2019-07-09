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

#include <lcs/bus.h>
#include <lcs/systime.h>

using namespace lcs;
using namespace std;

Line::Line(void)
{
    val = new LineState; *val = UNKNOWN;
    refCount = new int;
    *refCount = 1;

    delay_ = new unsigned int;
    *delay_ = 0;

    refList = new List< Line* >();
    refList->append(this);
}

Line::Line(const Line& line)
    : val(line.val),
      refCount(line.refCount), delay_(line.delay_), refList(line.refList),
      changeModList(line.changeModList),
      posEdgeList(line.posEdgeList), negEdgeList(line.negEdgeList),
      changeModIdList(line.changeModIdList), posEdgeIdList(line.posEdgeIdList),
      negEdgeIdList(line.negEdgeIdList),
      nextChangeTimeQueue(line.nextChangeTimeQueue),
      zeroDelayChangeTimeQueue(line.zeroDelayChangeTimeQueue),
      valueQueue(line.valueQueue), zeroDelayValueQueue(line.zeroDelayValueQueue)
{
    (*refCount)++;
    refList->append(this);
}

Line::~Line()
{
    if (*refCount <= 1)
    {
        delete val; delete refCount; delete delay_; delete refList;
    }
    else
    {
        (*refCount)--;
        refList->removeFirstMatch(this);
    }
}

void Line::notify(const LineEvent &event, Module *mod, const int &portId)
{
    if (mod != NULL)
    {
        if (event == LINE_STATE_CHANGE)
        {
            if (!changeModList.isPresent(mod))
            {
                changeModList.append(mod);
                changeModIdList.append(portId);
            }
        }
        else if (event == LINE_POS_EDGE)
        {
            posEdgeList.append(mod);
            posEdgeIdList.append(portId);
        }
        else if (event == LINE_NEG_EDGE)
        {
            negEdgeList.append(mod);
            negEdgeIdList.append(portId);
        }
    }
}

void Line::stopNotification(const LineEvent& event, Module *mod, const int &portId)
{
    if (mod != NULL)
    {
        if (event == LINE_STATE_CHANGE)
        {
            changeModList.removeFirstMatch(mod);
            changeModIdList.removeFirstMatch(portId);
        }
        else if (event == LINE_POS_EDGE)
        {
            posEdgeList.removeFirstMatch(mod);
            posEdgeIdList.removeFirstMatch(portId);
        }
        else if (event == LINE_NEG_EDGE)
        {
            negEdgeList.removeFirstMatch(mod);
            negEdgeIdList.removeFirstMatch(portId);
        }
    }
}

void Line::setAssDelay(unsigned int delay)
{
    if (delay == 0 && *delay_ != 0)
    {
        nextChangeTimeQueue.purge();
        valueQueue.purge();
    }

    *delay_ = delay;
}

void Line::hiddenUpdate(void)
{
    if (zeroDelayChangeTimeQueue.getSize() == 0)
    {
        return;
    }

    unsigned int time = SystemTimer::getHiddenTime(),
                 ct = zeroDelayChangeTimeQueue.getFirstInQueue();

    if (time >= ct)
    {
        zeroDelayChangeTimeQueue.deQueue();

        LineState value = zeroDelayValueQueue.getFirstInQueue();
        zeroDelayValueQueue.deQueue();

        setLineValue(value);
    }
}

void Line::update(void)
{
    if (nextChangeTimeQueue.getSize() == 0)
    {
        SystemTimer::stopLineNotification(refList);
        return;
    }

    unsigned int time = SystemTimer::getTime(), ct = nextChangeTimeQueue.getFirstInQueue();
    if (time >= ct)
    {
        SystemTimer::stopLineNotification(refList);
        nextChangeTimeQueue.deQueue();

        LineState value = valueQueue.getFirstInQueue();
        valueQueue.deQueue();

        setLineValue(value);
    }
}

void Line::operator=(const LineState& value)
{
    if (*delay_ != 0)
    {
        SystemTimer::notifyLine(refList);

        unsigned int nextTime = SystemTimer::getTime() + *delay_;

        nextChangeTimeQueue.removeLower(nextTime);
        valueQueue.removeLower(nextTime);

        nextChangeTimeQueue.enQueue(nextTime, nextTime);
        valueQueue.enQueue(value, nextTime);

        return;
    }
    else
    {
        if (zeroDelayValueQueue.getSize() == 0)
        {
            if (value == *val)
                return;
            else
            {
                SystemTimer::notifyHiddenTick(refList);
                zeroDelayChangeTimeQueue.enQueue(SystemTimer::getHiddenTime() + 1);
                zeroDelayValueQueue.enQueue(value);
            }
        }
        else if (zeroDelayValueQueue.getSize() == 1)
        {
            LineState qval = zeroDelayValueQueue.getFirstInQueue();
            if (qval == value)
                return;
            else if (*val == value)
            {
                zeroDelayChangeTimeQueue.deQueue();
                zeroDelayValueQueue.deQueue();
            }
            else
            {
                zeroDelayChangeTimeQueue.deQueue();
                zeroDelayValueQueue.deQueue();

                zeroDelayChangeTimeQueue.enQueue(SystemTimer::getHiddenTime() + 1);
                zeroDelayValueQueue.enQueue(value);
            }
        }

        return;
    }
}

const Line& Line::operator=(const Line &line)
{
    if (*refCount > 1)
    {
        (*refCount)--;
        refList->removeFirstMatch(this);

        val = line.val;
        delay_ = line.delay_;
        changeModList = line.changeModList;
        posEdgeList = line.posEdgeList;
        negEdgeList = line.negEdgeList;
        changeModIdList = line.changeModIdList;
        posEdgeIdList = line.posEdgeIdList;
        negEdgeIdList = line.negEdgeIdList;
        refCount = line.refCount;
        valueQueue = line.valueQueue;
        nextChangeTimeQueue = line.nextChangeTimeQueue;
        zeroDelayValueQueue = line.zeroDelayValueQueue;
        zeroDelayChangeTimeQueue = line.zeroDelayChangeTimeQueue;
        refList = line.refList;

        (*refCount)++;
        refList->append(this);
    }
    else
    {
        delete refCount; delete val; delete delay_; delete refList;

        val = line.val;
        delay_ = line.delay_;
        changeModList = line.changeModList;
        posEdgeList = line.posEdgeList;
        negEdgeList = line.negEdgeList;
        changeModIdList = line.changeModIdList;
        posEdgeIdList = line.posEdgeIdList;
        negEdgeIdList = line.negEdgeIdList;
        refCount = line.refCount;
        valueQueue = line.valueQueue;
        nextChangeTimeQueue = line.nextChangeTimeQueue;
        zeroDelayValueQueue = line.zeroDelayValueQueue;
        zeroDelayChangeTimeQueue = line.zeroDelayChangeTimeQueue;
        refList = line.refList;

        (*refCount)++;
        refList->append(this);
    }

    return *this;
}

void Line::setLineValue(const LineState &value)
{
    bool posEdge = false;
    if (value == HIGH && (*val == LOW || *val == UNKNOWN))
        posEdge = true;

    if (*val != value)
    {
        *val = value;

        ListIterator<Module*> changeIter = changeModList.getListIterator();
        ListIterator< int > changeIdIter = changeModIdList.getListIterator();

        changeIter.reset(); changeIdIter.reset();
        while (changeIter.hasNext())
        {
            Module *mod = changeIter.next();
            mod->onStateChange(changeIdIter.next());
        }

        if (posEdge)
        {
            ListIterator<Module*> posEdgeIter = posEdgeList.getListIterator();
            ListIterator< int > posEdgeIdIter = posEdgeIdList.getListIterator();

            posEdgeIter.reset(); posEdgeIdIter.reset();
            while (posEdgeIter.hasNext())
            {
                Module *mod = posEdgeIter.next();
                mod->onPosEdge(posEdgeIdIter.next());
            }
        }
        else
        {
            ListIterator<Module*> negEdgeIter = negEdgeList.getListIterator();
            ListIterator< int > negEdgeIdIter = negEdgeIdList.getListIterator();

            negEdgeIter.reset(); negEdgeIdIter.reset();
            while (negEdgeIter.hasNext())
            {
                Module *mod = negEdgeIter.next();
                mod->onNegEdge(negEdgeIdIter.next());
            }
        }
    }
}

ostream& operator<<(const std::ostream &osObj, const lcs::Line &line)
{
    if (line() == HIGH)
        return cout << "1";
    else if (line() == LOW)
        return cout << "0";
    else if (line() == UNKNOWN)
        return cout << "x";
    else
        return cout << "z";
}

const Line& operator~(Line &line)
{
    LineState state = line();
    if (state == LOW)
        line = HIGH;
    else if (state == HIGH)
        line = LOW;

    return line;
}

const LineState operator!(const Line &line)
{
    LineState state = line();
    if (state == LOW)
        return HIGH;
    else if (state == HIGH)
        return LOW;
    else
        return line();
}
