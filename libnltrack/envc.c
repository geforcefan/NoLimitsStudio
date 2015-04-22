 /* libnltrack.
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

/*! \brief Set the environment filename
 *				
 *	\param envFilename the filename of the environment file
 *	\param reservedString the reserved string of the environment
 *	\param Track a pointer to the NlTrack
 *
 *  This function will set the environment file
 *
 */
int setEnvc(char *envFilename, char *reservedString, NlTrack *Track)
{
				Track->envcsize = 36 + strlen(envFilename) + strlen(reservedString);

				Track->envFilename = envFilename;
				Track->reservedString = reservedString;
}

/*! \brief Get the environment filename
 *				
 *	\param Track a pointer to the NlTrack
 *
 *	\return the environment filename
 *
 *  This function will get the environment filename
 *
 */
char* getEnvFilename(NlTrack *Track)
{
				return Track->envFilename;
}

/*! \brief Get the reserved environment filename
 *				
 *	\param Track a pointer to the NlTrack
 *
  *	\return the reserved environment filename
 *
 *  This function will get the reserved environment filename
 *
 */
char* getReservedString(NlTrack *Track)
{
				return Track->reservedString;
}

