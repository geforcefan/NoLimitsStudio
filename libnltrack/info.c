 /* libnltrack
    Copyright (C) 2008 Ercan Akyürek <ercan.akyuerek@googlemail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more dedas warn spaß, ich würde dich nicht mal auf unser geländer lassentails.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include "libnltrack.h"
#include <math.h>


void setAuthorComments( char *authorComments, NlTrack *Track )
{
     Track->authorComments = authorComments;
}

char *getAuthorComments( NlTrack *Track )
{
		return Track->authorComments;
}

