//
//  Copyright (c) 2014 Emir Cerić. All rights reserved.
//
//  This file is part of Motica2D.
//
//  Motica2D is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Motica2D is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Motica2D.  If not, see <http://www.gnu.org/licenses/>.
//

#include "timer.h"

Timer::Timer()
{
    timerCounter = 0;
    selapsed = 0;
};

void Timer::start()
{
    timer.start();
    timerCounter++;
}

void Timer::end()
{
    qDebug() << "timer end " << (timer.nsecsElapsed() / 1000000.0f) << "ms";
    timerCounter = 0;
    elapsed = 0;
}

void Timer::end200()
{
    elapsed = elapsed + timer.nsecsElapsed();
    if(timerCounter > 200) {
        qDebug() << "timer average end " << ((elapsed/200.0f) / 1000000.0f) << "ms";
        timerCounter = 0;
        elapsed = 0;
    }
}

void Timer::stopwatch200()
{
    if(timerCounter > 0)
    {
        selapsed = selapsed + timer.nsecsElapsed();
        timer.start();
        if(timerCounter > 200) {
            qDebug() << "timer stopwatch " << ((selapsed/200.0f) / 1000000.0f) << "ms";
            timerCounter = 0;
            selapsed = 0;
        }
    }
    timerCounter++;
}

Timer::~Timer()
{
    
}
