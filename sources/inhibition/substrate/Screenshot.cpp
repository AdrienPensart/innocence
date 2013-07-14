#include "Screenshot.hpp"

#include <cstdio>
#include <windows.h>

#define HAVE_BOOLEAN
#include <utils/jpeg/jpeglib.h>

#include <common/Logger.hpp>
#include <common/Convert.hpp>

namespace Inhibition
{
    bool FillJpegBuffer(LPBYTE pBits, LPBITMAPINFOHEADER pbmih, int nSampsPerRow, JSAMPARRAY jsmpPixels)
    {
        if (pBits == 0 || nSampsPerRow <= 0)
        {
            return false;
        }

        int nRow = 0;
        int nPixel = 0;
        int nBytesWide =  (pbmih->biWidth*3);
        int nUnused    =  (((nBytesWide + 3) / 4) * 4) - nBytesWide;
        nBytesWide += nUnused;

        for (int r = 0; r < pbmih->biHeight; r++)
        {
            for (int p = 0, q = 0; p < (nBytesWide - nUnused); p+=3, q+=3)
            {
                nRow = (pbmih->biHeight-r-1) * nBytesWide;
                nPixel  = nRow + p;

                jsmpPixels[r][q+0] = pBits[nPixel+2]; //Red
                jsmpPixels[r][q+1] = pBits[nPixel+1]; //Green
                jsmpPixels[r][q+2] = pBits[nPixel+0]; //Blue
            }
        }
        return true;
    }

    bool JpegFromDib(LPBYTE    pBits,           // pointe sur les bits
                     LPBITMAPINFOHEADER pbmih,  // pointe sur bitmapinfoheader
                     int       nQuality,        // JPEG quality (0-100)
                     const char * pathJpeg)     // Pathname du fichier jpeg
    {
        // vérification des param.
        if (nQuality < 0 || nQuality > 100 || pBits == 0 || pathJpeg == "")
        {
            return false;
        }

        //utilise la libjpeg pour écrire les scanlines sur le disque
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr       jerr;

        int        nSampsPerRow; //longueur d'une ligne dans le buffer
        JSAMPARRAY jsmpArray;    //buffer des pixels RGB pour le fichier jpeg
        cinfo.err = jpeg_std_error(&jerr); //utilisation du default error handling

        jpeg_create_compress(&cinfo);

        FILE * pOutFile = 0;
        if ((pOutFile = fopen(pathJpeg, "wb")) == NULL)
        {
            jpeg_destroy_compress(&cinfo);
            return false;
        }

        jpeg_stdio_dest(&cinfo, pOutFile);
        cinfo.image_width      = pbmih->biWidth;  //Image width and height, in pixels
        cinfo.image_height     = pbmih->biHeight;
        cinfo.input_components = 3;              //Color components per pixel
        cinfo.in_color_space   = JCS_RGB; 	     //Colorspace of input image

        jpeg_set_defaults(&cinfo);              //set defaults parameters in cinfo
        jpeg_set_quality(&cinfo,nQuality,TRUE);
        jpeg_start_compress(&cinfo, TRUE);

        nSampsPerRow = cinfo.image_width * cinfo.input_components;

        //Allocate array of pixel RGB values
        jsmpArray = (*cinfo.mem->alloc_sarray)
                    ((j_common_ptr) &cinfo,
                     JPOOL_IMAGE,
                     nSampsPerRow,
                     cinfo.image_height);

        if (FillJpegBuffer(pBits,pbmih,nSampsPerRow,jsmpArray))
        {
            //Write the array of scan lines to the JPEG file
            (void)jpeg_write_scanlines(&cinfo,jsmpArray,cinfo.image_height);
        }

        jpeg_finish_compress(&cinfo); // Always finish
        fclose(pOutFile);
        jpeg_destroy_compress(&cinfo); // Free resources
        return true;
    }

    bool Screenshot::take(const std::string& fileJpg, int quality)
    {
        HDC hdcscreen = GetDC(0);
        if(!hdcscreen)
        {
            return false;
        }

        HDC hdcMem = CreateCompatibleDC(hdcscreen);
        if(!hdcMem)
        {
            return false;
        }

        UINT height = GetDeviceCaps(hdcscreen,VERTRES);
        UINT width = GetDeviceCaps(hdcscreen,HORZRES);

        BITMAPINFO bmi;
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth=width;
        bmi.bmiHeader.biHeight=height;
        bmi.bmiHeader.biPlanes=1;
        bmi.bmiHeader.biBitCount=24;
        bmi.bmiHeader.biCompression=BI_RGB;
        bmi.bmiHeader.biSizeImage=0;
        bmi.bmiHeader.biSizeImage     = 0 ;
        bmi.bmiHeader.biXPelsPerMeter = 0 ;
        bmi.bmiHeader.biYPelsPerMeter = 0 ;
        bmi.bmiHeader.biClrUsed       = 0 ;
        bmi.bmiHeader.biClrImportant  = 0 ;

        LPBITMAPINFOHEADER pbmih = &bmi.bmiHeader;

        BYTE * pBits = 0;
        HBITMAP hbmscreen = CreateDIBSection(0,&bmi,DIB_RGB_COLORS,(VOID **)&pBits,NULL,0);

        if(!hbmscreen)
        {
            return false;
        }
        SelectObject(hdcMem,hbmscreen);

        BitBlt(hdcMem,0,0,width,height,hdcscreen,0,0,SRCCOPY);
        JpegFromDib(pBits,pbmih,quality,fileJpg.c_str());

        DeleteObject(hbmscreen);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdcscreen);

        return true;
    }

} /* Inhibition */
