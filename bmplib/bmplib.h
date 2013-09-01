/*
 * (C) Copyright 2013 x8esix.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3.0 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

/*
    bmplib:
        a small library to make ripping and setting pixel data in bmp files much easier.
        only tested with 24bit color spacing bmps, but should work with all (ignores
        color tables in 8bit though).

        http://github.com/x8esix/bmplib
    
    ~x8esix
*/

#pragma once

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <sal.h>

#pragma region Constants & Structs
    #include <Windows.h>
    #include <WinGDI.h>

    #define BI_TYPE "BM"
#pragma endregion

#pragma region Prototypes
    int __stdcall blIsValidBitmap(__in void* pBitmap);

    size_t __stdcall blRipBits(__in void* pBitmap, __deref_out_bcount(cbBufsize) void* pBits, __in size_t cbBufsize);
    size_t __stdcall blSetBits(__in void* pBitmap, __in_bcount(cbBufsize) const void* pBits, __in size_t cbBufsize);

    size_t __stdcall blWriteBits(__in_bcount(cbBufsize) uint8_t* pDest, __in const uint8_t* pSrc, __in BITMAPINFOHEADER* pInfoHdr, __in size_t cbBufsize, __in const int iDirection);
#pragma endregion

#pragma region Build Constants
    #define MAX_PLANES          1
    #define HANDLES_COMPRESSION 0
#pragma endregion

#pragma region Watermark
    static char szWatermark[] = "bmplib 0.1 \x03\x03 by x8esix";
#pragma endregion