
#include	"tl_math.h"

PF_Err
tlmath::HandleArbitrary(
                PF_InData            *in_data,
                PF_OutData            *out_data,
                PF_ParamDef            *params[],
                PF_LayerDef            *output,
                PF_ArbParamsExtra    *extra)
{
    PF_Err     err     = PF_Err_NONE;
    void     *srcP    = NULL,
    *dstP    = NULL;

    switch (extra->which_function) {

        case PF_Arbitrary_NEW_FUNC:
            if (extra->u.new_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                err = tlmath::CreateDefaultArb(    in_data,
                                       out_data,
                                       extra->u.new_func_params.arbPH);
            }
            break;

        case PF_Arbitrary_DISPOSE_FUNC:
            if (extra->u.dispose_func_params.refconPV != ARB_REFCON) {
                err = PF_Err_INTERNAL_STRUCT_DAMAGED;
            } else {
                PF_DISPOSE_HANDLE(extra->u.dispose_func_params.arbH);
            }
            break;

        case PF_Arbitrary_COPY_FUNC:
            if(extra->u.copy_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(    in_data,
                                     out_data,
                                     extra->u.copy_func_params.dst_arbPH));

                ERR(tlmath::Arb_Copy(in_data,
                             out_data,
                             &extra->u.copy_func_params.src_arbH,
                             extra->u.copy_func_params.dst_arbPH));
            }
            break;

        case PF_Arbitrary_FLAT_SIZE_FUNC:
            *(extra->u.flat_size_func_params.flat_data_sizePLu) = sizeof(m_ArbData);
            break;

        case PF_Arbitrary_FLATTEN_FUNC:

            if(extra->u.flatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                srcP = (m_ArbData*)PF_LOCK_HANDLE(extra->u.flatten_func_params.arbH);
                dstP = extra->u.flatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                PF_UNLOCK_HANDLE(extra->u.flatten_func_params.arbH);
            }
            break;

        case PF_Arbitrary_UNFLATTEN_FUNC:
            if(extra->u.unflatten_func_params.buf_sizeLu == sizeof(m_ArbData)){
                PF_Handle    handle = PF_NEW_HANDLE(sizeof(m_ArbData));
                dstP = (m_ArbData*)PF_LOCK_HANDLE(handle);
                srcP = (void*)extra->u.unflatten_func_params.flat_dataPV;
                if (srcP){
                    memcpy(dstP,srcP,sizeof(m_ArbData));
                }
                *(extra->u.unflatten_func_params.arbPH) = handle;
                PF_UNLOCK_HANDLE(handle);
            }
            break;

        case PF_Arbitrary_INTERP_FUNC:
            if(extra->u.interp_func_params.refconPV == ARB_REFCON) {
                ERR(CreateDefaultArb(    in_data,
                                     out_data,
                                     extra->u.interp_func_params.interpPH));

                ERR(Arb_Interpolate(    in_data,
                                    out_data,
                                    extra->u.interp_func_params.tF,
                                    &extra->u.interp_func_params.left_arbH,
                                    &extra->u.interp_func_params.right_arbH,
                                    extra->u.interp_func_params.interpPH));
            }
            break;

        case PF_Arbitrary_COMPARE_FUNC:
            ERR(tlmath::Arb_Compare(    in_data,
                            out_data,
                            &extra->u.compare_func_params.a_arbH,
                            &extra->u.compare_func_params.b_arbH,
                            extra->u.compare_func_params.compareP));
            break;


        case PF_Arbitrary_PRINT_SIZE_FUNC:
            err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            break;
        case PF_Arbitrary_PRINT_FUNC:

            if (extra->u.print_func_params.refconPV == ARB_REFCON) {
                ERR(Arb_Print(in_data,
                              out_data,
                              extra->u.print_func_params.print_flags,
                              extra->u.print_func_params.arbH,
                              extra->u.print_func_params.print_sizeLu,
                              extra->u.print_func_params.print_bufferPC));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;

        case PF_Arbitrary_SCAN_FUNC:
            if (extra->u.scan_func_params.refconPV == ARB_REFCON) {
                ERR(tlmath::Arb_Scan(    in_data,
                             out_data,
                             extra->u.scan_func_params.refconPV,
                             extra->u.scan_func_params.bufPC,
                             extra->u.scan_func_params.bytes_to_scanLu,
                             extra->u.scan_func_params.arbPH));
            } else {
                err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
            }
            break;
    }
    return err;
}


PF_Err	
tlmath::CreateDefaultArb(
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
                strncpy_s(arbP->arbDataAc,  defaultArb.c_str(), 100000);
			#pragma warning (pop)
            #else
                strncpy(arbP->arbDataAc,  defaultArb.c_str(), 100000);
            #endif
			arbP->hasChangedB = false;


			*dephault = arbH;
		}
		suites.HandleSuite1()->host_unlock_handle(arbH);
	}
	return err;
}

PF_Err
tlmath::Arb_Copy(
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
tlmath::Arb_Interpolate(
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
tlmath::Arb_Compare(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	const PF_ArbitraryH		*a_arbP,
	const PF_ArbitraryH		*b_arbP,
	PF_ArbCompareResult		*resultP)
{
	PF_Err err = PF_Err_NONE;
	
	PF_Handle	a_handle = *a_arbP,
				b_handle = *b_arbP;

	size_t          total_aL	= 0,
                    total_bL	= 0;
	std::vector<size_t> total_aV, total_bV;


	*resultP = PF_ArbCompare_EQUAL;

	if (a_handle && b_handle) {
		m_ArbData	*first_arbP;
		m_ArbData  *second_arbP;
				
		first_arbP = (m_ArbData*)PF_LOCK_HANDLE(a_handle);
		second_arbP = (m_ArbData*)PF_LOCK_HANDLE(b_handle);

		if (!first_arbP || !second_arbP) {
			err = PF_Err_UNRECOGNIZED_PARAM_TYPE;
		}
		else {
			*resultP = PF_ArbCompare_EQUAL;
            total_aV.emplace_back (strlen(first_arbP->arbDataAc));
			total_aV.emplace_back(first_arbP->hasChangedB);
            total_bV.emplace_back (strlen(second_arbP->arbDataAc));
			total_bV.emplace_back(second_arbP->hasChangedB);
			total_aL = std::accumulate(total_aV.begin(), total_aV.end(), (size_t)0);
			total_bL =std::accumulate(total_bV.begin(), total_bV.end(), (size_t)0);
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
tlmath::Arb_Scan(
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
tlmath::Arb_Print_Size()
{
	// This size is actually provided directly in ColorGrid.cpp,
	// in response to PF_Arbitrary_PRINT_FUNC
	PF_Err err = PF_Err_NONE;
	return err;
}
PF_Err
tlmath::Arb_Print(
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
tlmath::AEFX_AppendText(
	A_char					*srcAC,				/* >> */
	const A_u_long			dest_sizeLu,		/* >> */
	A_char					*destAC,			/* <> */
	A_u_long				*current_indexPLu)	/* <> */
{
	PF_Err			err = PF_Err_NONE;

	A_u_long		new_strlenLu = A_u_long (strlen(srcAC)) + *current_indexPLu;


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

