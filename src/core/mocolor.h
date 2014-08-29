//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
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

#ifndef MOCOLOR_H
#define MOCOLOR_H

class MoColor
{
public:
    MoColor();
    MoColor(float r, float g, float b);
    MoColor(float r, float g, float b, float a);

    void SetColor(float r, float g, float b);
    void SetColor(float r, float g, float b, float a);
    float GetR() const;
    float GetG() const;
    float GetB() const;
    float GetA() const;

    //Operators
    MoColor& operator=(const MoColor &other);
private:
    float r;
    float g;
    float b;
    float a;
};

#endif // MOCOLOR_H
