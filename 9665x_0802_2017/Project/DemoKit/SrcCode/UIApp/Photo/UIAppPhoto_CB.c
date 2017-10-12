////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppPhoto.h"
#include "ImgCapCBMsg.h"
#include "ImgCapInfor.h"
#include "ExifVendor.h"
#include "GxSound.h"
#include "af_api.h"
#include "Flashlight_api.h"
#include "GxFlash.h"

#if (UI_STYLE==UI_STYLE_DEMO)
#include "Demo_Sound.h"
#endif
#include "dma.h"
//Morpho
#if STAB_FUNC
#include "morpho_rect_int.h"
#include "morpho_get_image_size.h"
#include "morpho_image_stabilizer3.h" // stability
#include "morpho_image_stabilizer3_internal.h" // Add
#endif
#if HDR_FUNC
#include "morpho_easy_hdr.h"          // hdr
#include "morpho_easy_hdr_internal.h" // Add
#endif
#if PIP_VIEW_FUNC
#include "PipView.h"
#endif

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiPhotoCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if HDR_FUNC
static UINT32 gImageIdx = 0;
#endif

void Photo_GenDateStrCB(void)
{
    // update the date time str
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_GEN_DATE_STR, 0);
}

void Photo_GenDatePicCB(IMG_CAP_DATASTAMP_INFO*  dateInfo)
{
    // Generate the date time picture
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_GEN_DATE_PIC, 1,(UINT32)dateInfo);
}

void Photo_WriteExifCB(IMG_CAP_EXIF_DATA* Info)
{
    EXIF_HANDLE_ID HandleID;
    // Write Exif
    HandleID = Info->Id;
    ExifVendor_Write0thIFD(HandleID);
    ExifVendor_WriteExifIFD(HandleID);
    ExifVendor_Write0thIntIFD(HandleID);
}


void Photo_WriteDebugCB(IMG_CAP_DEBUG_INFO *DebugInfo)
{
    UINT32 Addr;

    Addr = DebugInfo->Addr;
    Addr = AE_WriteDebugInfo(Addr);
    Addr = AWB_WriteDebugInfo(Addr);
    Addr = Flash_WriteDebugInfo(Addr);
    #if LENS_FUNCTION
    Addr = AF_WriteDebugInfo(Addr);
    #endif
    DebugInfo->UsedSize = Addr - DebugInfo->Addr;
    if(Addr - DebugInfo->Addr > DebugInfo->TotalSize)
        DBG_ERR("Debug message of JPEG end is overflow!!\r\n");
}

void Photo_ShutterSoundCB(void)
{
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_PLAY_SHUTTER_SOUND, 0);
}

void Photo_AFCB(AF_CBMSG uiMsgID, void *Data)
{
    UINT32 uiData;

    switch (uiMsgID)
    {
        case AF_CBMSG_AF_START:
           uiData = *(UINT32 *)Data;
           DBG_IND("===AF Start! - %d===\r\n", uiData);
           GxFlash_SuspendCharge();
           break;
        case AF_CBMSG_AF_DONE:
           uiData = *(UINT32 *)Data;
           DBG_IND("===AF Done! - %d===\r\n", uiData);
           GxFlash_ResumeCharge();
           Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND,1, NVTEVT_ALGMSG_FOCUSEND);
           break;
        default:
           break;
    }
}

#if STAB_FUNC
void ImageDataToMorpho(IPL_YCC_IMG_INFO* inImage, morpho_ImageDataEx* outImage)
{
    outImage->width  = inImage->Ch[0].Width;
    outImage->height = inImage->Ch[0].Height;
    outImage->dat.semi_planar.y  = (UINT8*)dma_getCacheAddr(inImage->PixelAddr[0]);
    outImage->dat.semi_planar.uv = (UINT8*)dma_getCacheAddr(inImage->PixelAddr[1]);
    outImage->pitch.semi_planar.y  = inImage->Ch[0].LineOfs; // Add
    outImage->pitch.semi_planar.uv = inImage->Ch[1].LineOfs; // Add

}

