//
//  tl_math_iterators.cpp
//  tlMath
//


#include "tl_math.h"



PF_Err
ShiftImage8 (
             void         *refcon,
             A_long         xL,
             A_long         yL,
             PF_Pixel     *inP,
             PF_Pixel     *outP)
{
    OffInfo    *oiP        = (OffInfo*)refcon;
    PF_Err                err            = PF_Err_NONE;
    PF_Fixed            new_xFi        = 0,
    new_yFi        = 0;

    // Resample original image at offset point.

    new_xFi = PF_Fixed (((A_long) xL<<16) + oiP->x_offFi);
    new_yFi = PF_Fixed (((A_long) yL<<16) + oiP->y_offFi);

    ERR(oiP->in_data.utils->subpixel_sample(oiP->in_data.effect_ref,
                                            new_xFi,
                                            new_yFi,
                                            &oiP->samp_pb,
                                            outP));

    return err;
}

PF_Err
ShiftImage16 (
              void         *refcon,
              A_long         xL,
              A_long         yL,
              PF_Pixel16     *inP,
              PF_Pixel16     *outP)
{
    OffInfo    *oiP        = (OffInfo*)refcon;
    PF_InData            *in_data    = &(oiP->in_data);
    PF_Err                err            = PF_Err_NONE;
    PF_Fixed            new_xFi         = 0,
    new_yFi         = 0;

    AEGP_SuiteHandler suites(in_data->pica_basicP);

    // Resample original image at offset point.

    new_xFi = PF_Fixed (((A_long) xL<<16) + oiP->x_offFi);
    new_yFi = PF_Fixed (((A_long) yL<<16) + oiP->y_offFi);

    ERR(suites.Sampling16Suite1()->subpixel_sample16(in_data->effect_ref,
                                                     new_xFi,
                                                     new_yFi,
                                                     &oiP->samp_pb,
                                                     outP));
    return err;
}


PF_Err
ShiftImage32 (
              void         *refcon,
              A_long         xL,
              A_long         yL,
              PF_Pixel32     *inP,
              PF_Pixel32     *outP)
{
    OffInfo    *oiP        = (OffInfo*)refcon;
    PF_InData            *in_data    = &(oiP->in_data);
    PF_Err                err            = PF_Err_NONE;
    PF_Fixed            new_xFi         = 0,
    new_yFi         = 0;

    AEGP_SuiteHandler suites(in_data->pica_basicP);

    // Resample original image at offset point.

    new_xFi = PF_Fixed (((A_long) xL<<16) + oiP->x_offFi);
    new_yFi = PF_Fixed (((A_long) yL<<16) + oiP->y_offFi);
    ERR(suites.SamplingFloatSuite1()->subpixel_sample_float(in_data->effect_ref,
                                                     new_xFi,
                                                     new_yFi,
                                                     &oiP->samp_pb,
                                                     outP));
    return err;
}

