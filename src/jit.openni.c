/****************************************************************************
	jit.openni
	Copyright (C) 2011 Dale Phurrough

	This file is part of jit.openni.

    jit.openni is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    jit.openni is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with jit.openni.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
	 jit.openni - a Max Jitter external for OpenNI middleware
	 Shell of it was imspired by the jit.simple example from the MaxSDK and
	 the MaxSDK documentation
*/


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "targetver.h"
#include "jit.openni.h"

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

// globals
static void *s_jit_openni_class = NULL;


/************************************************************************************/

t_jit_err jit_openni_init(void) 
{
	t_jit_object	*attr, *mop;
	void			*output;
	t_atom			a_arr[4];
	int				i;

	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, NUM_OPENNI_MAPS); // no matrix inputs, matrix outputs (generator outputs) + default dumpout

#ifdef DEPTHMAP_OUTPUT_INDEX
	//jit_mop_output_nolink(mop, DEPTHMAP_OUTPUT_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop, _jit_sym_getoutput, DEPTHMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

#ifdef IMAGEMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, IMAGEMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop, _jit_sym_getoutput, IMAGEMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,4);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	//jit_atom_setlong(&a_arr[0], 640);
	//jit_atom_setlong(&a_arr[1], 480);
	//jit_object_method(output,_jit_sym_maxdim,2,a_arr);
	//jit_object_method(output,_jit_sym_mindim,2,a_arr);
	jit_atom_setsym(&a_arr[0], _jit_sym_char);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_long);
	jit_atom_setsym(&a_arr[2], _jit_sym_float32);
	jit_atom_setsym(&a_arr[3], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,4,a_arr);
#endif

#ifdef USERPIXELMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, USERPIXELMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop,_jit_sym_getoutput, USERPIXELMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

#ifdef IRMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, IRMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop,_jit_sym_getoutput, IRMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

	jit_class_addadornment(s_jit_openni_class, mop);

	// add method(s)
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_matrix_calc, "matrix_calc", A_CANT, 0);
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_init_from_xml, "init_from_xml", A_SYM, 0);

	// add attribute(s)
	attr = jit_object_new(_jit_sym_jit_attr_offset, "joint_confidence_filter", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, fJointConfidenceFilter));
	jit_class_addattr(s_jit_openni_class, attr);
	attr = jit_object_new(_jit_sym_jit_attr_offset, "orient_confidence_filter", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, fOrientConfidenceFilter));
	jit_class_addattr(s_jit_openni_class, attr);
	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_skeleton_orientation", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputSkeletonOrientation));
	jit_class_addattr(s_jit_openni_class, attr);
	attr = jit_object_new(_jit_sym_jit_attr_offset, "skeleton_smooth_factor", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, fSkeletonSmoothingFactor));
	jit_class_addattr(s_jit_openni_class, attr);

	// finalize class
	jit_class_register(s_jit_openni_class);
	return JIT_ERR_NONE;
}


/************************************************************************************/
// Object Life Cycle

