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

#include <lcs/systime.h>
#include <lcs/line.h>
#include <lcs/clock.h>

using namespace lcs;

unsigned int SystemTimer::time = 0;
unsigned int SystemTimer::hiddenTime = 0;
unsigned int SystemTimer::stoptime = 0;

List<TickListener*> SystemTimer::tlList = List<TickListener*>();
List<Line*> SystemTimer::lnList = List<Line*>();
Queue<Line*> SystemTimer::hiddenQueue = Queue<Line*>();

Clock *SystemTimer::clock = NULL;

void SystemTimer::setStopTime(unsigned int stopTime) { stoptime = stopTime; }

void SystemTimer::notifyTick(TickListener *tl)
{
    if (tl != NULL)
        tlList.append(tl);
}

void SystemTimer::notifyClock(Clock *clk)
{
    if (clk != NULL && clock == NULL)
        clock = clk;
}

void SystemTimer::notifyLine(Line *l)
{
    if (l != NULL)
        lnList.append(l);
}

void SystemTimer::notifyHiddenTick(Line *l)
{
    if (l != NULL)
        hiddenQueue.enQueue(l);
}

unsigned int SystemTimer::getTime(void) { return time; }

unsigned int SystemTimer::getHiddenTime(void) { return hiddenTime; }

void SystemTimer::reset(void) { time = 0; }

void SystemTimer::stopTickNotification(TickListener *tl)
{
    if (tl != NULL)
        tlList.remove(tl);
}

void SystemTimer::stopLineNotification(Line *ln)
{
    if (ln != NULL)
        lnList.remove(ln);
}

void SystemTimer::stopClockNotification()
{
    clock = NULL;
}

void SystemTimer::start(void)
{
    updateHiddenDelays();

    ListIterator<TickListener*> iter = tlList.getListIterator();
    iter.reset();
    while (iter.hasNext())
    {
        TickListener *tl = iter.next();
        if (tl != NULL)
            tl->onTick();
    }

    while (time < stoptime)
    {
        time++;

        ListIterator<Line*> lnIter = lnList.getListIterator();
        lnIter.reset();
        while (lnIter.hasNext())
        {
            Line *line = lnIter.next();
            line->update();
        }

        updateHiddenDelays();

        if (clock != NULL)
            clock->tick();

        updateHiddenDelays();

        ListIterator<TickListener*> iter = tlList.getListIterator();
        iter.reset();

        while (iter.hasNext())
        {
            TickListener *tl = iter.next();
            if (tl != NULL)
                tl->onTick();
        }
    }
}

void SystemTimer::updateHiddenDelays(void)
{
    while (hiddenQueue.getSize() != 0)
    {
        hiddenTime++;
        Line *hdLine = hiddenQueue.getFirstInQueue();

        hdLine->hiddenUpdate();
        hiddenQueue.deQueue();
    }

    hiddenTime = 0;
}
