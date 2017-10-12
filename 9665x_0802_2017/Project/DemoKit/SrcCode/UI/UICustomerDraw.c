#include "UIGraphics.h"
#include "UIDisplay.h"

#include "PrjCfg.h"
#include "UIInfo.h"
#include "FlowScene.h"
#if (_QFUNC_UI_ == _QFUNC_UI_NEW_)
#include "FlowQFunc2.h"
#else
#include "FlowQFunc.h"
#endif


/////////////////////////////////////////////////////////////////////////
//
//  Customize Drawing Function
//
/////////////////////////////////////////////////////////////////////////

//#NT#2009/12/18#Jeah Yen -begin
RESULT UI_ClearRect(DC* pDestDC,LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2)
{
    GxGfx_SetShapeStroke(SHAPELINESTYLE_DEFAULT,SHAPEFILLSTYLE_DEFAULT);
    GxGfx_SetShapeColor(SHAPEFORECOLOR_DEFAULT,VDO_TRANSPARENT_COLOR,SHAPEMAPPING_DEFAULT);
    GxGfx_SetShapeLayout(SHAPELAYOUT_DEFAULT,SHAPEALIGN_DEFAULT);
    return GxGfx_FillRect(pDestDC,x1, y1, x2, y2);
}
//#NT#2009/12/18#Jeah Yen -end



#if (DRAW_TXT_BY_ICON==ENABLE)
/**
    UI_DrawUserShowText is draw text by font or icon depend on
    different language

    User can use Ux_SetShowObjFunc replace default draw text func
    ex:Ux_SetShowObjFunc(CMD_Text, (SHOWOBJ_FUNC_PTR)UI_DrawUserShowText);

    gDemo_ARString is the icon language(Arabic),icon sequence is the same with other stings
*/
UINT32 LangColorMapping[4]={0,1,2,3};

void UI_DrawUserShowText(UIScreen ScreenObj,ITEM_BASE* pOrgShowObj)
{
    ITEM_TEXT* pShowObj = (ITEM_TEXT*)pOrgShowObj;
    UINT32 ImageAlignment=pShowObj->uiAlignment;
    VControl *pFocusedWnd=0;
    if(pShowObj->Content==STRID_NULL)
        return;
#if (ARABICRIGHT_FUNCTION == ENABLE)
    if((pShowObj->uiAlignment&0xF)==ALIGN_LEFT)
    {
        ImageAlignment = (pShowObj->uiAlignment & ~0xF)|ALIGN_RIGHT;
    }
#endif
    Ux_GetFocusedWindow(&pFocusedWnd);
    if(pFocusedWnd == &FlowSceneCtrl)
    {
        if( (pShowObj->uiLayout == LAYOUT_LINEBREAK) &&
            ((pShowObj->uiAlignment&0xF)==ALIGN_LEFT) )
        {
            ImageAlignment = (pShowObj->uiAlignment & ~0xF)|ALIGN_RIGHT;
        }
    }

    if(pShowObj->Content&TEXT_POINTER)
    {
        GxGfx_SetTextColor(pShowObj->uiTextColor,pShowObj->uiShadowColor,pShowObj->uiLineColor);
        GxGfx_SetTextStroke(Ux_GetFontTable((UINT32)pShowObj->uiFontTable), pShowObj->uiFontStyle, 0);
        GxGfx_SetTextLayout(pShowObj->uiLayout,ImageAlignment,pShowObj->uiLineHeight, pShowObj->uiLetterSpace, pShowObj->uiIndentSpace);
        pShowObj->Content  =(pShowObj->Content)&~TEXT_POINTER;
        GxGfx_TextPrintInRect(((DC**)ScreenObj)[GxGfx_OSD],  pShowObj->x1,pShowObj->y1,pShowObj->x2,pShowObj->y2 ,"%s" ,pShowObj->Content);
    }
#if(_USR_WND_==ENABLE)
//if some string id dispaly Eng when change to Arabic
    else if((pShowObj->Content == STRID_OOOOOOOOOOOOOOOOOOOOP11)||
    (pShowObj->Content == STRID_SETUP_EIS))
    {   //use Eng string
        GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageTable());
        GxGfx_SetTextColor(pShowObj->uiTextColor,pShowObj->uiShadowColor,pShowObj->uiLineColor);
        GxGfx_SetTextStroke(Ux_GetFontTable((UINT32)pShowObj->uiFontTable), pShowObj->uiFontStyle, 0);
        GxGfx_SetTextLayout(pShowObj->uiLayout,ImageAlignment,pShowObj->uiLineHeight, pShowObj->uiLetterSpace, pShowObj->uiIndentSpace);
        GxGfx_TextInRect(((DC**)ScreenObj)[GxGfx_OSD],  pShowObj->x1,pShowObj->y1,pShowObj->x2,pShowObj->y2 , GxGfx_GetStringID((UINT16)pShowObj->Content));
    }