t_jit_openni *jit_openni_new(void)
{
	t_jit_openni	*x = NULL;
	
	x = (t_jit_openni*)jit_object_alloc(s_jit_openni_class);
	if (x)
	{
		XnStatus nRetVal = XN_STATUS_OK;
		x->bHaveValidGeneratorProductionNode = false;
		x->bNeedPose = false;
		x->bHaveSkeletonSupport = false;
		x->fJointConfidenceFilter = 1.0;
		x->fOrientConfidenceFilter = 1.0;
		x->bOutputSkeletonOrientation = 0;
		//x->fSkeletonSmoothingFactor = 1.0;	//BUGBUG what is the OpenNI/NITE default?
		x->pUserSkeletonJoints = NULL;

		LOG_DEBUG("Initializing OpenNI library");
		if (nRetVal = xnInit(&(x->pContext)))
		{
			LOG_ERROR("jit_openni_new: cannot initialize OpenNI");
		}
		else
		{
			x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateDepthMetaData();
			x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateImageMetaData();
			x->pMapMetaData[IRMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateIRMetaData();
			x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateSceneMetaData();
		}
	} 
	LOG_DEBUG("object created");
	return x;
}


void jit_openni_free(t_jit_openni *x)
{
	XnStatus nRetVal = XN_STATUS_OK;

	if (x->pContext) nRetVal = xnStopGeneratingAll(x->pContext);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_free(): cannot stop all generators");

	xnFreeDepthMetaData((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
	xnFreeImageMetaData((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
	xnFreeIRMetaData((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
	xnFreeSceneMetaData((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
	if (x->pUserSkeletonJoints) sysmem_freeptr(x->pUserSkeletonJoints);
	if (x->pProductionNodeList) xnNodeInfoListFree(x->pProductionNodeList);
	x->bHaveValidGeneratorProductionNode = false;
	LOG_DEBUG("Shutting down OpenNI library");
	xnShutdown(x->pContext);
	LOG_DEBUG("object freed");
}


t_jit_err jit_openni_matrix_calc(t_jit_openni *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long out_savelock[NUM_OPENNI_MAPS];
	t_jit_matrix_info out_minfo;
	char *out_bp;	// char* so can reference down to a single byte as needed
	long i, j, dimcount;
	void *out_matrix[NUM_OPENNI_MAPS];
	boolean bGotOutMatrices = true;
	XnStatus nRetVal = XN_STATUS_OK;
#ifdef _DEBUG
	XnUInt16 tmpNumUsers;
#endif

	// get the zeroth index input and output from
	// the corresponding input and output lists
	for (i=0;i<NUM_OPENNI_MAPS;i++)
	{
		if (!(out_matrix[i] = jit_object_method(outputs,_jit_sym_getindex,i)))
		{
			LOG_DEBUG2("could not get output [%d] matrix", i);
			bGotOutMatrices = false;
		}
	}

	// if the object and both input and output matrices, both generators are valid, then process else error
	if (x && bGotOutMatrices && x->bHaveValidGeneratorProductionNode)
	{
		// lock input and output matrices TODO its possible to move this inside the below i through NUM_OPENNI_GENERATORS iteration if it is not important for all matrices to be locked before any work
		for (i = 0; i< NUM_OPENNI_MAPS; i++)
		{
			out_savelock[i] = (long) jit_object_method(out_matrix[i],_jit_sym_lock,1);
		}

		// Don't wait for new data, just update all generators with the newest already available
		nRetVal = xnWaitNoneUpdateAll(x->pContext);
		if (nRetVal != XN_STATUS_OK)
		{
			LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			err = JIT_ERR_DATA_UNAVAILABLE;
		}
		else
		{
			LOG_DEBUG("updated generators and metadata, wait for none");
			for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
			{
				// TODO this could instead look at productionNode and for the ones that are generators, do the following matrix setup/resizing
				// TODO adjust codebase to optionally allow no new matrices to be output for generators that have no new data
				if (x->hProductionNode[i])
				{
					// fill out matrix info structs for input and output
					jit_object_method(out_matrix[i],_jit_sym_getinfo,&out_minfo);

					LOG_DEBUG3("generator[%d] type=%s", i, xnProductionNodeTypeToString(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type));
					LOG_DEBUG3("generator[%d] is derived from map=%s", i, xnIsTypeDerivedFrom(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type, XN_NODE_TYPE_MAP_GENERATOR) ? "true":"false");
#ifdef _DEBUG
					if (!xnIsDataNew(x->hProductionNode[i])) LOG_WARNING2("generator[%d] No new data", i); //TODO remove this debug once no new data->no new matrix output is implemented
#endif
					switch(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type)
					{
					case XN_NODE_TYPE_DEPTH:
						xnGetDepthMetaData(x->hProductionNode[i], (XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_IMAGE:
						xnGetImageMetaData(x->hProductionNode[i], (XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_IR:
						xnGetIRMetaData(x->hProductionNode[i], (XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[IRMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_USER:
						xnGetUserPixels(x->hProductionNode[USER_GEN_INDEX], 0, (XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX], &out_minfo)) goto out;

						tmpNumUsers = MAX_NUM_USERS_SUPPORTED;
						xnGetUsers(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs, &tmpNumUsers);
						LOG_DEBUG2("Current num of users=%d", tmpNumUsers);
						
						if (x->bHaveSkeletonSupport)
						{
							// xnSetSkeletonSmoothing(x->hProductionNode[USER_GEN_INDEX],x->fSkeletonSmoothingFactor); // TODO need to uncomment when understand what the default is
							x->iNumUserSkeletonJoints = 0;
							for (j=0; j<tmpNumUsers; j++)
							{
								int iJoint;

								LOG_DEBUG3("user[%d]=%d", j, x->aUserIDs[j]);
								if (xnIsSkeletonTracking(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs[j]))
								{
									// fill in user and joint struct
									x->pUserSkeletonJoints[x->iNumUserSkeletonJoints].userID = x->aUserIDs[j];
									for (iJoint = 1; iJoint <= NUM_OF_SKELETON_JOINT_TYPES; iJoint++)
									{
										xnGetSkeletonJoint(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs[j], (XnSkeletonJoint)iJoint, &(x->pUserSkeletonJoints[x->iNumUserSkeletonJoints].jointTransform[iJoint]));
									}
									x->iNumUserSkeletonJoints++;
								}
							}
						}
					}
//					if (xnIsTypeDerivedFrom(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type, XN_NODE_TYPE_MAP_GENERATOR))

					LOG_DEBUG3("generator[%d] pixelformat=%s", i, xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[i])->pMap->PixelFormat));
					LOG_DEBUG("updated metadata for incoming map generator frame");
					LOG_DEBUG3("generator[%d] gotNew=%s", i, ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->bIsNew ? "true":"false"); // TODO remove this, output should match LOG_WARNING2 above

					jit_object_method(out_matrix[i], _jit_sym_setinfo, &out_minfo);
					jit_object_method(out_matrix[i], _jit_sym_getinfo, &out_minfo);	// BUGBU for some reason, I have to call this or Max crashes when you change matrix attributes via inspector
#ifdef _DEBUG
					// get dimensions/planecount
					dimcount = out_minfo.dimcount;
					for (j=0;j<dimcount;j++)
					{
						object_post((t_object*)x, "out%d dim[%d] = %d", i, j, out_minfo.dim[j]);
						object_post((t_object*)x, "out%d dimstride[%d] = %d", i, j, out_minfo.dimstride[j]);
					}
					LOG_DEBUG3("out%d planes = %d", i, out_minfo.planecount);
					LOG_DEBUG3("out%d type = %s", i, out_minfo.type->s_name);
#endif
					// get matrix data pointers
					jit_object_method(out_matrix[i],_jit_sym_getdata,&out_bp);
					if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;} // if data pointers are invalid, set error, and cleanup

					// manually copy OpenNI arrays to jitter matrix because jitter doesn't directly support them
					// by using the Jitter parallel functions
					jit_parallel_ndim_simplecalc1((method)jit_openni_calculate_ndim, x->pMapMetaData[i], out_minfo.dimcount, out_minfo.dim, out_minfo.planecount, &out_minfo, out_bp, 0);
				}
			}
		}
out:
		// restore matrix lock state to previous value
		for (i = 0; i< NUM_OPENNI_MAPS; i++)
		{
			jit_object_method(out_matrix[i],_jit_sym_lock,out_savelock[i]);
		}
	}
	else
	{
		return JIT_ERR_INVALID_OUTPUT;
	}
	return err;
}

t_jit_err changeMatrixOutputGivenMapMetaData(void *pMetaData, t_jit_matrix_info *pMatrixOut)
{
	// setup the outputs to support the map's PixelFormat, assumes 1st parameter of XnDepthMetaData is superset of all map metadatas
	switch(((XnDepthMetaData *)pMetaData)->pMap->PixelFormat)
	{
		case XN_PIXEL_FORMAT_RGB24:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 4;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_YUV422:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 4;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X / 2;	// trusting that X res will always be an even number
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 1;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
			pMatrixOut->planecount = 1;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		default:
			return JIT_ERR_MISMATCH_TYPE;
	}
	return JIT_ERR_NONE;
}

void copy16BitDatatoJitterMatrix(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, long rowOffset)
{
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pMapMetaData have the same byte location of the 	
	
	int i, j;
	XnUInt16 *p16BitData; // aka XnDepthPixel or 16bit greysacale xnImagePixel

	// this ->pData assumes XnDepthMetaData struct, and pointer arithmetic automatically jumps by 2 bytes
	p16BitData = (XnUInt16 *)pMapMetaData->pData + (rowOffset * pMapMetaData->pMap->FullRes.X);
	
	for(i=0; i < dim[1]; i++)  // for each row
	{
		for(j=0; j < dim[0]; j++)  // go across each column
		{
			if (minfo1->type == _jit_sym_long)
			{
				((unsigned long *)bp1)[j] = *p16BitData;
			}
			else if (minfo1->type == _jit_sym_float32)
			{
				((float *)bp1)[j] = *p16BitData;
			}
			else // it is _jit_sym_float64
			{
				((double *)bp1)[j] = *p16BitData;
			}
			p16BitData++;
		}
		bp1 += minfo1->dimstride[1];
	}
}

void jit_openni_calculate_ndim(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, t_jit_parallel_ndim_worker *para_worker)
{
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pMapMetaData have the same byte location of the 	
	// uses the t_jit_parallel_ndim_worker undocumented functionality as posted in the formums at http://cycling74.com/forums/topic.php?id=24525
	int i, j;
	long rowOffset = para_worker->offset[1];

	if (pMapMetaData->pMap->PixelFormat != XN_PIXEL_FORMAT_GRAYSCALE_16_BIT)
	{
		XnUInt8 *pMapData;

		// this ->pData assumes XnDepthMetaData struct, and pointer arithmetic jumps only 1 bytes so need to multiply by PixelFormat size
		pMapData = (XnUInt8 *)pMapMetaData->pData + (rowOffset * (pMapMetaData->pMap->FullRes.X * xnGetBytesPerPixelForPixelFormat(pMapMetaData->pMap->PixelFormat)));

		for(i=0; i < dim[1]; i++) // for each row
		{
			for(j=0; j < dim[0]; j++)  // go across each column
			{
				switch(pMapMetaData->pMap->PixelFormat)
				{
					case XN_PIXEL_FORMAT_RGB24:
						((unsigned long *)bp1)[j] = MAKEULONGFROMCHARS(0xFF, pMapData[0], pMapData[1], pMapData[2]); // not tested on big endian systems
						pMapData += 3;
						break;
					case XN_PIXEL_FORMAT_YUV422:	// ordering is U, Y1, V, Y2; if I give up sources that are not 4-bte aligned, I could use Jitter matrix copying functions
						((unsigned long *)bp1)[j] = MAKEULONGFROMCHARS(pMapData[0], pMapData[1], pMapData[2], pMapData[3]); // not tested on big endian systems
						pMapData += 4;
						break;
					case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:	// if I give up sources that are not 4-bte aligned, I could use Jitter matrix copying functions
															// TODO add support for long, float32, float64 output matrices
						bp1[j] = *pMapData++;
						break;
					// case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT is now handled below by calling a shared function copy16BitDatatoJitterMatrix()
				}
			}
			bp1 += minfo1->dimstride[1];
		}
	}
	else
	{
		copy16BitDatatoJitterMatrix(pMapMetaData, dimcount, dim, planecount, minfo1, bp1, rowOffset);
	}
}

void jit_openni_init_from_xml(t_jit_openni *x, t_symbol *s)
{
	XnEnumerationErrors* pErrors;
	XnStatus nRetVal = XN_STATUS_OK;
	XnNodeInfoListIterator pCurrentNode;
	XnNodeInfo* pProdNodeInfo;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot allocate errors object");

	nRetVal = xnStopGeneratingAll(x->pContext);		// should stop generators in case we are loading a new XML file
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot stop all generators before loading XML config");

	nRetVal = xnContextRunXmlScriptFromFile(x->pContext, s->s_name, pErrors);	// BUGBUG this doesn't seem to support loading a 2nd XML file
																				// may need to iterate xnProductionNodeRelease() or xnShutdown()
	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		LOG_ERROR2("XMLconfig initialization failed", strError);
		xnEnumerationErrorsFree(pErrors);
		return;
	}
	xnEnumerationErrorsFree(pErrors);
	if (nRetVal != XN_STATUS_OK)
	{
		CHECK_RC_ERROR_EXIT(nRetVal, "XML config initialization open failed");
	}
	LOG_DEBUG2("XMLconfig loaded: %s", s->s_name);

	nRetVal = xnEnumerateExistingNodes(x->pContext,&x->pProductionNodeList);
	CHECK_RC_ERROR_EXIT(nRetVal, "XMLconfig cannot enumerate existing production nodes");
	for (pCurrentNode = xnNodeInfoListGetFirst(x->pProductionNodeList); xnNodeInfoListIteratorIsValid(pCurrentNode); pCurrentNode = xnNodeInfoListGetNext(pCurrentNode))
	{
		pProdNodeInfo = xnNodeInfoListGetCurrent(pCurrentNode);
		LOG_DEBUG2("found prodnode type=%s", xnProductionNodeTypeToString(xnNodeInfoGetDescription(pProdNodeInfo)->Type));
		LOG_DEBUG2("Derived from map=%s", xnIsTypeDerivedFrom(xnNodeInfoGetDescription(pProdNodeInfo)->Type, XN_NODE_TYPE_MAP_GENERATOR) ? "true":"false");

		switch(xnNodeInfoGetDescription(pProdNodeInfo)->Type)
		{
		case XN_NODE_TYPE_DEVICE:
			break;
		case XN_NODE_TYPE_DEPTH:
			x->hProductionNode[DEPTH_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetDepthMetaData(x->hProductionNode[DEPTH_GEN_INDEX], (XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_IMAGE:
			x->hProductionNode[IMAGE_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetImageMetaData(x->hProductionNode[IMAGE_GEN_INDEX], (XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_IR:
			x->hProductionNode[IR_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetIRMetaData(x->hProductionNode[IR_GEN_INDEX], (XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_USER:
			x->hProductionNode[USER_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnRegisterUserCallbacks(x->hProductionNode[USER_GEN_INDEX], User_NewUser, User_LostUser, x, &(x->hUserCallbacks));
			xnGetUserPixels(x->hProductionNode[USER_GEN_INDEX], 0, (XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;

			// check for then setup skeleton support
			if (xnIsCapabilitySupported(x->hProductionNode[USER_GEN_INDEX], XN_CAPABILITY_SKELETON))
			{
				if (xnIsProfileAvailable(x->hProductionNode[USER_GEN_INDEX], XN_SKEL_PROFILE_ALL))
				{
					xnSetSkeletonProfile(x->hProductionNode[USER_GEN_INDEX], XN_SKEL_PROFILE_ALL);
					x->bHaveSkeletonSupport = true;
					xnRegisterCalibrationCallbacks(x->hProductionNode[USER_GEN_INDEX], UserCalibration_CalibrationStart, UserCalibration_CalibrationEnd, x, &(x->hCalibrationCallbacks));
					if (xnNeedPoseForSkeletonCalibration (x->hProductionNode[USER_GEN_INDEX]))
					{
						x->bNeedPose = true;
						if (xnIsCapabilitySupported(x->hProductionNode[USER_GEN_INDEX], XN_CAPABILITY_POSE_DETECTION))
						{
							LOG_DEBUG2("user generator supports %u poses", xnGetNumberOfPoses(x->hProductionNode[USER_GEN_INDEX]));
							xnRegisterToPoseCallbacks(x->hProductionNode[USER_GEN_INDEX], UserPose_PoseDetected, NULL, x, &(x->hPoseCallbacks));
							xnGetSkeletonCalibrationPose(x->hProductionNode[USER_GEN_INDEX], x->strRequiredCalibrationPose);
						}
						else
						{
							LOG_ERROR("Pose required for skeleton, but user generator doesn't support detecting poses");
							x->bHaveSkeletonSupport = false;
						}
					}
					if (x->bHaveSkeletonSupport) x->pUserSkeletonJoints = (t_user_and_joints *)sysmem_newptr(sizeof(t_user_and_joints) * MAX_NUM_USERS_SUPPORTED);
				}
				else
				{
					LOG_ERROR("User generator skeleton capability must support all joints profile XN_SKEL_PROFILE_ALL");
				}
			}
			else
			{
				LOG_DEBUG("User generator doesn't support skeleton");
				// TODO consider user generator output other than a skeleton
			}
			break;
		default:
			LOG_ERROR2("found unsupported node type", xnProductionNodeTypeToString(xnNodeInfoGetDescription(pProdNodeInfo)->Type));
		}
	}
	LOG_DEBUG2("bHaveValidGeneratorProductionNode=%s", (x->bHaveValidGeneratorProductionNode ? "true": "false"));

	nRetVal = xnStartGeneratingAll(x->pContext);
	CHECK_RC_ERROR_EXIT(nRetVal, "XMLconfig cannot start all generator nodes");

#ifdef _DEBUG
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		XnMapOutputMode *depthMapModes;
		XnUInt32 i, numDepthMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[DEPTH_GEN_INDEX]);
		if (depthMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numDepthMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[DEPTH_GEN_INDEX], depthMapModes, &numDepthMapModes);
			LOG_DEBUG2("== %lu Depth modes avail==", numDepthMapModes);
			for (i=0; i<numDepthMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu Z=%u", depthMapModes[i].nFPS, depthMapModes[i].nXRes, depthMapModes[i].nYRes, xnGetDeviceMaxDepth(x->hProductionNode[DEPTH_GEN_INDEX]));
			}
			sysmem_freeptr(depthMapModes);
		}

	}

	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		XnMapOutputMode *imageMapModes;
		XnUInt32 i, numImageMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IMAGE_GEN_INDEX]);
		if (imageMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numImageMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[IMAGE_GEN_INDEX], imageMapModes, &numImageMapModes);
			LOG_DEBUG2("== %lu Image modes avail==", numImageMapModes);
			for (i=0; i<numImageMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", imageMapModes[i].nFPS, imageMapModes[i].nXRes, imageMapModes[i].nYRes);
			}
			sysmem_freeptr(imageMapModes);
		}
	}

	if (x->hProductionNode[USER_GEN_INDEX])
	{
		LOG_DEBUG("== Created user generator ==");
		if (xnIsCapabilitySupported(x->hProductionNode[USER_GEN_INDEX], XN_CAPABILITY_SKELETON))
		{
			LOG_DEBUG("Supports skeletons");
			if (xnNeedPoseForSkeletonCalibration (x->hProductionNode[USER_GEN_INDEX]))
			{
				LOG_DEBUG("Skeleton needs a pose");
			}
		}
	}
	
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		XnMapOutputMode *IrMapModes;
		XnUInt32 i, numIrMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IR_GEN_INDEX]);
		if (IrMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numIrMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[IR_GEN_INDEX], IrMapModes, &numIrMapModes);
			LOG_DEBUG2("== %lu IR modes avail==", numIrMapModes);
			for (i=0; i<numIrMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", IrMapModes[i].nFPS, IrMapModes[i].nXRes, IrMapModes[i].nYRes);
			}
			sysmem_freeptr(IrMapModes);
		}
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		object_post((t_object*)x, "DepthMD FPS=%lu FullX=%lu FullY=%lu Z=%u", ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->FullRes.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->nZRes);
		object_post((t_object*)x, "DepthMD OffsetX=%lu OffsetY=%lu CropX=%lu CropY=%lu", ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Offset.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Offset.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Res.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Res.Y);
		object_post((t_object*)x, "DepthMD PixelFormat=%s", xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "ImageMD PixelFormat=%s", xnPixelFormatToString(((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[USER_GEN_INDEX])
	{
		object_post((t_object*)x, "UserPixelMD FPS=%lu X=%lu Y=%lu", ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "UserPixelMD PixelFormat=%s", xnPixelFormatToString(((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		object_post((t_object*)x, "IrMD FPS=%lu X=%lu Y=%lu", ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "IrMD PixelFormat=%s", xnPixelFormatToString(((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
#endif

}
// ---------- user and skeleton generator code -----------------

// Callback: New user was detected
void __stdcall User_NewUser(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	LOG_COMMENT2("New User %d\n", userID);
	// New user found
	if (x->bNeedPose)
	{
		xnStartPoseDetection(hUserGenerator, x->strRequiredCalibrationPose, userID);
	}
	else
	{
		xnRequestSkeletonCalibration(hUserGenerator, userID, true);
	}
}

// Callback: An existing user was lost
void __stdcall User_LostUser(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	LOG_COMMENT2("Lost user %d\n", userID);
}

// Callback: Detected a pose
void __stdcall UserPose_PoseDetected(XnNodeHandle hPoseCapability, const XnChar *strPose, XnUserID userID, t_jit_openni *x)
{
	LOG_COMMENT3("Pose %s detected for user %d\n", strPose, userID);
	xnStopPoseDetection(hPoseCapability, userID);
	xnRequestSkeletonCalibration(hPoseCapability, userID, true);
}

// Callback: Started calibration
void __stdcall UserCalibration_CalibrationStart(XnNodeHandle hSkeletonCapability, XnUserID userID, t_jit_openni *x)
{
	LOG_COMMENT2("Calibration started for user %d", userID);
}

// Callback: Finished calibration
void __stdcall UserCalibration_CalibrationEnd(XnNodeHandle hSkeletonCapability, XnUserID userID, XnBool bSuccess, t_jit_openni *x)
{
	if (bSuccess)
	{
		// Calibration succeeded
		LOG_COMMENT2("Calibration complete, start tracking user %d", userID);
		xnStartSkeletonTracking(hSkeletonCapability, userID);
	}
	else
	{
		// Calibration failed
		LOG_COMMENT2("Calibration failed for user %d", userID);
		if (x->bNeedPose)
		{
			xnStartPoseDetection(hSkeletonCapability, x->strRequiredCalibrationPose, userID);
		}
		else
		{
			xnRequestSkeletonCalibration(hSkeletonCapability, userID, true);
		}
	}
}
