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
    : changeModList(List<Module*>()), posEdgeList(List<Module*>()),
      negEdgeList(List<Module*>()), changeModIdList(List<int>()),
      posEdgeIdList(List<int>()), negEdgeIdList(List<int>()),
      nextChangeTimeQueue(Queue<unsigned int>()),
      zeroDelayChangeTimeQueue(Queue<unsigned int>()),
      valueQueue(Queue<LineState>()), zeroDelayValueQueue(Queue<LineState>())
{
    val = new LineState; *val = UNKNOWN;
    refCount = new int;
    *refCount = 1;

    delay_ = new unsigned int;
    *delay_ = 0;

    purgedVal = new LineState; *purgedVal = UNKNOWN;
    purgeTime = new unsigned int; *purgeTime = 0;
    purgedChangeTime = new unsigned int; *purgedChangeTime = 0;

    lockedModPtr = new DataPtr<Module>();
    lockedModPtr->data = NULL;
}

Line::Line(const Line& line)
    : val(line.val), purgedVal(line.purgedVal),
      refCount(line.refCount), delay_(line.delay_),
      purgeTime(line.purgeTime), purgedChangeTime(line.purgedChangeTime),
      changeModList(line.changeModList),
      posEdgeList(line.posEdgeList), negEdgeList(line.negEdgeList),
      changeModIdList(line.changeModIdList), posEdgeIdList(line.posEdgeIdList),
      negEdgeIdList(line.negEdgeIdList),
      nextChangeTimeQueue(line.nextChangeTimeQueue),
      zeroDelayChangeTimeQueue(line.zeroDelayChangeTimeQueue),
      valueQueue(line.valueQueue), zeroDelayValueQueue(line.zeroDelayValueQueue),
      lockedModPtr(line.lockedModPtr)
{
    (*refCount)++;
}

Line::~Line()
{
    if (*refCount <= 1)
    {
        delete val; delete refCount; delete lockedModPtr; delete delay_;
    }
    else
        (*refCount)--;
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
            changeModList.remove(mod);
            changeModIdList.remove(portId);
        }
        else if (event == LINE_POS_EDGE)
        {
            posEdgeList.remove(mod);
            posEdgeIdList.remove(portId);
        }
        else if (event == LINE_NEG_EDGE)
        {
            negEdgeList.remove(mod);
            negEdgeIdList.remove(portId);
        }
    }
}

void Line::lock(Module *mod, unsigned int delay)
{
    if (lockedModPtr->data == NULL)
    {
        lockedModPtr->data = mod;
        *delay_ = delay;
    }
    else if (lockedModPtr->data == mod)
        ; // Do nothing as the line is already locked by this module.
    else
    {
        cerr << "Trying to lock an already locked module." << endl
             << "Expect undefined behaviour and/or a ShortCircuitException!" << endl;
    }
}

void Line::unLock(Module *mod)
{
    if (lockedModPtr->data == mod)
    {
        lockedModPtr->data = NULL;
        *delay_ = 0;
    }
    else if (lockedModPtr->data == NULL)
        ; // Do nothing as the line isnt locked by any module.
    else
    {
        cerr << "Trying to un-lock a line locked by another module." << endl
             << "Expect undefined behaviour and/or a ShortCircuitException!" << endl;
    }
}

bool Line::isLocked(void)
{
    if (lockedModPtr->data == NULL)
        return false;
    else
        return true;
}

