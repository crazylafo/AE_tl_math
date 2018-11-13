
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

            #ifdef AE_OS_WIN
			#pragma warning (disable : 4305)
                strncpy_s(arbP->redExAc, STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->greenExAc, STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->blueExAc, STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->alphaExAc, STR(StrID_Default_expr), 4096);
            
                strncpy_s(arbP->redExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->greenExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->blueExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy_s(arbP->alphaExAcFlat ,STR(StrID_Default_expr), 4096);
			#pragma warning (pop)
            #else
                strncpy(arbP->redExAc ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->greenExAc ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->blueExAc ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->alphaExAc ,STR(StrID_Default_expr), 4096);

                strncpy(arbP->redExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->greenExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->blueExAcFlat ,STR(StrID_Default_expr), 4096);
                strncpy(arbP->alphaExAcFlat ,STR(StrID_Default_expr), 4096);
#endif
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
					#ifdef AE_OS_WIN
					memcpy_s(dst_arbP,  sizeof(m_ArbData), src_arbP, sizeof(m_ArbData));
					#else
					memcpy(dst_arbP,src_arbP,sizeof(m_ArbData));
					#endif
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

    A_char			*headP		= NULL,
					*lpixP		= NULL,
					*rpixP		= NULL;


	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	int_arbP	= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*intrp_arbP));
	l_arbP		= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*l_arbPH));
	r_arbP		= reinterpret_cast<m_ArbData*>(suites.HandleSuite1()->host_lock_handle(*r_arbPH));

	headP	= reinterpret_cast<A_char*>(int_arbP);
	lpixP	= reinterpret_cast<A_char*>(l_arbP);
	rpixP	= reinterpret_cast<A_char*>(r_arbP);


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

	size_t          total_a_rL	= 0,
					total_a_gL	= 0,
					total_a_bL	= 0,
                    total_a_aL	= 0,
                    total_a_rfL	= 0,
                    total_a_gfL	= 0,
                    total_a_bfL	= 0,
                    total_a_afL	= 0,
                    total_aL	= 0,
				
					total_b_rL	= 0,
					total_b_gL	= 0,
					total_b_bL	= 0,
                    total_b_aL	= 0,
                    total_b_rfL	= 0,
                    total_b_gfL	= 0,
                    total_b_bfL	= 0,
                    total_b_afL	= 0,
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
            *resultP = PF_ArbCompare_EQUAL;


            total_a_rL	= strlen(first_arbP->redExAc);
            total_a_gL	= strlen(first_arbP->greenExAc);
            total_a_bL	= strlen(first_arbP->blueExAc);
            total_a_aL	= strlen(first_arbP->alphaExAc);
            
            
            total_a_rfL	= strlen(first_arbP->redExAcFlat);
            total_a_gfL	= strlen(first_arbP->greenExAcFlat);
            total_a_bfL	= strlen(first_arbP->blueExAcFlat);
            total_a_afL	= strlen(first_arbP->alphaExAcFlat);
            
            total_b_rL	= strlen(second_arbP->redExAc);
            total_b_gL	= strlen(second_arbP->greenExAc);
            total_b_bL	= strlen(second_arbP->blueExAc);
            total_b_aL	= strlen(second_arbP->alphaExAc);
            
            total_b_rfL	= strlen(second_arbP->redExAcFlat);
            total_b_gfL	= strlen(second_arbP->greenExAcFlat);
            total_b_bfL	= strlen(second_arbP->blueExAcFlat);
            total_b_afL	= strlen(second_arbP->alphaExAcFlat);
            

			total_aL = total_a_rL + total_a_gL + total_a_bL + total_a_aL +total_a_rfL + total_a_gfL + total_a_bfL+ total_a_afL  ;
			total_bL = total_b_rL + total_b_gL + total_b_bL + total_b_aL +total_b_rfL + total_b_gfL + total_b_bfL+ total_b_afL ;

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


PF_Err
AEFX_AppendText(
	A_char					*srcAC,				/* >> */
	const A_u_long			dest_sizeLu,		/* >> */
	A_char					*destAC,			/* <> */
	A_u_long				*current_indexPLu)	/* <> */
{
	PF_Err			err = PF_Err_NONE;

	A_u_long		new_strlenLu = strlen(srcAC) + *current_indexPLu;


	if (new_strlenLu <= dest_sizeLu) {
		destAC[*current_indexPLu] = 0x00;

		#ifdef AE_OS_WIN
		strncat_s(destAC, dest_sizeLu, srcAC, strlen(srcAC));
		#else
		strncat(destAC, srcAC, strlen(srcAC));
		#endif
		*current_indexPLu = new_strlenLu;
	}
	else {
		err = AEFX_ParseError_APPEND_ERROR;
	}

	return err;
}