void Photo_StabCB(IMG_CAP_MFANTISHAKE_INFO* info)
{
    UINT32                   i;
    UINT32                   buffer_size;                                       /**< working memory size for PhotoSolid */
    UINT32                   image_num    = info->PicNum;                       /**< input number of images */
    UINT32                   image_width  = info->SrcImgInfo[0].Ch[0].Width;    /**< horizontal image size */
    UINT32                   image_height = info->SrcImgInfo[0].Ch[0].Height;   /**< vertical image size */
    UINT32                   image_offset = info->SrcImgInfo[0].Ch[0].LineOfs;  /**< Image lineoffset */
    CHAR                    *p_format    = "YUV422_SEMIPLANAR";                /**< format name for YUV422 (UVpacked) */
    UINT32                   image_size   = image_offset * ALIGN_CEIL_16(image_height) * 2;    /**< image data syze for YUV422 (UVpacked) */
    INT32                   ret          = MORPHO_OK;                          /**< return code */
    UINT8                   *p_temp_mem;                                       /**< address of allocated memory space */
    UINT8                   *p_temp_cache;//Novatek: cache for working buffer
    morpho_ImageDataEx      input_image;                                       /**< input image */
    morpho_ImageDataEx      output_image;                                      /**< output image */
    morpho_ImageDataEx      work_image;                                        /**< work   image */
    morpho_ImageStabilizer3 instance[1];                                       /**< instance of PhotoSolid */

    DBG_IND("Morpho PhotoSolid version = %s \r\n", morpho_ImageStabilizer3_getVersion());

    Perf_Mark();//Novatek
    //[Working Buffer][Image Buffer1][Image Buffer2]
    // ========== [initialize PhotoSolid] ==========
    {


        // allocate working memory
        buffer_size = morpho_ImageStabilizer3_getBufferSize(image_width, image_height, image_num, p_format);
        DBG_DUMP("buffer size: %d, image_width: %d, image_height: %d\r\n", buffer_size, image_width, image_height);//Novatek
        if (buffer_size+ image_size > info->WorkBufSize)
        {
            DBG_ERR("buff size %d < need %d\r\n",info->WorkBufSize,buffer_size + image_size);
            goto L_return;
        }
        //use cache
        p_temp_mem = (UINT8*)info->WorkBufAddr;
        p_temp_cache = (UINT8*)dma_getCacheAddr((UINT32)p_temp_mem);//Novatek

        // initialize engine
        //dma_flushReadCache((UINT32)p_temp_cache,buffer_size);
        //dma_flushWriteCache((UINT32)p_temp_cache,buffer_size);
        ret = morpho_ImageStabilizer3_initialize(instance, p_temp_cache, buffer_size);//Novatek
    }
    if( ret != MORPHO_OK ) goto L_return;


    // ========== [set image format] ==========
    ret = morpho_ImageStabilizer3_setImageFormat(instance, p_format);
    if (ret != MORPHO_OK) { DBG_ERR("ImageFormat (ret = 0x%08X)\r\n", ret); goto L_finalize; }//Novatek

    // ========== [set gain ] ==========
    //ret = morpho_ImageStabilizer3_setGain(instance,400);
    //if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }//Novatek

    // ========== [set noise reduction parameters] ==========
    ret = morpho_ImageStabilizer3_setLumaNoiseReductionLevel  (instance, 2); // default: 3
    if (ret != MORPHO_OK) { DBG_ERR("LumaNoiseReductionLevel (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
    ret = morpho_ImageStabilizer3_setChromaNoiseReductionLevel(instance, 2); // default: 3
    if (ret != MORPHO_OK) { DBG_ERR("ChromaNoiseReductionLevel (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
    ret = morpho_ImageStabilizer3_setLumaNoiseReductionType   (instance, MORPHO_IMAGE_STABILIZER3_NR_LITE); // default: MORPHO_IMAGE_STABILIZER3_NR_NORMAL
    if (ret != MORPHO_OK) { DBG_ERR("LumaNoiseReductionType (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
    ret = morpho_ImageStabilizer3_setChromaNoiseReductionType (instance, MORPHO_IMAGE_STABILIZER3_NR_LITE); // default: MORPHO_IMAGE_STABILIZER3_NR_NORMAL
    if (ret != MORPHO_OK) { DBG_ERR("ChromaNoiseReductionType (ret = 0x%08X)\r\n", ret);  goto L_finalize;}

    // ========== [set work image data] ==========
    /* Add */
        work_image.width  = image_width;
        work_image.height = image_height;
        work_image.dat.semi_planar.y  = (UINT8*)(info->WorkBufAddr+buffer_size);
        work_image.dat.semi_planar.uv = work_image.dat.semi_planar.y + image_offset * ALIGN_CEIL_16(image_height);;
        work_image.pitch.semi_planar.y  = image_offset;
        work_image.pitch.semi_planar.uv = image_offset;

    // ========== [set output image data] ==========
    {
        ImageDataToMorpho(&info->SrcImgInfo[0],&output_image);
        info->RstImgInfo = info->SrcImgInfo[0];
        ret = morpho_ImageStabilizer3_startEx(instance, &output_image,&work_image, image_num);
        if (ret != MORPHO_OK) { DBG_ERR("startEx (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
    }
    DBG_IND("Morpho initialize (ms) = %d\r\n", Perf_GetDuration()/1000);

    // ========== [process detection and alpha blending] ==========
    for(i = 0; i < image_num; i++){
        morpho_MotionData motion;

        // Image loading
        Perf_Mark();//Novatek

        ImageDataToMorpho(&info->SrcImgInfo[i],&input_image);
        dma_flushReadCache((UINT32)input_image.dat.p,image_size);


        Perf_Mark();//Novatek
        if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret);  goto L_finalize;}

        ret = morpho_ImageStabilizer3_detectMotionEx(instance, &input_image, &motion);
        if (ret != MORPHO_OK) { DBG_ERR("MotionEx (ret = 0x%08X)\r\n", ret);  goto L_finalize;}

        ret = morpho_ImageStabilizer3_renderEx(instance, &input_image, &motion);
        if (ret != MORPHO_OK) { DBG_ERR("renderEx (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
        DBG_IND("(%d/%d) Morpho process (ms) = %d\r\n", i+1, image_num, Perf_GetDuration()/1000);//Novatek
    }

    // ========== [apply noise reduction] ==========
    {
        Perf_Mark();//Novatek
        ret = morpho_ImageStabilizer3_reduceNoise(instance);
        if (ret != MORPHO_OK) { DBG_ERR("reduceNoise (ret = 0x%08X)\r\n", ret);  goto L_finalize;}
        DBG_IND("Morpho reduce noise (ms) = %d\r\n", Perf_GetDuration()/1000);//Novatek
    }

L_finalize:
    Perf_Mark();//Novatek
    ret = morpho_ImageStabilizer3_finalize(instance);
    //dma_flushWriteCache((UINT32)output_image.dat.p,image_size);
    DBG_IND("Morpho finalize (ms) = %d\r\n", Perf_GetDuration()/1000);//Novatek
L_return:
    return;
}
#endif

#if HDR_FUNC
void Photo_HDRCB(IMG_CAP_MFHDR_INFO* info)
{
    UINT32            i;
    UINT32            buffer_size;                                       /**< working memory size for PhotoSolid */
    UINT32            image_num    = info->PicNum;                       /**< input number of images */
    UINT32            image_width  = info->SrcImgInfo[0].Ch[0].Width;    /**< horizontal image size */
    UINT32            image_height = info->SrcImgInfo[0].Ch[0].Height;   /**< vertical image size */
    UINT32            image_offset = info->SrcImgInfo[0].Ch[0].LineOfs;  /**< Image lineoffset */
    CHAR             *p_format    = "YUV422_SEMIPLANAR";                /**< format name for YUV422 (UVpacked) */
    UINT32            image_size   = image_offset * ALIGN_CEIL_16(image_height) * 2;    /**< image data syze for YUV422 (UVpacked) */
    INT32            ret          = MORPHO_OK;                          /**< return code */
    UINT8            *p_temp_mem;                                       /**< address of allocated memory space */
    UINT8            *p_temp_cache;                                     //Novatek: cache for working buffer
    INT32            nthreads     = 0;                                  /**< number of parallel threads (0: parallelization disabled)*/
    morpho_ImageDataEx images[MORPHO_EASY_HDR_MAX_NIMAGES];             /**< input images */
    morpho_ImageDataEx *input_images[MORPHO_EASY_HDR_MAX_NIMAGES];      /**< pointer to an array of input images */
    morpho_ImageDataEx  output_image;                                   /**< output image */
    morpho_EasyHDR   instance[1];                                       /**< instance of Morpho HDR */
    int              ContrastLvl, SaturationLvl, value;


    DBG_IND("Morpho HDR version = %s \r\n", morpho_EasyHDR_getVersion());
    Perf_Mark();//Novatek
    // ========== [initialize Morpho HDR] ==========
    {
        // allocate buffer
        buffer_size = morpho_EasyHDR_getBufferSize(image_width, image_height, p_format);
        DBG_DUMP("buffer size: %d, image_width: %d, image_height: %d\r\n", buffer_size, image_width, image_height);//Novatek
        if (buffer_size + image_size> info->WorkBufSize)
        {
            DBG_ERR("buff size %d < need %d\r\n",info->WorkBufSize,buffer_size + image_size);
            goto L_return;
        }
        //use cache
        p_temp_mem = (UINT8*)info->WorkBufAddr;
        p_temp_cache = (UINT8*)dma_getCacheAddr((UINT32)p_temp_mem);//Novatek

        // initialize engine
        //dma_flushReadCache((UINT32)p_temp_cache,buffer_size);
        //dma_flushWriteCache((UINT32)p_temp_cache,buffer_size);

        ret = morpho_EasyHDR_initialize(instance, p_temp_cache, buffer_size, nthreads, NULL);//Novatek
        if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X), p_temp_cache = 0x%x\r\n", ret, p_temp_cache); info->RstImgInfo=info->SrcImgInfo[0]; goto L_return; }
    }
    DBG_IND("Morpho initialize (ms) = %d\r\n", Perf_GetDuration()/1000);//Novatek

    // ========== [set output image data] ==========    {

    //ImageDataToMorpho(&info->SrcImgInfo[0],&output_image);
    //info->RstImgInfo = info->SrcImgInfo[0];

    // ========== [allocate memory for output image] ==========
    {
        IPL_YCC_IMG_INFO    iplOutImg={0};

        iplOutImg = info->SrcImgInfo[0];
        iplOutImg.PixelAddr[0] = dma_getCacheAddr(info->WorkBufAddr+buffer_size);
        iplOutImg.PixelAddr[1] = iplOutImg.PixelAddr[0] + image_size/2;
        info->RstImgInfo = iplOutImg;
        ImageDataToMorpho(&iplOutImg,&output_image);

    }

    // ========== [load image files] ==========
    for (i = 0; i < image_num; ++i)
    {
        Perf_Mark();//Novatek

        input_images[i] =  &images[i];
        ImageDataToMorpho(&info->SrcImgInfo[i],input_images[i]);
        dma_flushReadCache((UINT32)input_images[i]->dat.p,image_size);

        #if 0
        {
            FST_FILE     filehdl = NULL;
            CHAR         path[60];
            UINT32       uiBufSize = image_offset*image_height;

            sprintf(path, "A:\\HDR\\%04d_%dx%d_y", i,image_width,image_height);
            filehdl = FileSys_OpenFile(path,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
            if (filehdl)
            {
                FileSys_WriteFile(filehdl, (UINT8 *)info->SrcImgInfo[i].PixelAddr[0], &uiBufSize, 0, NULL);
                FileSys_CloseFile(filehdl);
            }
            sprintf(path, "A:\\HDR\\%04d_%dx%d_uv", i,image_width,image_height);
            filehdl = FileSys_OpenFile(path,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
            if (filehdl)
            {
                FileSys_WriteFile(filehdl, (UINT8 *)info->SrcImgInfo[i].PixelAddr[1], &uiBufSize, 0, NULL);
                FileSys_CloseFile(filehdl);
            }
        }
        #endif

        if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_return; }

    }
    Perf_Mark();//Novatek

    morpho_EasyHDR_getFMColorCorrectionLevels(instance, &ContrastLvl, &SaturationLvl);
    DBG_IND("ContrastLvl=%d, SaturationLvl=%d\r\n",ContrastLvl,SaturationLvl);

    morpho_EasyHDR_getGhostDetectionSensitivityLevel(instance, &value);
    DBG_IND("SensitivityLevel=%d\r\n",value);

    morpho_EasyHDR_getReliableRectRateThreshold(instance, &value);
    DBG_IND("ReliableRectRateThreshold=%d\r\n",value);

    morpho_EasyHDR_getGhostRateThreshold(instance, &value);
    DBG_IND("GhostRateThreshold=%d\r\n",value);




    // ========== [set parameters to Morpho HDR] ==========
    ret = morpho_EasyHDR_setImageFormat(instance, p_format);
    if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }

    ret = morpho_EasyHDR_setFMColorCorrectionLevels(instance, 8, 2); // contrast 8 , saturation 2
    if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }

    /* Set ghost detection sensitivity level */
    //ret = morpho_EasyHDR_setGhostDetectionSensitivityLevel(instance, 7);
    //if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }
    /* Set valid rectangle threshold value */
    //ret = morpho_EasyHDR_setReliableRectRateThreshold(instance, 80);
    //if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }
    /* Set ghost rate threshold value */
    //ret = morpho_EasyHDR_setGhostRateThreshold(instance, 85);
    //if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); goto L_finalize; }



    // ========== [process Morpho HDR] ==========
    DBG_IND("morpho_EasyHDR process start\r\n");
    ret = morpho_EasyHDR_mergeEx(instance, &output_image, input_images, image_num);
    while (ret == MORPHO_DOPROCESS)
    {
        ret = morpho_EasyHDR_process(instance);
    }
    DBG_IND("morpho_EasyHDR process end\r\n");

    if (morpho_EasyHDR_getMergeStatus(instance) != MORPHO_EASY_HDR_OK)
    {
        switch(morpho_EasyHDR_getMergeStatus(instance)){
            case MORPHO_EASY_HDR_ERROR_IMAGE_ALIGNMENT_FAILURE:
                DBG_ERR("Merge finished with an error: alignment failure\r\n");
                break;
            case MORPHO_EASY_HDR_ERROR_EXP_ESTIMATION_FAILURE:
                DBG_ERR("Merge finished with an error: estimation failure\r\n");
                break;
            case MORPHO_EASY_HDR_ERROR_MOSTLY_GHOST:
                DBG_ERR("Merge finished with an error: mostly ghost\r\n");
                break;
            case MORPHO_EASY_HDR_ERROR_INTERNAL:
                DBG_ERR("Merge finished with an internal error\r\n");
                break;
        }
    }
    DBG_IND("Morpho process (ms) = %d\r\n", Perf_GetDuration()/1000);//Novatek

    // ========== [finalize Morpho HDR] ==========
L_finalize:
    Perf_Mark();//Novatek
    ret = morpho_EasyHDR_finalize(instance);
    //dma_flushWriteCache((UINT32)output_image.dat.p,image_size);
    if (ret != MORPHO_OK) { DBG_ERR("ERROR (ret = 0x%08X)\r\n", ret); }
    DBG_IND("Morpho finalize (ms) = %d\r\n", Perf_GetDuration()/1000);//Novatek
L_return:
    return;
}
void Photo_OneRawEndCB(void)
{
    // HDR take 3 pictures
    if (ImgCap_GetUIInfo(CAP_SEL_DRIVEMODE) == SEL_DRIVEMODE_MULFMHDR)
    {
        gImageIdx++;
        DBG_IND("HDR RAW %d\r\n",gImageIdx);

        // set next image
        if (gImageIdx == 1)
            AEB_SetRatio2Cap(AEB_GetRatioFromEV(SEL_AEEV_N1));
        else if(gImageIdx == 2)
            AEB_SetRatio2Cap(AEB_GetRatioFromEV(SEL_AEEV_P1));
        else
        {
            AEB_SetRatio2Cap(AEB_GetRatioFromEV(SEL_AEEV_00));
            gImageIdx = 0;
        }
    }
}
#endif

void Photo_RegCB(void)
{
    DscPhoto_RegDateStrCB(Photo_GenDateStrCB);
    DscPhoto_RegDatePicCB(Photo_GenDatePicCB);
    DscPhoto_RegWriteExifCB((WRITE_EXIF_CB_FP)Photo_WriteExifCB);
    DscPhoto_RegShutterSoundCB(Photo_ShutterSoundCB);
    DscPhoto_RegWriteDebugCB(Photo_WriteDebugCB);
    #if STAB_FUNC
    DscPhoto_RegStabCB(Photo_StabCB);
    #endif
    #if HDR_FUNC
    DscPhoto_RegHDRCB(Photo_HDRCB);
    DscPhoto_RegOneRawEndCB(Photo_OneRawEndCB);
    #endif

    #if PIP_VIEW_FUNC
    DscPhoto_RegChgDispSizeInforCB(PipView_ActivePrimaryImgRatio);
    #endif
}