Bus<1> Line::makeBus(void)
{
    Bus<1> bus;
    Bus<>::LineAccessor lines(bus.getLineAccessor(NULL));

    lines[0] = *this;

    return bus;
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
        SystemTimer::stopLineNotification(this);
        return;
    }

    unsigned int time = SystemTimer::getTime(), ct = nextChangeTimeQueue.getFirstInQueue();
    if (time >= ct)
    {
        SystemTimer::stopLineNotification(this);
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
        int qsize = nextChangeTimeQueue.getSize();
        unsigned int crtime = SystemTimer::getTime();

        if (qsize == 0)
        {
            if (*purgedChangeTime != 0 && *purgeTime == crtime)
            {
                if (*purgedVal == value)
                {
                    SystemTimer::notifyLine(this);
                    nextChangeTimeQueue.enQueue(*purgedChangeTime);
                    valueQueue.enQueue(value);
                }

                return;
            }
            // Dont schedule a line state change is the same value is requested.
            if (*val == value)
                return;

            SystemTimer::notifyLine(this);
            nextChangeTimeQueue.enQueue(SystemTimer::getTime() + *delay_);
            valueQueue.enQueue(value);
        }
        else
        {
            LineState nextVal = valueQueue.getFirstInQueue();
            // Do nothing if the value requested to be set is the same as the
            // value to be set next.
            if (nextVal == value)
                return;

            unsigned int nxtime = nextChangeTimeQueue.getFirstInQueue();

            if (crtime <= nxtime && ((crtime + *delay_) > nxtime))
            {
                valueQueue.deQueue();
                nextChangeTimeQueue.deQueue();
                SystemTimer::stopLineNotification(this);

                *purgedVal = nextVal;
                *purgedChangeTime = nxtime;
                *purgeTime = crtime;
            }
            else if ((crtime + *delay_) == nxtime)
            {
                valueQueue.deQueue();
                valueQueue.enQueue(value);
            }
            else
            {
                SystemTimer::notifyLine(this);
                nextChangeTimeQueue.enQueue(SystemTimer::getTime() + *delay_);
                valueQueue.enQueue(value);
            }
        }

        return;
    }
    else if (*delay_ == 0 && lockedModPtr->data != NULL)
    {
        SystemTimer::notifyHiddenTick(this);
        zeroDelayChangeTimeQueue.enQueue(SystemTimer::getHiddenTime() + 1);
        zeroDelayValueQueue.enQueue(value);

        return;
    }

    setLineValue(value);
}

const Line& Line::operator=(const Line &line)
{
    if (*refCount > 1)
    {
        (*refCount)--;

        val = line.val;
        purgedVal = line.purgedVal;
        delay_ = line.delay_;
        purgeTime = line.purgeTime;
        purgedChangeTime = line.purgedChangeTime;
        changeModList = line.changeModList;
        posEdgeList = line.posEdgeList;
        negEdgeList = line.negEdgeList;
        changeModIdList = line.changeModIdList;
        posEdgeIdList = line.posEdgeIdList;
        negEdgeIdList = line.negEdgeIdList;
        refCount = line.refCount;
        lockedModPtr = line.lockedModPtr;
        valueQueue = line.valueQueue;
        nextChangeTimeQueue = line.nextChangeTimeQueue;
        zeroDelayValueQueue = line.zeroDelayValueQueue;
        zeroDelayChangeTimeQueue = line.zeroDelayChangeTimeQueue;

        (*refCount)++;
    }
    else
    {
        delete refCount; delete val; delete lockedModPtr; delete delay_;

        val = line.val;
        purgedVal = line.purgedVal;
        delay_ = line.delay_;
        purgeTime = line.purgeTime;
        purgedChangeTime = line.purgedChangeTime;
        changeModList = line.changeModList;
        posEdgeList = line.posEdgeList;
        negEdgeList = line.negEdgeList;
        changeModIdList = line.changeModIdList;
        posEdgeIdList = line.posEdgeIdList;
        negEdgeIdList = line.negEdgeIdList;
        refCount = line.refCount;
        lockedModPtr = line.lockedModPtr;
        valueQueue = line.valueQueue;
        nextChangeTimeQueue = line.nextChangeTimeQueue;
        zeroDelayValueQueue = line.zeroDelayValueQueue;
        zeroDelayChangeTimeQueue = line.zeroDelayChangeTimeQueue;

        (*refCount)++;
    }

    return *this;
}

void Line::setLineValue(const LineState &value)
{
    bool posEdge = false;
    if (value == HIGH && (*val == LOW || *val == UNKNOWN))
        posEdge = true;

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