#endif
    else  // draw language icon
    {
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#if (MULTILANG_FUNCTION == ENABLE)
        if(UI_GetData(FL_LANGUAGE)==AR)
            GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_ARString);
        else
            GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_PERString);
#endif
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
        GxGfx_SetImageLayout(pShowObj->uiLayout,ImageAlignment);
        GxGfx_SetImageStroke(ROP_KEY,_OSD_INDEX_TRANSPART);
        LangColorMapping[1]=pShowObj->uiTextColor;
        LangColorMapping[2]=_OSD_INDEX_TRANSPART;
        GxGfx_SetImageColor(IMAGEPALETTE_DEFAULT,LangColorMapping);
        GxGfx_ImageInRect(((DC**)ScreenObj)[GxGfx_OSD],pShowObj->x1,pShowObj->y1,pShowObj->x2,pShowObj->y2, GxGfx_GetImageID((UINT16)pShowObj->Content));
#if (UI_STYLE==UI_STYLE_DRIVER)
        GxGfx_SetImageTable((const IMAGE_TABLE*)gDemoKit_Image);
#else
        GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_Image);
#endif
    }


}

//#NT#2011/04/13#Jeah Yen - begin
extern void Ux_DrawShowText(UIScreen ScreenObj,ITEM_TEXT* pShowObj);

void UI_DrawUserShowText2(UIScreen ScreenObj,ITEM_BASE* pOrgShowObj)
{
    ITEM_TEXT* pShowObj = (ITEM_TEXT*)pOrgShowObj;
    UINT32 ImageAlignment=pShowObj->uiAlignment;
    VControl *pFocusedWnd=0;
    #if (_QFUNC_UI_ == _QFUNC_UI_NEW_)
    VControl *pQFuncWnd =&FlowQFunc2Ctrl;
    #else
    VControl *pQFuncWnd =&FlowQFuncCtrl;
    #endif
//#NT#2011/04/22#Janice Huang -begin
//#NT# user text do not use backward,because it is not be reversed by UI tools
    if(pShowObj->Content&TEXT_POINTER)
    {
        Ux_DrawShowText(ScreenObj, pShowObj);
    }
    else
    {
        Ux_GetFocusedWindow(&pFocusedWnd);
        if((pFocusedWnd == &FlowSceneCtrl)||
           (pFocusedWnd == pQFuncWnd))
        {
            if( (pShowObj->uiLayout == LAYOUT_LINEBREAK) &&
                ((pShowObj->uiAlignment&0xF)==ALIGN_LEFT) )
            {
                ImageAlignment = (pShowObj->uiAlignment & ~0xF)|ALIGN_RIGHT;
                pShowObj->uiAlignment = ImageAlignment;
            }
        }
        pShowObj->uiAlignment |= STREAMALIGN_BACKWARD;
        Ux_DrawShowText(ScreenObj, pShowObj);
        pShowObj->uiAlignment &= ~STREAMALIGN_BACKWARD;
    }
//#NT#2011/04/22#Janice Huang -end
}
//#NT#2011/04/13#Jeah Yen - end

