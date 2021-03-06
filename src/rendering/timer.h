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

#ifndef __Timer__
#define __Timer__
 
#include <QElapsedTimer>
#include <QDebug>

class Timer
{
public: 
    Timer();
    ~Timer();
    
    void start();
    void end();
    void end200();
    void stopwatch200();
    
private:
    QElapsedTimer timer;
    qint64 timerCounter;
    qint64 elapsed;
    qint64 selapsed;
};

#endif 
