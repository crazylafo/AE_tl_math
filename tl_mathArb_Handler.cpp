/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include	"tl_math.h"

PF_Err	
CreateDefaultArb(	
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ArbitraryH		*dephault)
{
	PF_Err			err		= PF_Err_NONE;
	PF_Handle		arbH	= NULL;
	
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	
	arbH = suites.HandleSuite1()->host_new_handle(sizeof(m_ArbData));

	if (arbH) {
		m_ArbData	*arbP = reinterpret_cast<m_ArbData*>(PF_LOCK_HANDLE(arbH));
		if (!arbP) {
			err = PF_Err_OUT_OF_MEMORY;
		} else {
			AEFX_CLR_STRUCT(*arbP);

			arbP->redExAcP = STR(StrID_Default_expr);
            arbP->greenExAcP; STR(StrID_Default_expr);
            arbP->blueExAcP = STR(StrID_Default_expr);
            arbP->alphaExAcP = STR(StrID_Default_expr);

			*dephault = arbH;
		}
		suites.HandleSuite1()->host_unlock_handle(arbH);
	}
	return err;
}

PF_Err
Arb_Copy(	
	PF_InData				*in_data,
	PF_OutData				*out_data,
	const PF_ArbitraryH		*srcP,
	PF_ArbitraryH			*dstP)
{
	PF_Err err = PF_Err_NONE;

	PF_Handle	sourceH = *srcP;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);

	if (sourceH) {
		m_ArbData	*src_arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(sourceH));
		if (!src_arbP) {
			err = PF_Err_OUT_OF_MEMORY;
		} else {	
			PF_Handle	destH = *dstP;
			if (destH) {
				m_ArbData	*dst_arbP = reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(destH));
				if (!dst_arbP) {
					err = PF_Err_OUT_OF_MEMORY;
				} else 	{
					memcpy(dst_arbP,src_arbP,sizeof(m_ArbData));
					suites.HandleSuite1()->host_unlock_handle(destH);
				}
			}
			suites.HandleSuite1()->host_unlock_handle(sourceH);
		}	
	}
	return err;
}


PF_Err
Arb_Interpolate(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	double					intrp_amtF,
	const PF_ArbitraryH		*l_arbPH,
	const PF_ArbitraryH		*r_arbPH,
	PF_ArbitraryH			*intrp_arbP)
{
	PF_Err			err			= PF_Err_NONE;

	m_ArbData		*int_arbP	= NULL,
					*l_arbP		= NULL,
					*r_arbP		= NULL;

    std::string 	*headP		= NULL,
					*lpixP		= NULL,
					*rpixP		= NULL;


	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	int_arbP	= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*intrp_arbP));
	l_arbP		= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*l_arbPH));
	r_arbP		= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*r_arbPH));

	headP	= reinterpret_cast<std::string*>(int_arbP);
	lpixP	= reinterpret_cast<std::string*>(l_arbP);
	rpixP	= reinterpret_cast<std::string*>(r_arbP);


	suites.HandleSuite1()->host_unlock_handle(*intrp_arbP);
	suites.HandleSuite1()->host_unlock_handle(*l_arbPH);
	suites.HandleSuite1()->host_unlock_handle(*r_arbPH);
	return err;
}

PF_Err
Arb_Compare(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	const PF_ArbitraryH		*a_arbP,
	const PF_ArbitraryH		*b_arbP,
	PF_ArbCompareResult		*resultP)
{
	PF_Err err = PF_Err_NONE;
	
	PF_Handle	a_handle = *a_arbP,
				b_handle = *b_arbP;

	PF_FpShort		total_a_rL	= 0,
					total_a_gL	= 0,
					total_a_bL	= 0,
                    total_a_aL	= 0,
					total_aL	= 0,
				
					total_b_rL	= 0,
					total_b_gL	= 0,
					total_b_bL	= 0,
                    total_b_aL	= 0,
					total_bL	= 0;

	*resultP = PF_ArbCompare_EQUAL;

	if (a_handle && b_handle) {
		m_ArbData	*first_arbP;
		m_ArbData  *second_arbP;
				
		first_arbP = (m_ArbData*)PF_LOCK_HANDLE(a_handle);
		second_arbP = (m_ArbData*)PF_LOCK_HANDLE(b_handle);

		if (!first_arbP || !second_arbP) {
			err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
		} else {
            total_a_rL	= first_arbP->redExAcP.length(),
            total_a_gL	= first_arbP->greenExAcP.length(),
            total_a_bL	= first_arbP->blueExAcP.length(),
            total_a_aL	= first_arbP->alphaExAcP.length(),
            
            total_b_rL	= second_arbP->redExAcP.length(),
            total_b_gL	= second_arbP->greenExAcP.length(),
            total_b_bL	= second_arbP->blueExAcP.length(),
            total_b_aL	= second_arbP->alphaExAcP.length(),

            
			total_aL = total_a_rL + total_a_gL + total_a_bL;
			total_bL = total_b_rL + total_b_gL + total_b_bL; 

			if(total_aL > total_bL)	{
				*resultP = PF_ArbCompare_MORE;
			} else if(total_aL < total_bL){
				*resultP = PF_ArbCompare_LESS;
			} else {
				*resultP = PF_ArbCompare_EQUAL;
			}
			PF_UNLOCK_HANDLE(a_handle);
			PF_UNLOCK_HANDLE(b_handle);
		}
	}
	return err;
}


PF_Err
Arb_Scan(
         PF_InData			*in_data,
         PF_OutData			*out_data,
         void 				*refconPV,
         const char			*bufPC,
         unsigned long		bytes_to_scanLu,
         PF_ArbitraryH		*arbPH)
{
    PF_Err err = PF_Err_NONE;

    return err;
}


PF_Err
Arb_Print_Size()
{
	// This size is actually provided directly in ColorGrid.cpp,
	// in response to PF_Arbitrary_PRINT_FUNC
	PF_Err err = PF_Err_NONE;
	return err;
}


PF_Err
Arb_Print(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_ArbPrintFlags	print_flags,
	PF_ArbitraryH		arbH,
	A_u_long			print_sizeLu,
	A_char				*print_bufferPC)
{
	PF_Err		err			= PF_Err_NONE;
	PF_Handle	a_handle	= arbH;
	if (a_handle) {
		m_ArbData	*a_arbP;
				
		a_arbP = (m_ArbData*)PF_LOCK_HANDLE(a_handle);

		if (!a_arbP) {
			err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
		}
        PF_UNLOCK_HANDLE(a_handle);
		}
	if (err == AEFX_ParseError_APPEND_ERROR) {
		err = PF_Err_OUT_OF_MEMORY; // increase COLORGRID_ARB_MAX_PRINT_SIZE
	}
	return err;
}