/**
    UI_Set2StaticStrPos can exchange two static control left to rigit
    some language (Arabic) would aligment to right,so need to exchange
*/
#if (UI_STYLE==UI_STYLE_DEMO)
#define INTVAL 4
void UI_Set2StaticStrPos(VControl *pLeftStrCtrl,VControl *pRightStrCtrl)
{
    Ux_RECT LeftRect,RightRect;
    UINT32 RightWidth,LeftWidth;

    UINT32 LeftstrId = UxStatic_GetData(pLeftStrCtrl, STATIC_VALUE);
    UINT32 RightstrId = UxStatic_GetData(pRightStrCtrl, STATIC_VALUE);
    ISIZE Leftsize,Rightsize;

    UxCtrl_GetPos(pLeftStrCtrl,&LeftRect);
    UxCtrl_GetPos(pRightStrCtrl,&RightRect);
    RightWidth = RightRect.x2-RightRect.x1;
    LeftWidth = LeftRect.x2-LeftRect.x1;


    //#NT#2010/03/15#Lincy Lin -begin
    GxGfx_SetTextStroke((const FONT*)UIFontTable[0], FONTSTYLE_NORMAL, SCALE_1X);
    //#NT#2010/03/15#Lincy Lin -end

    if((UI_GetData(FL_LANGUAGE) != AR)&&(UI_GetData(FL_LANGUAGE) != PER))
    {
        if(UI_GetData(FL_LANGUAGE)== HEB)
        {
            if(RightstrId&TEXT_POINTER)
            {
                RightstrId = RightstrId&~TEXT_POINTER;
                GxGfx_Text(0, 0, 0, (const TCHAR*)RightstrId); //not really draw
                debug_ind(("RightstrId %d \r\n",RightstrId));
            }
            else
            {
                GxGfx_Text(0, 0, 0, GxGfx_GetStringID(RightstrId)); //not really draw
            }

            Rightsize = GxGfx_GetTextLastSize();
            if(RightRect.x1 > LeftRect.x1)
            {

                RightRect.x1 = LeftRect.x1;
                RightRect.x2 = RightRect.x1 + Rightsize.w;

                LeftRect.x1 = RightRect.x2 + INTVAL;
                LeftRect.x2 = LeftRect.x1 + LeftWidth;
            }
            else
            {
                debug_ind(("not change  right %d left %d\r\n",RightRect.x1,LeftRect.x1));
                RightRect.x2 = RightRect.x1 + Rightsize.w;

                LeftRect.x1 = RightRect.x2 + INTVAL;
                LeftRect.x2 = LeftRect.x1 + LeftWidth;
            }
            UxCtrl_SetPos(pLeftStrCtrl,LeftRect);
            UxCtrl_SetPos(pRightStrCtrl,RightRect);
            debug_ind(("LeftRect %d %d %d\r\n",LeftRect.x1,LeftRect.x2,LeftWidth));
            debug_ind(("RightRect %d %d %d\r\n",RightRect.x1,RightRect.x2,Rightsize.w));

        }
        else
        {
            GxGfx_Text(0, 0, 0, GxGfx_GetStringID(LeftstrId)); //not really draw
            Leftsize = GxGfx_GetTextLastSize();

            RightRect.x1 = LeftRect.x1+Leftsize.w+INTVAL;
            RightRect.x2 = RightRect.x1+RightWidth;
            UxCtrl_SetPos(pRightStrCtrl,RightRect);
            debug_msg("RightRect.x1=%d\r\n", RightRect.x1);
            debug_msg("RightRect.x2=%d\r\n", RightRect.x2);
        }
    }
    else //change left to right
    {
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -begin
#if (MULTILANG_FUNCTION == ENABLE)
        if(UI_GetData(FL_LANGUAGE)==AR)
            GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_ARString);
        else
            GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_PERString);
#endif
//#NT#2010/04/15#Jeah Yen -FASTBOOT- -end
        GxGfx_GetImageIDSize(RightstrId,&Rightsize);
        GxGfx_GetImageIDSize(LeftstrId,&Leftsize);
        GxGfx_SetImageTable((const IMAGE_TABLE*)gDemo_Image);

        //#NT#2011/02/15#Janice Huang -begin
        //#NT# change AR alignment
        #if (ARABICRIGHT_FUNCTION == ENABLE) //alignment to right
        RightRect.x2 = LeftRect.x2-Leftsize.w-INTVAL;
        RightRect.x1 = RightRect.x2-RightWidth;
        UxCtrl_SetPos(pRightStrCtrl,RightRect);
        #else //alignment to left

        //check if user string or string id
        if(RightstrId&TEXT_POINTER)
        {
            RightstrId = RightstrId&~TEXT_POINTER;
            GxGfx_Text(0, 0, 0, (const TCHAR*)RightstrId); //not really draw
            Rightsize = GxGfx_GetTextLastSize();
            debug_ind(("RightstrId  %x w %d\r\n",RightstrId,Rightsize.w));
        }

        if(RightRect.x1 > LeftRect.x1)
        {

            RightRect.x1 = LeftRect.x1;
            RightRect.x2 = RightRect.x1 + Rightsize.w;

            LeftRect.x1 = RightRect.x2 + INTVAL;
            LeftRect.x2 = LeftRect.x1 + LeftWidth;
        }
        else
        {
            debug_ind(("not change  right %d left %d\r\n",RightRect.x1,LeftRect.x1));
            RightRect.x2 = RightRect.x1 + Rightsize.w;

            LeftRect.x1 = RightRect.x2 + INTVAL;
            LeftRect.x2 = LeftRect.x1 + LeftWidth;
        }
        debug_ind(("^G LeftRect %d %d\r\n",LeftRect.x1,LeftRect.x2));
        debug_ind(("^G RightRect %d %d\r\n",RightRect.x1,RightRect.x2));

        UxCtrl_SetPos(pLeftStrCtrl,LeftRect);
        UxCtrl_SetPos(pRightStrCtrl,RightRect);
        #endif
        //#NT#2011/02/15#Janice Huang -end
    }
}
#endif

#endif
