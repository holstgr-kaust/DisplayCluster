/*********************************************************************/
/* Copyright (c) 2014, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#ifndef FFMPEGVIDEOFRAMECONVERTER_H
#define FFMPEGVIDEOFRAMECONVERTER_H

// required for FFMPEG includes below, specifically for the Linux build
#ifdef __cplusplus
    #ifndef __STDC_CONSTANT_MACROS
        #define __STDC_CONSTANT_MACROS
    #endif

    #ifdef _STDINT_H
        #undef _STDINT_H
    #endif

    #include <stdint.h>
#endif

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavutil/mem.h>
    #include <libswscale/swscale.h>
}

/**
 * Converts FFMPEG's AVFrame format to a data buffer of user-defined format
 */
class FFMPEGVideoFrameConverter
{
public:
    /**
     * Create a new converter
     * @param videoCodecContext The FFMPEG context to allocate resources
     * @param targetFormat The desired data output format (e.g. PIX_FMT_RGBA)
     */
    FFMPEGVideoFrameConverter(const AVCodecContext& videoCodecContext,
                              const PixelFormat targetFormat);
    /** Desturctor */
    ~FFMPEGVideoFrameConverter();

    /**
     * Convert an AVFrame to the target data format
     * @param srcFrame The source frame
     * @return true on success
     * @see getData()
     */
    bool convert(const AVFrame* srcFrame);

    /**
     * Get the converted data in the target format
     * @see convert()
     */
    const uint8_t* getData() const;

private:
    SwsContext * swsContext_;           // Scaling context
    AVFrame * avFrameRGB_;
};

#endif // FFMPEGVIDEOFRAMECONVERTER_H
