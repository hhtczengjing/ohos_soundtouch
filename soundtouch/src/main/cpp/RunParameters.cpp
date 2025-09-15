////////////////////////////////////////////////////////////////////////////////
///
/// A class for parsing the 'soundstretch' application command line parameters
///
/// Author        : Copyright (c) Olli Parviainen
/// Author e-mail : oparviai 'at' iki.fi
/// SoundTouch WWW: http://www.surina.net/soundtouch
///
////////////////////////////////////////////////////////////////////////////////
//
// License :
//
//  SoundTouch audio processing library
//  Copyright (c) Olli Parviainen
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////

#include "RunParameters.h"

RunParameters::RunParameters()
{
    tempoDelta = 0;
    pitchDelta = 0;
    rateDelta = 0;
    quick = 0;
    noAntiAlias = 0;
    goalBPM = 0;
    speech = false;
    detectBPM = false;
}

void RunParameters::checkLimits()
{
    if (tempoDelta < -95.0f)
    {
        tempoDelta = -95.0f;
    }
    else if (tempoDelta > 5000.0f)
    {
        tempoDelta = 5000.0f;
    }
    
    if (pitchDelta < -60.0f)
    {
        pitchDelta = -60.0f;
    }
    else if (pitchDelta > 60.0f)
    {
        pitchDelta = 60.0f;
    }
    
    if (rateDelta < -95.0f)
    {
        rateDelta = -95.0f;
    }
    else if (rateDelta > 5000.0f)
    {
        rateDelta = 5000.0f;
    }
}
