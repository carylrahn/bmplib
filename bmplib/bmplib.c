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

#include "bmplib.h"

/// <summary>
///	Checks if a bitmap is valid and can be handled by this library </summary>
///
/// <param name="pBitmap">
/// Pointer to a bitmap file array
///
/// <returns>
/// number of bytes copied </returns>
int __stdcall blIsValidBitmap(__in void* pBitmap) {
    BITMAPFILEHEADER *pFileHdr = NULL;
    BITMAPINFOHEADER *pInfoHdr = NULL;

    pFileHdr = (BITMAPFILEHEADER*)pBitmap;
    if (pFileHdr->bfType != (uint16_t)BI_TYPE 
     || pInfoHdr->biPlanes > MAX_PLANES)
        return 0;
#if ! HANDLES_COMPRESSION
    if (pInfoHdr->biCompression != BI_RGB)
        return 0;
#endif
    return 1;
}

/// <summary>
///	Rips the raw pixel array from a bitmap </summary>
///
/// <param name="pBitmap">
/// Pointer to a bitmap file array
/// <param name="pBits">
/// Uninitialized array that will recieve non-padded bitmap pixel array
/// <param name="cbBufsize">
/// Maximum size of pBits
///
/// <returns>
/// number of bytes copied </returns>
size_t __stdcall blRipBits(__in void* pBitmap, __deref_out_bcount(cbBufsize) uint8_t* pBits, __in size_t cbBufsize) {
    BITMAPFILEHEADER *pFileHdr = NULL;
    BITMAPINFOHEADER *pInfoHdr = NULL;

    pFileHdr = (BITMAPFILEHEADER*)pBitmap;
    pInfoHdr = (BITMAPINFOHEADER*)((uint8_t*)pBitmap + sizeof(BITMAPFILEHEADER));
    return blWriteBits(pBits, (uint8_t*)pBitmap + pFileHdr->bfOffBits, pInfoHdr, cbBufsize, 1);
}

/// <summary>
///	Sets the pixel array and adds padding </summary>
///
/// <param name="pBitmap">
/// Pointer to a bitmap file array
/// <param name="pBits">
/// Initialized array containing pixel array
/// <param name="cbBufsize">
/// Maximum size of pBits
///
/// <returns>
/// number of bytes copied </returns>
size_t __stdcall blSetBits(__in void* pBitmap, __in_bcount(cbBufsize) const uint8_t* pBits, __in size_t cbBufsize) {
    BITMAPFILEHEADER *pFileHdr = NULL;
    BITMAPINFOHEADER *pInfoHdr = NULL;

    pFileHdr = (BITMAPFILEHEADER*)pBitmap;
    if (pFileHdr->bfType != (uint16_t)BI_TYPE)
        return 0;
    pInfoHdr = (BITMAPINFOHEADER*)((uint8_t*)pBitmap + sizeof(BITMAPFILEHEADER));
#if ! HANDLES_COMPRESSION
    if (pInfoHdr->biCompression != BI_RGB)
        return 0;
#endif
    return blWriteBits((uint8_t*)pBitmap + pFileHdr->bfOffBits, pBits, pInfoHdr, cbBufsize, 0);
}

/// <summary>
///	Copies the pixel array in pSrc to pDest and removes padding </summary>
///
/// <param name="pDest">
/// Uninitialized array that will recieve pixel data
/// <param name="pSrc">
/// Initialized array containing pixel data
/// <param name="pInfoHdr">
/// Initialized BITMAPINFOHEADER that corresponds to pSrc
/// <param name="cbBufsize">
/// Maximum size of pDest
/// <param name="iDirection">
/// If zero, then pSrc will be adjusted for padding at the end of each column.
/// If non-zero, pDest will be adjusted for padding.
///
/// <returns>
/// number of bytes copied </returns>
size_t __stdcall blWriteBits(__in_bcount(cbBufsize) uint8_t* pDest, __in const uint8_t* pSrc, __in BITMAPINFOHEADER* pInfoHdr, __in size_t cbBufsize, __in const int iDirection) {
    size_t  pad,
            col,
            row,
            i,
            cur = 0;
    char    bIsBroken = 0;
    
    pad = ((uint32_t)(abs(pInfoHdr->biWidth) * (pInfoHdr->biBitCount / CHAR_BIT)) % 4);
    if (pad)
        pad = 4 - pad;
    for (col = 0; col < (uint32_t)abs(pInfoHdr->biHeight); ++col) {
        for (row = 0; row < (uint32_t)abs(pInfoHdr->biWidth); ++row) {
            for (i = 0; i < pInfoHdr->biBitCount / CHAR_BIT; ++i) {
                if (++cur > cbBufsize) {
                    bIsBroken = 1;
                    break;
                }
                *pDest++ = *pSrc++;
            }
            if (bIsBroken)
                break;
        }
        if (bIsBroken)
            break;
        if (iDirection)
            pSrc += pad;
        else
            pDest += pad;
    }
    return cur;
}