
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
                strncpy_s(arbP->redExAc, "in_red", 4096);
                strncpy_s(arbP->greenExAc, "in_green", 4096);
                strncpy_s(arbP->blueExAc, "in_blue", 4096);
                strncpy_s(arbP->alphaExAc, "1", 4096);
                strncpy_s(arbP->presetNameAc,"Preset Name", 32);
                strncpy_s(arbP->descriptionAc,"Describe your preset here", 2048);
                strncpy_s(arbP->Glsl_FragmentShAc, "fragSh", 4096);
				strncpy_s(arbP->Glsl_VertexShAc, "vertexSh", 4096);
				strncpy_s(arbP->uiSliderGrp_NameAC, "Slider Group", 32);
                strncpy_s(arbP->uiSliderOne_NameAC,"variable 1", 32);
                strncpy_s(arbP->uiSliderTwo_NameAC,"variable 2", 32);
                strncpy_s(arbP->uiSliderThree_NameAC,"variable 3", 32);
                strncpy_s(arbP->uiSliderFour_NameAC,"variable 4", 32);
				strncpy_s(arbP->uiPointGrp_NameAC, "Point Group", 32);
                strncpy_s(arbP->uiPointOne_NameAC,"Point 1", 32);
                strncpy_s(arbP->uiPointTwo_NameAC,"Point 2", 32);
				strncpy_s(arbP->uiColorGrp_NameAC, "Color Group", 32);
                strncpy_s(arbP->uiColorOne_NameAC,"Color 1", 32);
                strncpy_s(arbP->uiColorTwo_NameAC,"Color 2", 32);
				strncpy_s(arbP->uiExtLGrp_NameAC, "external Layer", 32);
                strncpy_s(arbP->redExAcFlat , "in_red", 4096);
                strncpy_s(arbP->greenExAcFlat ,"in_green", 4096);
                strncpy_s(arbP->blueExAcFlat ,"in_blue", 4096);
                strncpy_s(arbP->alphaExAcFlat , "1", 4096);
                strncpy_s(arbP->descriptionAcFlat,"Describe your preset here", 2048);
                strncpy_s(arbP->Glsl_FragmentShFlat, "fragSh", 4096);
				strncpy_s(arbP->Glsl_VertexShFlat, "vertexSh", 4096);

         
            
			#pragma warning (pop)
            #else
                strncpy(arbP->redExAc, "in_red", 4096);
                strncpy(arbP->greenExAc, "in_green", 4096);
                strncpy(arbP->blueExAc, "in_blue", 4096);
                strncpy(arbP->alphaExAc, "1", 4096);
                strncpy(arbP->presetNameAc,"Preset Name", 32);
                strncpy(arbP->descriptionAc,"Describe your preset here", 2048);
                strncpy(arbP->Glsl_FragmentShAc, "fragSh", 4096);
				strncpy(arbP->Glsl_VertexShAc, "vertexSh", 4096);
				strncpy(arbP->uiSliderGrp_NameAC, "Slider Group", 32);
                strncpy(arbP->uiSliderOne_NameAC,"variable 1", 32);
                strncpy(arbP->uiSliderTwo_NameAC,"variable 2", 32);
                strncpy(arbP->uiSliderThree_NameAC,"variable 3", 32);
                strncpy(arbP->uiSliderFour_NameAC,"variable 4", 32);
				strncpy(arbP->uiPointGrp_NameAC, "Point Group", 32);
                strncpy(arbP->uiPointOne_NameAC,"Point 1", 32);
                strncpy(arbP->uiPointTwo_NameAC,"Point 2", 32);
				strncpy(arbP->uiColorGrp_NameAC, "Color Group", 32);
                strncpy(arbP->uiColorOne_NameAC,"Color 1", 32);
                strncpy(arbP->uiColorTwo_NameAC,"Color 2", 32);
				strncpy(arbP->uiExtLGrp_NameAC, "external Layer", 32);
                strncpy(arbP->redExAcFlat , "in_red", 4096);
                strncpy(arbP->greenExAcFlat ,"in_green", 4096);
                strncpy(arbP->blueExAcFlat ,"in_blue", 4096);
                strncpy(arbP->alphaExAcFlat , "1", 4096);
                strncpy(arbP->descriptionAcFlat,"Describe your preset here", 2048);
                strncpy(arbP->Glsl_FragmentShFlat, "fragSh", 4096);
				strncpy(arbP->Glsl_VertexShFlat, "vertexSh", 4096);
            #endif
                arbP->parserModeB = false;
				PF_Boolean ShaderResetB = false;
				PF_Boolean ExprResetB = false;
				arbP->NeedsPixelAroundB = false;
				arbP->PixelsCallExternalInputB = false;
				arbP->NeedsLumaB = false;
				arbP->PresetHasWideInputB = false;
                arbP->CallsAEGP_CompB= false;
                arbP->CallsAEGP_layerB= false;
				arbP->ShaderResetB = false;
				arbP->uiSliderGrpB = true;
				arbP->uiSliderOneB = true;
				arbP->uiSliderTwoB = true;
				arbP->uiSliderThreeB = true;
				arbP->uiSliderFourB = true;
				arbP->uiPointGrpB = true;
				arbP->uiPointOneB = true;
				arbP->uiPointTwoB = true;
				arbP->uiColorGrpB = true;
				arbP->uiColorOneB = true;
				arbP->uiColorTwoB = true;
				arbP->uiExtLGrpB = true;

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


			total_aV.emplace_back (strlen(first_arbP->redExAc));
			total_aV.emplace_back (strlen(first_arbP->greenExAc));
			total_aV.emplace_back (strlen(first_arbP->blueExAc));
			total_aV.emplace_back (strlen(first_arbP->alphaExAc));
			total_aV.emplace_back (strlen(first_arbP->presetNameAc));
			total_aV.emplace_back (strlen(first_arbP->descriptionAc));
            
			total_aV.emplace_back (strlen(first_arbP->redExAcFlat));
			total_aV.emplace_back (strlen(first_arbP->greenExAcFlat));
			total_aV.emplace_back (strlen(first_arbP->blueExAcFlat));
			total_aV.emplace_back (strlen(first_arbP->alphaExAcFlat));
			total_aV.emplace_back (strlen(first_arbP->descriptionAcFlat));

			total_aV.emplace_back (first_arbP->NeedsPixelAroundB);
			total_aV.emplace_back (first_arbP->PixelsCallExternalInputB);
			total_aV.emplace_back (first_arbP->NeedsLumaB);
			total_aV.emplace_back (first_arbP->PresetHasWideInputB);
            
            total_aV.emplace_back ((first_arbP->parserModeB));
            total_aV.emplace_back ( strlen(first_arbP->Glsl_FragmentShAc));
			total_aV.emplace_back( strlen(first_arbP->Glsl_VertexShAc));
			total_aV.emplace_back(strlen(first_arbP->uiSliderGrp_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiSliderOne_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiSliderTwo_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiSliderThree_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiSliderFour_NameAC));
			total_aV.emplace_back(strlen(first_arbP->uiPointGrp_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiPointOne_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiPointTwo_NameAC));
			total_aV.emplace_back(strlen(first_arbP->uiColorGrp_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiColorOne_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->uiColorTwo_NameAC));
			total_aV.emplace_back ( strlen(first_arbP->uiExtLGrp_NameAC));
            total_aV.emplace_back ( strlen(first_arbP->Glsl_FragmentShFlat));
			total_aV.emplace_back(strlen(first_arbP->Glsl_VertexShFlat));

            total_aV.emplace_back (first_arbP->CallsAEGP_CompB);
            total_aV.emplace_back (first_arbP->CallsAEGP_layerB);
			total_aV.emplace_back(first_arbP->ShaderResetB);
			total_aV.emplace_back(first_arbP->ExprResetB);
			total_aV.emplace_back(first_arbP->uiSliderGrpB );
			total_aV.emplace_back(first_arbP->uiSliderOneB );
			total_aV.emplace_back(first_arbP->uiSliderTwoB );
			total_aV.emplace_back(first_arbP->uiSliderThreeB );
			total_aV.emplace_back(first_arbP->uiSliderFourB );
			total_aV.emplace_back(first_arbP->uiPointGrpB );
			total_aV.emplace_back(first_arbP->uiPointOneB );
			total_aV.emplace_back(first_arbP->uiPointTwoB );
			total_aV.emplace_back(first_arbP->uiColorGrpB );
			total_aV.emplace_back(first_arbP->uiColorOneB );
			total_aV.emplace_back(first_arbP->uiColorTwoB );
			total_aV.emplace_back(first_arbP->uiExtLGrpB );
            
			total_bV.emplace_back ( strlen(second_arbP->redExAc));
			total_bV.emplace_back ( strlen(second_arbP->greenExAc));
			total_bV.emplace_back ( strlen(second_arbP->blueExAc));
			total_bV.emplace_back ( strlen(second_arbP->alphaExAc));
			total_bV.emplace_back ( strlen(second_arbP->presetNameAc));
			total_bV.emplace_back (strlen(second_arbP->redExAcFlat));
			total_bV.emplace_back (strlen(second_arbP->greenExAcFlat));
			total_bV.emplace_back (strlen(second_arbP->blueExAcFlat));
			total_bV.emplace_back ( strlen(second_arbP->alphaExAcFlat));
			total_bV.emplace_back (strlen(second_arbP->descriptionAcFlat));

			total_bV.emplace_back (  second_arbP->NeedsPixelAroundB);
            total_bV.emplace_back (second_arbP->PixelsCallExternalInputB);
			total_bV.emplace_back ( second_arbP->NeedsLumaB);
			total_bV.emplace_back ( second_arbP->PresetHasWideInputB);
            
            total_bV.emplace_back ((second_arbP->parserModeB));
            total_bV.emplace_back ( strlen(second_arbP->Glsl_FragmentShAc));
			total_bV.emplace_back(strlen(second_arbP->Glsl_VertexShAc));
			total_bV.emplace_back(strlen(second_arbP->uiSliderGrp_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiSliderOne_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiSliderTwo_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiSliderThree_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiSliderFour_NameAC));
			total_bV.emplace_back(strlen(second_arbP->uiPointGrp_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiPointOne_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiPointTwo_NameAC));
			total_bV.emplace_back(strlen(second_arbP->uiColorGrp_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiColorOne_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->uiColorTwo_NameAC));
			total_aV.emplace_back ( strlen(second_arbP->uiExtLGrp_NameAC));
            total_bV.emplace_back ( strlen(second_arbP->Glsl_FragmentShFlat));
			total_bV.emplace_back ( strlen(second_arbP->Glsl_VertexShFlat));

            total_bV.emplace_back (second_arbP->CallsAEGP_CompB);
            total_bV.emplace_back (second_arbP->CallsAEGP_layerB);
			total_bV.emplace_back(second_arbP->ShaderResetB);
			total_aV.emplace_back(first_arbP->ExprResetB);
			total_bV.emplace_back(second_arbP->uiSliderGrpB);
			total_bV.emplace_back(second_arbP->uiSliderOneB);
			total_bV.emplace_back(second_arbP->uiSliderTwoB);
			total_bV.emplace_back(second_arbP->uiSliderThreeB);
			total_bV.emplace_back(second_arbP->uiSliderFourB);
			total_bV.emplace_back(second_arbP->uiPointGrpB);
			total_bV.emplace_back(second_arbP->uiPointOneB);
			total_bV.emplace_back(second_arbP->uiPointTwoB);
			total_bV.emplace_back(second_arbP->uiColorGrpB);
			total_bV.emplace_back(second_arbP->uiColorOneB);
			total_bV.emplace_back(second_arbP->uiColorTwoB);
			total_bV.emplace_back(second_arbP->uiExtLGrpB);

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