PF_Err
LineIteration8Func ( void *refconPV,
                    void *refconFunc,
                    void *refconFlags,
                    void *refconWorld,
                    A_long yL)
{
    PF_Err err = PF_Err_NONE;
    MathInfo           *miP    = reinterpret_cast<MathInfo*>(refconPV);
    funcTransfertInfo  *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
    FlagsInfoP         *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
    WorldTransfertInfo *wtiP = reinterpret_cast<WorldTransfertInfo*>(refconWorld);
    PF_EffectWorld inW = wtiP->inW;//cast input buffer here.
    PF_EffectWorld outW = wtiP->outW;//cast output buffer here.

    PF_EffectWorld extW;
    PF_Pixel8 *bop_extP;
    if (flagsP->PixelsCallExternalInputB){
        extW = wtiP->extLW;
        //external layer world
        bop_extP = reinterpret_cast<PF_Pixel8*>(extW.data) + (extW.rowbytes* yL / sizeof(PF_Pixel));
    }

    PF_FpShort  red_result, green_result, blue_result, alpha_result;
    PF_Pixel8 *bop_outP = reinterpret_cast<PF_Pixel8*>(outW.data)+ (outW.rowbytes* yL / sizeof(PF_Pixel)),
               *bop_inP = reinterpret_cast<PF_Pixel8*>(inW.data)+ (inW.rowbytes* yL / sizeof(PF_Pixel));
    AEFX_CLR_STRUCT(miP->pixF[1]);
    miP->pixF[1] = PF_FpShort(yL);

    for ( A_long xL =0; xL < inW.width; xL++) {
        if (flagsP->NeedsPixelAroundB) {
            PF_Pixel *in00;
            PF_Pixel *in10;
            PF_Pixel *in20;
            PF_Pixel *in01;
            PF_Pixel *in21;
            PF_Pixel *in02;
            PF_Pixel *in12;
            PF_Pixel *in22;

            in00 = bop_inP - (inW.rowbytes / sizeof(PF_Pixel)) - 1;//top left pixel in 3X3.
            in10 = in00 + 1;//top middle pixel in 3X3.
            in20 = in10 + 1;//top right pixel in 3X3.
            in01 = bop_inP - 1;//mid left pixel in 3X3.
            in21 = bop_inP + 1;//top right pixel in 3X3.
            in02 = bop_inP + (inW.rowbytes / sizeof(PF_Pixel)) - 1;//bottom left pixel in 3X3.
            in12 = in02 + 1;//bottom middle pixel in 3X3.
            in22 = in12 + 1;//bottom right pixel in 3X3.
            if (yL - 1 >= 0) {
                miP->m9P_red[0] = PF_FpShort(in00->red) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_green[0] = PF_FpShort(in00->green) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_blue[0] = PF_FpShort(in00->blue) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_alpha[0] = PF_FpShort(in00->alpha) / (PF_FpShort)PF_MAX_CHAN8;

                miP->m9P_red[1] = PF_FpShort(in10->red) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_green[1] = PF_FpShort(in10->green) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_blue[1] = PF_FpShort(in10->blue) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_alpha[1] = PF_FpShort(in10->alpha) / (PF_FpShort)PF_MAX_CHAN8;
                if (xL + 1 <= inW.width) {
                    miP->m9P_red[2] = PF_FpShort(in20->red) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_green[2] = PF_FpShort(in20->green) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_blue[2] = PF_FpShort(in20->blue) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_alpha[2] = PF_FpShort(in20->alpha) / (PF_FpShort)PF_MAX_CHAN8;
                }
                else {
                    miP->m9P_red[2] = 0;
                    miP->m9P_green[2] = 0;
                    miP->m9P_blue[2] = 0;
                    miP->m9P_alpha[2] = 0;
                }
            }
            miP->m9P_red[3] = PF_FpShort(in01->red) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_green[3] = PF_FpShort(in01->green) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_blue[3] = PF_FpShort(in01->blue) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_alpha[3] = PF_FpShort(in01->alpha) / (PF_FpShort)PF_MAX_CHAN8;

            miP->m9P_red[4] = PF_FpShort(bop_inP->red) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_green[4] = PF_FpShort(bop_inP->green) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_blue[4] = PF_FpShort(bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
            miP->m9P_alpha[4] = PF_FpShort(bop_inP->alpha) / (PF_FpShort)PF_MAX_CHAN8;
            if (xL + 1 <= inW.width) {
                miP->m9P_red[5] = PF_FpShort(in21->red) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_green[5] = PF_FpShort(in21->green) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_blue[5] = PF_FpShort(in21->blue) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_alpha[5] = PF_FpShort(in21->alpha) / (PF_FpShort)PF_MAX_CHAN8;
            }
            else {
                miP->m9P_red[5] = 0;
                miP->m9P_green[5] = 0;
                miP->m9P_blue[5] = 0;
                miP->m9P_alpha[5] = 0;
            }
            if (yL + 1 <= inW.height) {
                miP->m9P_red[6] = PF_FpShort(in02->red) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_green[6] = PF_FpShort(in02->green) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_blue[6] = PF_FpShort(in02->blue) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_alpha[6] = PF_FpShort(in02->alpha) / (PF_FpShort)PF_MAX_CHAN8;

                miP->m9P_red[7] = PF_FpShort(in12->red) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_green[7] = PF_FpShort(in12->green) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_blue[7] = PF_FpShort(in12->blue) / (PF_FpShort)PF_MAX_CHAN8;
                miP->m9P_alpha[7] = PF_FpShort(in12->alpha) / (PF_FpShort)PF_MAX_CHAN8;

                if (xL + 1 <= inW.width) {
                    miP->m9P_red[8] = PF_FpShort(in22->red) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_green[8] = PF_FpShort(in22->green) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_blue[8] = PF_FpShort(in22->blue) / (PF_FpShort)PF_MAX_CHAN8;
                    miP->m9P_alpha[8] = PF_FpShort(in22->alpha) / (PF_FpShort)PF_MAX_CHAN8;
                }
                else {
                    miP->m9P_red[8] = 0;
                    miP->m9P_green[8] = 0;
                    miP->m9P_blue[8] = 0;
                    miP->m9P_alpha[8] = 0;
                }
            }
        }
        AEFX_CLR_STRUCT(miP->pixF[0]);
        miP->pixF[0] = PF_FpShort(xL);
        AEFX_CLR_STRUCT(miP->inColorF[3]);
        miP->inColorF[3] = (PF_FpShort)bop_inP->alpha / PF_MAX_CHAN8;

        if (miP->inColorF[3] != 0) {
            if (flagsP->NeedsLumaB) {
                AEFX_CLR_STRUCT(miP->luma);
                miP->luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN8;
            }
            AEFX_CLR_STRUCT(miP->inColorF[0]);
            miP->inColorF[0] = (PF_FpShort)bop_inP->red / PF_MAX_CHAN8;
            AEFX_CLR_STRUCT(miP->inColorF[1]);
            miP->inColorF[1] = (PF_FpShort)bop_inP->green / PF_MAX_CHAN8;
            AEFX_CLR_STRUCT(miP->inColorF[2]);
            miP->inColorF[2] = (PF_FpShort)bop_inP->blue / PF_MAX_CHAN8;
            if (flagsP->PixelsCallExternalInputB) {
                AEFX_CLR_STRUCT(miP->extLayerColorF[0]);
                miP->extLayerColorF[0] = (PF_FpShort)bop_extP->red / PF_MAX_CHAN8;
                AEFX_CLR_STRUCT(miP->extLayerColorF[1]);
                miP->extLayerColorF[1] = (PF_FpShort)bop_extP->green / PF_MAX_CHAN8;
                AEFX_CLR_STRUCT(miP->extLayerColorF[2]);
                miP->extLayerColorF[2] = (PF_FpShort)bop_extP->blue / PF_MAX_CHAN8;
                AEFX_CLR_STRUCT(miP->extLayerColorF[3]);
                miP->extLayerColorF[3] = (PF_FpShort)bop_extP->alpha / PF_MAX_CHAN8;

            }

            AEFX_CLR_STRUCT(red_result);
            red_result = MIN(fiP->redExpr(), 1);
            AEFX_CLR_STRUCT(green_result);
            green_result = MIN(fiP->greenExpr(), 1);
            AEFX_CLR_STRUCT(blue_result);
            blue_result = MIN(fiP->blueExpr(), 1);
            AEFX_CLR_STRUCT(alpha_result);
            alpha_result = MIN(fiP->alphaExpr(), 1);

            bop_outP->alpha = A_u_char(alpha_result *PF_MAX_CHAN8);
            bop_outP->red = A_u_char(red_result *PF_MAX_CHAN8);
            bop_outP->green = A_u_char(green_result *PF_MAX_CHAN8);
            bop_outP->blue = A_u_char(blue_result *PF_MAX_CHAN8);

        }
        else {
            bop_outP->alpha = 0;
            bop_outP->red   = 0;
            bop_outP->green = 0;
            bop_outP->blue  = 0;
        }
        bop_outP++;
        bop_inP++;
        bop_extP++;
    }
    return err;
}
PF_Err
LineIteration16Func(void *refconPV,
                    void *refconFunc,
                    void *refconFlags,
                    void *refconWorld,
                    A_long yL)
{
    PF_Err err = PF_Err_NONE;
    MathInfo*   miP = reinterpret_cast<MathInfo*>(refconPV);
    funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
    FlagsInfoP *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
    WorldTransfertInfo *wtiP = reinterpret_cast<WorldTransfertInfo*>(refconWorld);
    PF_EffectWorld inW = wtiP->inW;//cast input buffer here.
    PF_EffectWorld outW = wtiP->outW;//cast output buffer here.

    PF_EffectWorld extW;
    PF_Pixel16 *bop_extP;
    if (flagsP->PixelsCallExternalInputB) {
        extW = wtiP->extLW;
        //external layer world
        bop_extP = reinterpret_cast<PF_Pixel16*>(extW.data) + (extW.rowbytes* yL / sizeof(PF_Pixel16));
    }
    PF_FpShort  red_result, green_result, blue_result, alpha_result;
    PF_Pixel16 *bop_outP = reinterpret_cast<PF_Pixel16*>(outW.data) + (outW.rowbytes* yL / sizeof(PF_Pixel16)),
    *bop_inP = reinterpret_cast<PF_Pixel16*>(inW.data) +  (inW.rowbytes* yL / sizeof(PF_Pixel16));
    AEFX_CLR_STRUCT(miP->pixF[1]);
    miP->pixF[1] = PF_FpShort(yL);
    for (A_long xL = 0; xL < inW.width; xL++) {
        if (flagsP->NeedsPixelAroundB) {
            PF_Pixel16 *in00;
            PF_Pixel16 *in10;
            PF_Pixel16 *in20;
            PF_Pixel16 *in01;
            PF_Pixel16 *in21;
            PF_Pixel16 *in02;
            PF_Pixel16 *in12;
            PF_Pixel16 *in22;

            in00 = bop_inP - (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//top left pixel in 3X3.
            in10 = in00 + 1;//top middle pixel in 3X3.
            in20 = in10 + 1;//top right pixel in 3X3.
            in01 = bop_inP - 1;//mid left pixel in 3X3.
            in21 = bop_inP + 1;//top right pixel in 3X3.
            in02 = bop_inP + (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//bottom left pixel in 3X3.
            in12 = in02 + 1;//bottom middle pixel in 3X3.
            in22 = in12 + 1;//bottom right pixel in 3X3.

            if (yL - 1 >= 0) {
                miP->m9P_red[0] = PF_FpShort(in00->red) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_green[0] = PF_FpShort(in00->green) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_blue[0] = PF_FpShort(in00->blue) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_alpha[0] = PF_FpShort(in00->alpha) / (PF_FpShort)PF_MAX_CHAN16;

                miP->m9P_red[1] = PF_FpShort(in10->red) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_green[1] = PF_FpShort(in10->green) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_blue[1] = PF_FpShort(in10->blue) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_alpha[1] = PF_FpShort(in10->alpha) / (PF_FpShort)PF_MAX_CHAN16;
                if (xL + 1 <= inW.width) {
                    miP->m9P_red[2] = PF_FpShort(in20->red) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_green[2] = PF_FpShort(in20->green) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_blue[2] = PF_FpShort(in20->blue) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_alpha[2] = PF_FpShort(in20->alpha) / (PF_FpShort)PF_MAX_CHAN16;
                }
                else {
                    miP->m9P_red[2] = 0;
                    miP->m9P_green[2] = 0;
                    miP->m9P_blue[2] = 0;
                    miP->m9P_alpha[2] = 0;
                }
            }
            miP->m9P_red[3] = PF_FpShort(in01->red) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_green[3] = PF_FpShort(in01->green) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_blue[3] = PF_FpShort(in01->blue) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_alpha[3] = PF_FpShort(in01->alpha) / (PF_FpShort)PF_MAX_CHAN16;

            miP->m9P_red[4] = PF_FpShort(bop_inP->red) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_green[4] = PF_FpShort(bop_inP->green) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_blue[4] = PF_FpShort(bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN16;
            miP->m9P_alpha[4] = PF_FpShort(bop_inP->alpha) / (PF_FpShort)PF_MAX_CHAN16;
            if (xL + 1 <= inW.width) {
                miP->m9P_red[5] = PF_FpShort(in21->red) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_green[5] = PF_FpShort(in21->green) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_blue[5] = PF_FpShort(in21->blue) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_alpha[5] = PF_FpShort(in21->alpha) / (PF_FpShort)PF_MAX_CHAN16;
            }
            else {
                miP->m9P_red[5] = 0;
                miP->m9P_green[5] = 0;
                miP->m9P_blue[5] = 0;
                miP->m9P_alpha[5] = 0;
            }
            if (yL + 1 <= inW.height) {
                miP->m9P_red[6] = PF_FpShort(in02->red) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_green[6] = PF_FpShort(in02->green) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_blue[6] = PF_FpShort(in02->blue) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_alpha[6] = PF_FpShort(in02->alpha) / (PF_FpShort)PF_MAX_CHAN16;

                miP->m9P_red[7] = PF_FpShort(in12->red) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_green[7] = PF_FpShort(in12->green) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_blue[7] = PF_FpShort(in12->blue) / (PF_FpShort)PF_MAX_CHAN16;
                miP->m9P_alpha[7] = PF_FpShort(in12->alpha) / (PF_FpShort)PF_MAX_CHAN16;

                if (xL + 1 <= inW.width) {
                    miP->m9P_red[8] = PF_FpShort(in22->red) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_green[8] = PF_FpShort(in22->green) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_blue[8] = PF_FpShort(in22->blue) / (PF_FpShort)PF_MAX_CHAN16;
                    miP->m9P_alpha[8] = PF_FpShort(in22->alpha) / (PF_FpShort)PF_MAX_CHAN16;
                }
                else {
                    miP->m9P_red[8] = 0;
                    miP->m9P_green[8] = 0;
                    miP->m9P_blue[8] = 0;
                    miP->m9P_alpha[8] = 0;
                }
            }
        }

        AEFX_CLR_STRUCT(miP->pixF[0]);
        miP->pixF[0] = PF_FpShort(xL);

        AEFX_CLR_STRUCT(miP->inColorF[3]);
        miP->inColorF[3] = (PF_FpShort)bop_inP->alpha / PF_MAX_CHAN16;


        if (miP->inColorF[3] != 0) {
            if (flagsP->NeedsLumaB) {
                AEFX_CLR_STRUCT(miP->luma);
                miP->luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue) / (PF_FpShort)PF_MAX_CHAN16;
            }

            AEFX_CLR_STRUCT(miP->inColorF[0]);
            miP->inColorF[0] = (PF_FpShort)bop_inP->red / PF_MAX_CHAN16;
            AEFX_CLR_STRUCT(miP->inColorF[1]);
            miP->inColorF[1] = (PF_FpShort)bop_inP->green / PF_MAX_CHAN16;
            AEFX_CLR_STRUCT(miP->inColorF[2]);
            miP->inColorF[2] = (PF_FpShort)bop_inP->blue / PF_MAX_CHAN16;
            if (flagsP->PixelsCallExternalInputB) {
                AEFX_CLR_STRUCT(miP->extLayerColorF[3]);
                miP->extLayerColorF[3] = (PF_FpShort)bop_extP->alpha / PF_MAX_CHAN16;
                AEFX_CLR_STRUCT(miP->extLayerColorF[0]);
                miP->extLayerColorF[0] = (PF_FpShort)bop_extP->red / PF_MAX_CHAN16;
                AEFX_CLR_STRUCT(miP->extLayerColorF[1]);
                miP->extLayerColorF[1] = (PF_FpShort)bop_extP->green / PF_MAX_CHAN16;
                AEFX_CLR_STRUCT(miP->extLayerColorF[2]);
                miP->extLayerColorF[2] = (PF_FpShort)bop_extP->blue / PF_MAX_CHAN16;
            }
            AEFX_CLR_STRUCT(red_result);
            red_result = MIN(fiP->redExpr(), 1);
            AEFX_CLR_STRUCT(green_result);
            green_result = MIN(fiP->greenExpr(), 1);
            AEFX_CLR_STRUCT(blue_result);
            blue_result = MIN(fiP->blueExpr(), 1);
            AEFX_CLR_STRUCT(alpha_result);
            alpha_result = MIN(fiP->alphaExpr(), 1);

            bop_outP->alpha = A_u_short(alpha_result *PF_MAX_CHAN16);
            bop_outP->red = A_u_short(red_result *PF_MAX_CHAN16);
            bop_outP->green = A_u_short(green_result *PF_MAX_CHAN16);
            bop_outP->blue = A_u_short(blue_result *PF_MAX_CHAN16);

        }
        else {
            bop_outP->alpha = 0;
            bop_outP->red = 0;
            bop_outP->green = 0;
            bop_outP->blue = 0;
        }

        bop_outP++;
        bop_inP++;
        bop_extP++;
    }
    return err;
}

PF_Err
LineIteration32Func(void *refconPV,
                    void *refconFunc,
                    void *refconFlags,
                    void *refconWorld,
	A_long yL)
{
	PF_Err err = PF_Err_NONE;
	MathInfo    *miP = reinterpret_cast<MathInfo*>(refconPV);
	funcTransfertInfo *fiP = reinterpret_cast<funcTransfertInfo*>(refconFunc);
	FlagsInfoP *flagsP = reinterpret_cast<FlagsInfo*>(refconFlags);
    WorldTransfertInfo *wtiP = reinterpret_cast<WorldTransfertInfo*>(refconWorld);
    PF_EffectWorld inW = wtiP->inW;//cast input buffer here.
    PF_EffectWorld outW = wtiP->outW;//cast output buffer here.
    
	PF_EffectWorld extW;
	PF_PixelFloat *bop_extP;
	if (flagsP->PixelsCallExternalInputB) {
		extW = wtiP->extLW;
		//external layer world
		bop_extP = reinterpret_cast<PF_PixelFloat*>(extW.data) + (extW.rowbytes* yL / sizeof(PF_PixelFloat));
	}
	PF_FpShort  red_result, green_result, blue_result, alpha_result;
	PF_PixelFloat *bop_outP = reinterpret_cast<PF_PixelFloat*>(outW.data) + (outW.rowbytes* yL / sizeof(PF_PixelFloat)),
                  *bop_inP = reinterpret_cast<PF_PixelFloat*>(inW.data) + (inW.rowbytes* yL / sizeof(PF_PixelFloat));
	AEFX_CLR_STRUCT(miP->pixF[1]);
	miP->pixF[1] = PF_FpShort(yL);
	for (A_long xL = 0; xL < inW.width; xL++) {
		if (flagsP->NeedsPixelAroundB) {
			PF_PixelFloat *in00;
			PF_PixelFloat *in10;
			PF_PixelFloat *in20;
			PF_PixelFloat *in01;
			PF_PixelFloat *in21;
			PF_PixelFloat *in02;
			PF_PixelFloat *in12;
			PF_PixelFloat *in22;

			in00 = bop_inP - (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//top left pixel in 3X3.
			in10 = in00 + 1;//top middle pixel in 3X3.
			in20 = in10 + 1;//top right pixel in 3X3.
			in01 = bop_inP - 1;//mid left pixel in 3X3.
			in21 = bop_inP + 1;//top right pixel in 3X3.
			in02 = bop_inP + (inW.rowbytes / sizeof(PF_Pixel16)) - 1;//bottom left pixel in 3X3.
			in12 = in02 + 1;//bottom middle pixel in 3X3.
			in22 = in12 + 1;//bottom right pixel in 3X3.

			if (yL - 1 >= 0) {
				miP->m9P_red[0] = PF_FpShort(in00->red) ;
				miP->m9P_green[0] = PF_FpShort(in00->green);
				miP->m9P_blue[0] = PF_FpShort(in00->blue);
				miP->m9P_alpha[0] = PF_FpShort(in00->alpha);


				miP->m9P_red[1] = PF_FpShort(in10->red);
				miP->m9P_green[1] = PF_FpShort(in10->green);
				miP->m9P_blue[1] = PF_FpShort(in10->blue);
				miP->m9P_alpha[1] = PF_FpShort(in10->alpha);
				if (xL + 1 <= inW.width) {
					miP->m9P_red[2] = PF_FpShort(in20->red);
					miP->m9P_green[2] = PF_FpShort(in20->green);
					miP->m9P_blue[2] = PF_FpShort(in20->blue);
					miP->m9P_alpha[2] = PF_FpShort(in20->alpha);
				}
				else {
					miP->m9P_red[2] = 0;
					miP->m9P_green[2] = 0;
					miP->m9P_blue[2] = 0;
					miP->m9P_alpha[2] = 0;
				}
			}
			miP->m9P_red[3] = PF_FpShort(in01->red);
			miP->m9P_green[3] = PF_FpShort(in01->green);
			miP->m9P_blue[3] = PF_FpShort(in01->blue);
			miP->m9P_alpha[3] = PF_FpShort(in01->alpha);

			miP->m9P_red[4] = PF_FpShort(bop_inP->red);
			miP->m9P_green[4] = PF_FpShort(bop_inP->green);
			miP->m9P_blue[4] = PF_FpShort(bop_inP->blue);
			miP->m9P_alpha[4] = PF_FpShort(bop_inP->alpha);
			if (xL + 1 <= inW.height) {
				miP->m9P_red[5] = PF_FpShort(in21->red);
				miP->m9P_green[5] = PF_FpShort(in21->green);
				miP->m9P_blue[5] = PF_FpShort(in21->blue);
				miP->m9P_alpha[5] = PF_FpShort(in21->alpha);
			}
			else {
				miP->m9P_red[5] = 0;
				miP->m9P_green[5] = 0;
				miP->m9P_blue[5] = 0;
				miP->m9P_alpha[5] = 0;
			}
			if (yL + 1 <= inW.height) {
				miP->m9P_red[6] = PF_FpShort(in02->red);
				miP->m9P_green[6] = PF_FpShort(in02->green);
				miP->m9P_blue[6] = PF_FpShort(in02->blue);
				miP->m9P_alpha[6] = PF_FpShort(in02->alpha);

				miP->m9P_red[7] = PF_FpShort(in12->red);
				miP->m9P_green[7] = PF_FpShort(in12->green);
				miP->m9P_blue[7] = PF_FpShort(in12->blue);
				miP->m9P_alpha[7] = PF_FpShort(in12->alpha);

				if (xL + 1 <= inW.width) {
					miP->m9P_red[8] = PF_FpShort(in22->red);
					miP->m9P_green[8] = PF_FpShort(in22->green);
					miP->m9P_blue[8] = PF_FpShort(in22->blue);
					miP->m9P_alpha[8] = PF_FpShort(in22->alpha);
				}
				else {
					miP->m9P_red[8] = 0;
					miP->m9P_green[8] = 0;
					miP->m9P_blue[8] = 0;
					miP->m9P_alpha[8] = 0;
				}
			}
		}

		AEFX_CLR_STRUCT(miP->pixF[0]);
		miP->pixF[0] = PF_FpShort(xL);

		AEFX_CLR_STRUCT(miP->inColorF[3]);
		miP->inColorF[3] = (PF_FpShort)bop_inP->alpha;


		if (miP->inColorF[3] != 0) {
			if (flagsP->NeedsLumaB) {
				AEFX_CLR_STRUCT(miP->luma);
				miP->luma = (0.2126*bop_inP->red + 0.7152*bop_inP->green + 0.0722*bop_inP->blue);
			}

			AEFX_CLR_STRUCT(miP->inColorF[0]);
			miP->inColorF[0] = (PF_FpShort)bop_inP->red;
			AEFX_CLR_STRUCT(miP->inColorF[1]);
			miP->inColorF[1] = (PF_FpShort)bop_inP->green;
			AEFX_CLR_STRUCT(miP->inColorF[2]);
			miP->inColorF[2] = (PF_FpShort)bop_inP->blue;
			if (flagsP->PixelsCallExternalInputB) {
				AEFX_CLR_STRUCT(miP->extLayerColorF[3]);
				miP->extLayerColorF[3] = (PF_FpShort)bop_extP->alpha;
				AEFX_CLR_STRUCT(miP->extLayerColorF[0]);
				miP->extLayerColorF[0] = (PF_FpShort)bop_extP->red;
				AEFX_CLR_STRUCT(miP->extLayerColorF[1]);
				miP->extLayerColorF[1] = (PF_FpShort)bop_extP->green;
				AEFX_CLR_STRUCT(miP->extLayerColorF[2]);
				miP->extLayerColorF[2] = (PF_FpShort)bop_extP->blue;
			}
			AEFX_CLR_STRUCT(red_result);
			red_result = MIN(fiP->redExpr(), 1);
			AEFX_CLR_STRUCT(green_result);
			green_result = MIN(fiP->greenExpr(), 1);
			AEFX_CLR_STRUCT(blue_result);
			blue_result = MIN(fiP->blueExpr(), 1);
			AEFX_CLR_STRUCT(alpha_result);
			alpha_result = MIN(fiP->alphaExpr(), 1);

			bop_outP->alpha =alpha_result;
			bop_outP->red = red_result;
			bop_outP->green = green_result;
			bop_outP->blue = blue_result;

		}
		else {
			bop_outP->alpha = 0;
			bop_outP->red = 0;
			bop_outP->green = 0;
			bop_outP->blue = 0;
		}

		bop_outP++;
		bop_inP++;
		bop_extP++;
	}
	return err;
}

void threaded_render::render_8(void *refconPV, void *refconFunc, void *refconFlags, void *refconWorld, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter)
{
    curNumIter =numIter;
    if ( thread_idxL == (numThreads-1)){
        curNumIter =lastNumIter;
    }

    std::lock_guard<std::mutex> guard(mut);
    for (A_long iterIndex = 0; iterIndex < curNumIter; ++iterIndex)
    {
        A_long yL =  thread_idxL*numIter+iterIndex;
        LineIteration8Func(refconPV, refconFunc, refconFlags,refconWorld, yL);
    }
}

void threaded_render::render_16(void *refconPV, void *refconFunc, void *refconFlags,void *refconWorld, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter)
{
    curNumIter =numIter;
    if ( thread_idxL == (numThreads-1)){
        curNumIter =lastNumIter;
    }
    std::lock_guard<std::mutex> guard(mut);
    for (A_long iterIndex = 0; iterIndex < curNumIter; ++iterIndex)
    {
        A_long yL =  thread_idxL*numIter+iterIndex;
        LineIteration16Func(refconPV,refconFunc, refconFlags,refconWorld, yL);
    }
}


void threaded_render::render_32(void *refconPV, void *refconFunc, void *refconFlags,void *refconWorld, A_long thread_idxL, A_long numThreads, A_long numIter, A_long lastNumIter)
{
	curNumIter = numIter;
	if (thread_idxL == (numThreads - 1)) {
		curNumIter = lastNumIter;
	}
	std::lock_guard<std::mutex> guard(mut);
	for (A_long iterIndex = 0; iterIndex < curNumIter; ++iterIndex)
	{
		A_long yL = thread_idxL * numIter + iterIndex;
		LineIteration32Func(refconPV, refconFunc, refconFlags,refconWorld, yL);
	}
}
