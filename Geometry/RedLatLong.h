// -------------------------------------------------------------------------------------------------
// This file is covered by: The MIT License (MIT) Copyright (c) 2016 David G. Steadman
// -------------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------------------------------
// (http://opensource.org/licenses/MIT)
// -------------------------------------------------------------------------------------------------

#pragma once

#include "RedCoreNamespace.h"
#include "RedNumberRange.h"

using namespace Red::Core;

namespace Red {
namespace Geometry {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const RedNumberRange kDegreesLatitudeRange  = RedNumberRange(-90,   90, 0); // 0 = default range behaviour to crop
static const RedNumberRange kDegreesLongitudeRange = RedNumberRange(-180, 180, kRedNumberRangeWrapOnUpper);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class RedLatLong
{
public:
      
    // Constructors
    RedLatLong(void)                                               { Init(); };
    RedLatLong(const RedNumber& nlat, const RedNumber& nlon)       { Set(nlat, nlon); };

    // Basic accessors
    void      Init(void)                                           { lat=0; lon=0; };
    void      Set(const RedLatLong& p)                             { lat=p.lat; lon=p.lon; };
    void      Set(const RedNumber& nlat, const RedNumber& nlon)    { lat=nlat; lon=nlon; };
    RedNumber Lat(void) const                                      { return lat; };
    RedNumber Lon(void) const                                      { return lon; };

    void      PlaceNumbersWithinRange(void) { kDegreesLatitudeRange.CropNumber(lat); kDegreesLongitudeRange.WrapNumber(lon); };

    RedString StringOfObject(void) const;
    int       PopulateFromString(const RedString& str);

    void operator =(const RedLatLong& p) { Set(p); };
    //operator==

private:
    RedNumber lat;
    RedNumber lon;
};

} // Geometry
} // Red


