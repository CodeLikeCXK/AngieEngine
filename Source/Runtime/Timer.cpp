/*

Hork Engine Source Code

MIT License

Copyright (C) 2017-2022 Alexander Samusev.

This file is part of the Hork Engine Source Code.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "Timer.h"
#include "World.h"

enum TIMER_STATE
{
    TIMER_STATE_FINISHED                 = 1,
    TIMER_STATE_PULSE                    = 2,
    TIMER_STATE_TRIGGERED_ON_FIRST_DELAY = 4
};

HK_CLASS_META(WorldTimer)

WorldTimer::WorldTimer()
{}

WorldTimer::~WorldTimer()
{}

void WorldTimer::Trigger()
{
    Callback();
}

void WorldTimer::Restart()
{
    State       = 0;
    NumPulses   = 0;
    ElapsedTime = 0;
}

void WorldTimer::Stop()
{
    State = TIMER_STATE_FINISHED;
}

bool WorldTimer::IsStopped() const
{
    return !!(State & TIMER_STATE_FINISHED);
}

void WorldTimer::Tick(World* World, float TimeStep)
{
    if (State & TIMER_STATE_FINISHED)
    {
        return;
    }

    if (bPaused)
    {
        return;
    }

    if (World->IsPaused() && !bTickEvenWhenPaused)
    {
        return;
    }

    if (State & TIMER_STATE_PULSE)
    {
        if (ElapsedTime >= PulseTime)
        {
            ElapsedTime = 0;
            if (MaxPulses > 0 && NumPulses == MaxPulses)
            {
                State = TIMER_STATE_FINISHED;
                return;
            }
            State &= ~TIMER_STATE_PULSE;
        }
        else
        {
            Trigger();
            if (State & TIMER_STATE_FINISHED)
            {
                // Called Stop() in trigger function
                return;
            }
            ElapsedTime += TimeStep;
            return;
        }
    }

    if (!(State & TIMER_STATE_TRIGGERED_ON_FIRST_DELAY))
    {
        if (ElapsedTime >= FirstDelay)
        {
            State |= TIMER_STATE_TRIGGERED_ON_FIRST_DELAY | TIMER_STATE_PULSE;

            NumPulses++;
            Trigger();
            if (State & TIMER_STATE_FINISHED)
            {
                // Called Stop() in trigger function
                return;
            }
            if (State == 0)
            {
                // Called Restart() in trigger function
                ElapsedTime += TimeStep;
                return;
            }
            ElapsedTime = 0;

            if (PulseTime <= 0)
            {
                if (MaxPulses > 0 && NumPulses == MaxPulses)
                {
                    State = TIMER_STATE_FINISHED;
                    return;
                }
                State &= ~TIMER_STATE_PULSE;
            }
        }
        else
        {
            ElapsedTime += TimeStep;
            return;
        }
    }

    if (ElapsedTime >= SleepDelay)
    {
        State |= TIMER_STATE_PULSE;
        NumPulses++;
        Trigger();
        if (State & TIMER_STATE_FINISHED)
        {
            // Called Stop() in trigger function
            return;
        }
        if (State == 0)
        {
            // Called Restart() in trigger function
            ElapsedTime += TimeStep;
            return;
        }
        ElapsedTime = 0;
        if (MaxPulses > 0 && NumPulses == MaxPulses)
        {
            State = TIMER_STATE_FINISHED;
            return;
        }
        if (PulseTime <= 0)
        {
            State &= ~TIMER_STATE_PULSE;
        }
    }
    ElapsedTime += TimeStep;
}
