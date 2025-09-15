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

#ifndef RUNPARAMETERS_H
#define RUNPARAMETERS_H

class RunParameters
{
public:
    // Change sound tempo by n percents  (n=-95..+5000 %)
    float tempoDelta;
    // Change sound pitch by n semitones (n=-60..+60 semitones)
    float pitchDelta;
    // Change sound rate by n percents   (n=-95..+5000 %)
    float rateDelta;
    // Use quicker tempo change algorithm (gain speed, lose quality)
    int   quick;
    // Don't use anti-alias filtering (gain speed, lose quality)
    int   noAntiAlias;
    // Detect the Beats-Per-Minute (BPM) rate of the sound and adjust the tempo to meet 'n' BPMs. When this switch is applied, the "-tempo" switch is ignored. If "=n" is omitted, i.e. switch "-bpm" is used alone, then the BPM rate is estimated and displayed, but tempo not adjusted according to the BPM value.
    bool  detectBPM;
    float goalBPM;
    // Tune algorithm for speech processing (default is for music)
    bool  speech;
    
public:
    void checkLimits();
    RunParameters();
};

#endif /* RUNPARAMETERS_H */
