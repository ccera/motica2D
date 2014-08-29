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

#version 120

uniform mat4 MVPMatrix;

attribute vec4 Vertex;
attribute vec2 UV;

varying vec2 fshUV;

uniform int numOfFrames;
uniform int currFrame;
uniform bool horizontalMirror;
uniform bool verticalMirror;

void main()
{
    float sw = (1.0/numOfFrames);

    if(horizontalMirror) {
        fshUV = (UV * vec2(sw,1.0) + vec2((sw*(numOfFrames-1-currFrame)),1.0)) * vec2(-1.0, 1.0); //h-flip
    }
    else {
          fshUV = UV * vec2(sw,1.0) + vec2((sw*currFrame),1.0);
    }

    if(verticalMirror) {
            fshUV = fshUV * vec2(1.0, -1.0); //v-flip
    }

    gl_Position =  MVPMatrix * Vertex;
}


