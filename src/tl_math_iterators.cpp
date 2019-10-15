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
