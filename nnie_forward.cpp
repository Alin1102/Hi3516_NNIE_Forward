/******************************************************************************
  File Name     : nnie_forward.cpp
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : nnie_forward source code, common utils for nnie
******************************************************************************/

#include "nnie_forward.h"

HI_S32 SAMPLE_COMM_SVP_MallocMem(HI_CHAR *pszMmb, HI_CHAR *pszZone, HI_U64 *pu64PhyAddr, HI_VOID **ppvVirAddr, HI_U32 u32Size) {
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_SYS_MmzAlloc(pu64PhyAddr, ppvVirAddr, pszMmb, pszZone, u32Size);

    return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_LoadModel(const HI_CHAR* pszModelFile, SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel) {
	HI_S32 s32Ret = HI_SUCCESS;

	HI_U64 u64PhyAddr = 0;
	HI_U8 *pu8VirAddr = NULL;
	HI_SL slFileSize = 0;

	/*Get model file size*/
	FILE *fp = fopen(pszModelFile, "rb");
    if (fp == NULL) {
        // cout << "Error, open model file failed!" << endl;
        printf("Error, open model file failed!");
        return HI_FAILURE;
    }

    s32Ret = fseek(fp, 0L, SEEK_END);
    if (s32Ret != HI_SUCCESS) {
        // cout << "Error, open model fseek failed!" << endl;
        printf("Error, open model fseek failed!");
        fclose(fp);
	    return HI_FAILURE;
    }

    slFileSize = ftell(fp);
    if (slFileSize <= 0) {
        // cout << "Error, open model ftell failed!" << endl;
        printf("Error, open model ftell failed!");
        fclose(fp);
	    return HI_FAILURE;
    }

	s32Ret = fseek(fp, 0L, SEEK_SET);
    if (s32Ret != HI_SUCCESS) {
        // cout << "Error, open model fseek failed!" << endl;
        printf("Error, open model fseek failed!");
        fclose(fp);
	    return HI_FAILURE;
    }

	/*malloc model file mem*/
	s32Ret = SAMPLE_COMM_SVP_MallocMem("SAMPLE_NNIE_MODEL", NULL, (HI_U64*)&u64PhyAddr, (void**)&pu8VirAddr, slFileSize);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,Malloc memory failed!" << endl;
        printf("Error, Malloc memory failed!");
        fclose(fp);
	    return HI_FAILURE;
    }

	pstNnieModel->stModelBuf.u32Size = (HI_U32)slFileSize;
	pstNnieModel->stModelBuf.u64PhyAddr = u64PhyAddr;
	pstNnieModel->stModelBuf.u64VirAddr = (HI_U64)pu8VirAddr;

    s32Ret = fread(pu8VirAddr, slFileSize, 1, fp);
    if (1 != s32Ret) {
        // cout << "Error,read model file failed!" << endl;
        printf("Error, read model file failed!");
        SAMPLE_SVP_MMZ_FREE(pstNnieModel->stModelBuf.u64PhyAddr,pstNnieModel->stModelBuf.u64VirAddr);
        pstNnieModel->stModelBuf.u32Size  = 0;
        fclose(fp);
	    return HI_FAILURE;
    }

	/*load model*/
	s32Ret = HI_MPI_SVP_NNIE_LoadModel(&pstNnieModel->stModelBuf, &pstNnieModel->stModel);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error, HI_MPI_SVP_NNIE_LoadModel failed!" << endl;
        printf("Error, HI_MPI_SVP_NNIE_LoadModel failed!");
        SAMPLE_SVP_MMZ_FREE(pstNnieModel->stModelBuf.u64PhyAddr,pstNnieModel->stModelBuf.u64VirAddr);
        pstNnieModel->stModelBuf.u32Size  = 0;
        fclose(fp);
	    return HI_FAILURE;
    }

	fclose(fp);
    return HI_SUCCESS;
}

static HI_S32 SAMPLE_SVP_NNIE_FillForwardInfo(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam) {
	HI_U32 i = 0, j = 0;
	HI_U32 u32Offset = 0;
	HI_U32 u32Num = 0;

	for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        /*fill forwardCtrl info*/
		if (SVP_NNIE_NET_TYPE_ROI == pstNnieParam->pstModel->astSeg[i].enNetType) {
			pstNnieParam->astForwardWithBboxCtrl[i].enNnieId = pstNnieCfg->aenNnieCoreId[i];
			pstNnieParam->astForwardWithBboxCtrl[i].u32SrcNum = pstNnieParam->pstModel->astSeg[i].u16SrcNum;
			pstNnieParam->astForwardWithBboxCtrl[i].u32DstNum = pstNnieParam->pstModel->astSeg[i].u16DstNum;
			pstNnieParam->astForwardWithBboxCtrl[i].u32ProposalNum = 1;
			pstNnieParam->astForwardWithBboxCtrl[i].u32NetSegId = i;
			pstNnieParam->astForwardWithBboxCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64PhyAddr= pstNnieParam->stTaskBuf.u64PhyAddr+u32Offset;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64VirAddr= pstNnieParam->stTaskBuf.u64VirAddr+u32Offset;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u32Size= pstNnieParam->au32TaskBufSize[i];
		} else if(SVP_NNIE_NET_TYPE_CNN == pstNnieParam->pstModel->astSeg[i].enNetType ||
            SVP_NNIE_NET_TYPE_RECURRENT== pstNnieParam->pstModel->astSeg[i].enNetType) {
			pstNnieParam->astForwardCtrl[i].enNnieId = pstNnieCfg->aenNnieCoreId[i];
			pstNnieParam->astForwardCtrl[i].u32SrcNum = pstNnieParam->pstModel->astSeg[i].u16SrcNum;
			pstNnieParam->astForwardCtrl[i].u32DstNum = pstNnieParam->pstModel->astSeg[i].u16DstNum;
			pstNnieParam->astForwardCtrl[i].u32NetSegId = i;
			pstNnieParam->astForwardCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u64PhyAddr= pstNnieParam->stTaskBuf.u64PhyAddr+u32Offset;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u64VirAddr= pstNnieParam->stTaskBuf.u64VirAddr+u32Offset;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u32Size= pstNnieParam->au32TaskBufSize[i];
		}
		u32Offset += pstNnieParam->au32TaskBufSize[i];

        /*fill src blob info*/
		for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16SrcNum; j++) {
            /*Recurrent blob*/
            if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType) {
                pstNnieParam->astSegData[i].astSrc[j].enType = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stSeq.u32Dim = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.u32Dim;
                pstNnieParam->astSegData[i].astSrc[j].u32Num = pstNnieCfg->u32MaxInputNum;
                pstNnieParam->astSegData[i].astSrc[j].unShape.stSeq.u64VirAddrStep = pstNnieCfg->au64StepVirAddr[i*SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM];
            } else {
    		    pstNnieParam->astSegData[i].astSrc[j].enType = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].enType;
    	        pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Chn = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Chn;
    	        pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Height = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Height;
    	        pstNnieParam->astSegData[i].astSrc[j].unShape.stWhc.u32Width = pstNnieParam->pstModel->astSeg[i].astSrcNode[j].unShape.stWhc.u32Width;
    	        pstNnieParam->astSegData[i].astSrc[j].u32Num = pstNnieCfg->u32MaxInputNum;
            }
	    }

        /*fill dst blob info*/
		if (SVP_NNIE_NET_TYPE_ROI == pstNnieParam->pstModel->astSeg[i].enNetType) {
			u32Num = pstNnieCfg->u32MaxRoiNum*pstNnieCfg->u32MaxInputNum;
		} else {
			u32Num = pstNnieCfg->u32MaxInputNum;
		}

		for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16DstNum; j++) {
            if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType) {
    			pstNnieParam->astSegData[i].astDst[j].enType = pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType;
    			pstNnieParam->astSegData[i].astDst[j].unShape.stSeq.u32Dim =
                    pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.u32Dim;
                pstNnieParam->astSegData[i].astDst[j].u32Num = u32Num;
                pstNnieParam->astSegData[i].astDst[j].unShape.stSeq.u64VirAddrStep =
                    pstNnieCfg->au64StepVirAddr[i*SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM+1];
            } else {
    		    pstNnieParam->astSegData[i].astDst[j].enType = pstNnieParam->pstModel->astSeg[i].astDstNode[j].enType;
    		    pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Chn = pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Chn;
    		    pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Height = pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Height;
    		    pstNnieParam->astSegData[i].astDst[j].unShape.stWhc.u32Width = pstNnieParam->pstModel->astSeg[i].astDstNode[j].unShape.stWhc.u32Width;
    		    pstNnieParam->astSegData[i].astDst[j].u32Num = u32Num;
            }
		}
	}
	return HI_SUCCESS;
}

static void SAMPLE_SVP_NNIE_GetBlobMemSize(SVP_NNIE_NODE_S astNnieNode[], HI_U32 u32NodeNum,
	HI_U32 u32TotalStep,SVP_BLOB_S astBlob[], HI_U32 u32Align, HI_U32* pu32TotalSize,HI_U32 au32BlobSize[]) {

	HI_U32 i = 0;
	HI_U32 u32Size = 0;
	HI_U32 u32Stride = 0;

	for (i = 0; i < u32NodeNum; i++) {
		if (SVP_BLOB_TYPE_S32== astNnieNode[i].enType||SVP_BLOB_TYPE_VEC_S32== astNnieNode[i].enType||
            SVP_BLOB_TYPE_SEQ_S32== astNnieNode[i].enType) {
			u32Size = sizeof(HI_U32);
		} else {
			u32Size = sizeof(HI_U8);
		}
        if (SVP_BLOB_TYPE_SEQ_S32 == astNnieNode[i].enType) {
            if (SAMPLE_SVP_NNIE_ALIGN_16 == u32Align) {
    			u32Stride = SAMPLE_SVP_NNIE_ALIGN16(astNnieNode[i].unShape.u32Dim*u32Size);
    		} else {
    			u32Stride = SAMPLE_SVP_NNIE_ALIGN32(astNnieNode[i].unShape.u32Dim*u32Size);
    		}
            au32BlobSize[i] = u32TotalStep*u32Stride;
        } else {
            if (SAMPLE_SVP_NNIE_ALIGN_16 == u32Align) {
    			u32Stride = SAMPLE_SVP_NNIE_ALIGN16(astNnieNode[i].unShape.stWhc.u32Width*u32Size);
    		} else {
    			u32Stride = SAMPLE_SVP_NNIE_ALIGN32(astNnieNode[i].unShape.stWhc.u32Width*u32Size);
    		}
    		au32BlobSize[i] = astBlob[i].u32Num*u32Stride*astNnieNode[i].unShape.stWhc.u32Height*
    			astNnieNode[i].unShape.stWhc.u32Chn;
        }
		*pu32TotalSize += au32BlobSize[i];
	    astBlob[i].u32Stride = u32Stride;
	}
}

static HI_S32 SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg,
    SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam, HI_U32*pu32TotalTaskBufSize, HI_U32*pu32TmpBufSize,
    SAMPLE_SVP_NNIE_BLOB_SIZE_S astBlobSize[], HI_U32*pu32TotalSize) {

	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 i = 0, j = 0;
    HI_U32 u32TotalStep = 0;

	/*Get each seg's task buf size*/
	s32Ret = HI_MPI_SVP_NNIE_GetTskBufSize(pstNnieCfg->u32MaxInputNum, pstNnieCfg->u32MaxRoiNum,
		pstNnieParam->pstModel, pstNnieParam->au32TaskBufSize,pstNnieParam->pstModel->u32NetSegNum);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,HI_MPI_SVP_NNIE_GetTaskSize failed!" << endl;
        printf("Error, HI_MPI_SVP_NNIE_GetTaskSize failed!");
    }

    /*Get total task buf size*/
	*pu32TotalTaskBufSize = 0;
	for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
		*pu32TotalTaskBufSize += pstNnieParam->au32TaskBufSize[i];
	}

	/*Get tmp buf size*/
	*pu32TmpBufSize = pstNnieParam->pstModel->u32TmpBufSize;
	*pu32TotalSize += *pu32TotalTaskBufSize + *pu32TmpBufSize;

	/*calculate Blob mem size*/
	for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
        if (SVP_NNIE_NET_TYPE_RECURRENT == pstNnieParam->pstModel->astSeg[i].enNetType) {
            for (j = 0; j < pstNnieParam->astSegData[i].astSrc[0].u32Num; j++) {
                u32TotalStep += *((HI_S32*)pstNnieParam->astSegData[i].astSrc[0].unShape.stSeq.u64VirAddrStep+j);
            }
        }
		/*the first seg's Src Blob mem size, other seg's src blobs from the output blobs of
		those segs before it or from software output results*/
		if (i == 0) {
			SAMPLE_SVP_NNIE_GetBlobMemSize(&(pstNnieParam->pstModel->astSeg[i].astSrcNode[0]),
			    pstNnieParam->pstModel->astSeg[i].u16SrcNum,u32TotalStep,&(pstNnieParam->astSegData[i].astSrc[0]),
				SAMPLE_SVP_NNIE_ALIGN_16, pu32TotalSize, &(astBlobSize[i].au32SrcSize[0]));
		}

		/*Get each seg's Dst Blob mem size*/
		SAMPLE_SVP_NNIE_GetBlobMemSize(&(pstNnieParam->pstModel->astSeg[i].astDstNode[0]),
			pstNnieParam->pstModel->astSeg[i].u16DstNum,u32TotalStep,&(pstNnieParam->astSegData[i].astDst[0]),
			SAMPLE_SVP_NNIE_ALIGN_16, pu32TotalSize, &(astBlobSize[i].au32DstSize[0]));
	}
	return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_MallocCached(HI_CHAR *pszMmb, HI_CHAR *pszZone,HI_U64 *pu64PhyAddr, HI_VOID **ppvVirAddr, HI_U32 u32Size) {
	HI_S32 s32Ret = HI_SUCCESS;
	s32Ret = HI_MPI_SYS_MmzAlloc_Cached(pu64PhyAddr, ppvVirAddr, pszMmb, pszZone, u32Size);

	return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_FlushCache(HI_U64 u64PhyAddr, HI_VOID *pvVirAddr, HI_U32 u32Size) {
	HI_S32 s32Ret = HI_SUCCESS;
	s32Ret = HI_MPI_SYS_MmzFlushCache(u64PhyAddr, pvVirAddr,u32Size);
	return s32Ret;
}

static HI_S32 SAMPLE_SVP_NNIE_ParamInit(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam) {
	HI_U32 i = 0, j = 0;
	HI_U32 u32TotalSize = 0;
	HI_U32 u32TotalTaskBufSize = 0;
	HI_U32 u32TmpBufSize = 0;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32Offset = 0;
	HI_U64 u64PhyAddr = 0;
	HI_U8 *pu8VirAddr = NULL;
	SAMPLE_SVP_NNIE_BLOB_SIZE_S astBlobSize[SVP_NNIE_MAX_NET_SEG_NUM] = {0};

	/*fill forward info*/
	s32Ret = SAMPLE_SVP_NNIE_FillForwardInfo(pstNnieCfg,pstNnieParam);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,SAMPLE_SVP_NNIE_FillForwardCtrl failed!" << endl;
        printf("Error, SAMPLE_SVP_NNIE_FillForwardCtrl failed!");
    }

	/*Get taskInfo and Blob mem size*/
	s32Ret = SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize(pstNnieCfg,pstNnieParam,&u32TotalTaskBufSize,
		&u32TmpBufSize,astBlobSize,&u32TotalSize);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize failed!" << endl;
        printf("Error, SAMPLE_SVP_NNIE_GetTaskAndBlobBufSize failed!");
    }

	/*Malloc mem*/
	s32Ret = SAMPLE_COMM_SVP_MallocCached("SAMPLE_NNIE_TASK",NULL,(HI_U64*)&u64PhyAddr,(void**)&pu8VirAddr,u32TotalSize);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,Malloc memory failed!" << endl;
        printf("Error, Malloc memory failed!");
    }
	memset(pu8VirAddr, 0, u32TotalSize);
	SAMPLE_COMM_SVP_FlushCache(u64PhyAddr,(void*)pu8VirAddr,u32TotalSize);

	/*fill taskinfo mem addr*/
	pstNnieParam->stTaskBuf.u32Size = u32TotalTaskBufSize;
	pstNnieParam->stTaskBuf.u64PhyAddr = u64PhyAddr;
	pstNnieParam->stTaskBuf.u64VirAddr = (HI_U64)pu8VirAddr;

	/*fill Tmp mem addr*/
	pstNnieParam->stTmpBuf.u32Size = u32TmpBufSize;
	pstNnieParam->stTmpBuf.u64PhyAddr = u64PhyAddr+u32TotalTaskBufSize;
	pstNnieParam->stTmpBuf.u64VirAddr = (HI_U64)pu8VirAddr+u32TotalTaskBufSize;

	/*fill forward ctrl addr*/
	for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
		if (SVP_NNIE_NET_TYPE_ROI == pstNnieParam->pstModel->astSeg[i].enNetType) {
			pstNnieParam->astForwardWithBboxCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64PhyAddr= pstNnieParam->stTaskBuf.u64PhyAddr+u32Offset;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u64VirAddr= pstNnieParam->stTaskBuf.u64VirAddr+u32Offset;
			pstNnieParam->astForwardWithBboxCtrl[i].stTskBuf.u32Size= pstNnieParam->au32TaskBufSize[i];
		} else if (SVP_NNIE_NET_TYPE_CNN == pstNnieParam->pstModel->astSeg[i].enNetType ||
            SVP_NNIE_NET_TYPE_RECURRENT == pstNnieParam->pstModel->astSeg[i].enNetType) {
			pstNnieParam->astForwardCtrl[i].stTmpBuf = pstNnieParam->stTmpBuf;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u64PhyAddr= pstNnieParam->stTaskBuf.u64PhyAddr+u32Offset;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u64VirAddr= pstNnieParam->stTaskBuf.u64VirAddr+u32Offset;
			pstNnieParam->astForwardCtrl[i].stTskBuf.u32Size= pstNnieParam->au32TaskBufSize[i];
		}
		u32Offset += pstNnieParam->au32TaskBufSize[i];
	}

	/*fill each blob's mem addr*/
	u64PhyAddr =  u64PhyAddr+u32TotalTaskBufSize+u32TmpBufSize;
	pu8VirAddr = pu8VirAddr+u32TotalTaskBufSize+u32TmpBufSize;
	for (i = 0; i < pstNnieParam->pstModel->u32NetSegNum; i++) {
		/*first seg has src blobs, other seg's src blobs from the output blobs of
		those segs before it or from software output results*/
		if (0 == i) {
			for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16SrcNum; j++) {
				if (j!=0) {
					u64PhyAddr += astBlobSize[i].au32SrcSize[j-1];
					pu8VirAddr += astBlobSize[i].au32SrcSize[j-1];
				}
				pstNnieParam->astSegData[i].astSrc[j].u64PhyAddr = u64PhyAddr;
				pstNnieParam->astSegData[i].astSrc[j].u64VirAddr = (HI_U64)pu8VirAddr;
			}
			u64PhyAddr += astBlobSize[i].au32SrcSize[j-1];
			pu8VirAddr += astBlobSize[i].au32SrcSize[j-1];
		}

		/*fill the mem addrs of each seg's output blobs*/
		for (j = 0; j < pstNnieParam->pstModel->astSeg[i].u16DstNum; j++) {
			if (j!=0) {
				u64PhyAddr += astBlobSize[i].au32DstSize[j-1];
				pu8VirAddr += astBlobSize[i].au32DstSize[j-1];
			}
			pstNnieParam->astSegData[i].astDst[j].u64PhyAddr = u64PhyAddr;
			pstNnieParam->astSegData[i].astDst[j].u64VirAddr = (HI_U64)pu8VirAddr;
		}
		u64PhyAddr += astBlobSize[i].au32DstSize[j-1];
		pu8VirAddr += astBlobSize[i].au32DstSize[j-1];
	}
	return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_ParamDeinit(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam) {
    if (NULL == pstNnieParam) {
        // cout << "Error, pstNnieParam can't be NULL!" << endl;
        printf("Error, pstNnieParam can't be NULL!");
        return HI_INVALID_VALUE;
    }

	if (0!=pstNnieParam->stTaskBuf.u64PhyAddr && 0!=pstNnieParam->stTaskBuf.u64VirAddr) {
		SAMPLE_SVP_MMZ_FREE(pstNnieParam->stTaskBuf.u64PhyAddr,pstNnieParam->stTaskBuf.u64VirAddr);
		pstNnieParam->stTaskBuf.u64PhyAddr = 0;
		pstNnieParam->stTaskBuf.u64VirAddr = 0;
	}

    if (0!=pstNnieParam->stStepBuf.u64PhyAddr && 0!=pstNnieParam->stStepBuf.u64VirAddr) {
		SAMPLE_SVP_MMZ_FREE(pstNnieParam->stStepBuf.u64PhyAddr,pstNnieParam->stStepBuf.u64VirAddr);
		pstNnieParam->stStepBuf.u64PhyAddr = 0;
		pstNnieParam->stStepBuf.u64VirAddr = 0;
	}
	return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_ParamInit(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam) {
	HI_S32 s32Ret = HI_SUCCESS;

    if ((NULL == pstNnieCfg || NULL == pstNnieParam)) {
        // cout << "Error,pstNnieCfg and pstNnieParam can't be NULL!" << endl;
        printf("Error, pstNnieCfg and pstNnieParam can't be NULL!");
        return HI_FAILURE;
    }
    
    if (NULL == pstNnieParam->pstModel) {
        // cout << "Error,pstNnieParam->pstModel can't be NULL!" << endl;
        printf("Error, pstNnieParam->pstModel can't be NULL!");
        return HI_FAILURE;
    }

	s32Ret = SAMPLE_SVP_NNIE_ParamInit(pstNnieCfg, pstNnieParam);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error, SAMPLE_SVP_NNIE_ParamInit failed!" << endl;
        printf("Error, SAMPLE_SVP_NNIE_ParamInit failed");
        s32Ret = SAMPLE_COMM_SVP_NNIE_ParamDeinit(pstNnieParam);
        if (HI_SUCCESS != s32Ret) {
            // cout << "Error, SAMPLE_COMM_SVP_NNIE_ParamDeinit failed!" << endl;
            printf("Error, SAMPLE_COMM_SVP_NNIE_ParamDeinit failed!");
        }
	    return HI_FAILURE;
    }

	return s32Ret;
}

HI_S32 SAMPLE_COMM_SVP_NNIE_UnloadModel(SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel) {
	if (0 != pstNnieModel->stModelBuf.u64PhyAddr && 0 != pstNnieModel->stModelBuf.u64VirAddr) {
		SAMPLE_SVP_MMZ_FREE(pstNnieModel->stModelBuf.u64PhyAddr, pstNnieModel->stModelBuf.u64VirAddr);
		pstNnieModel->stModelBuf.u64PhyAddr = 0;
		pstNnieModel->stModelBuf.u64VirAddr = 0;
	}
    return HI_SUCCESS;
}

void NNIE_Param_Deinit(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam, SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel) {

    HI_S32 s32Ret = HI_SUCCESS;
    /*hardware para deinit*/
    if (pstNnieParam != NULL) {
        s32Ret = SAMPLE_COMM_SVP_NNIE_ParamDeinit(pstNnieParam);
        if (HI_SUCCESS != s32Ret) {
            // cout << "Error,SAMPLE_COMM_SVP_NNIE_ParamDeinit failed!" << endl;
            printf("Error, SAMPLE_COMM_SVP_NNIE_ParamDeinit failed!");
        }
    }
    /*model deinit*/
    if (pstNnieModel != NULL) {
        s32Ret = SAMPLE_COMM_SVP_NNIE_UnloadModel(pstNnieModel);
        if (HI_SUCCESS != s32Ret) {
            // cout << "Error,SAMPLE_COMM_SVP_NNIE_UnloadModel failed!" << endl;
            printf("Error, SAMPLE_COMM_SVP_NNIE_UnloadModel failed!");
        }
    }
}

static HI_S32 NNIE_FillData(const unsigned char *data,
                            SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam, SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S *pstInputDataIdx) {

    HI_U32 i = 0, j = 0, n = 0;
    HI_U32 u32Height = 0, u32Width = 0, u32Chn = 0, u32Stride = 0, u32Dim = 0;
    HI_U32 u32VarSize = 0;
    HI_U8 *pu8PicAddr = NULL;
    HI_U32 *pu32StepAddr = NULL;
    HI_U32 u32SegIdx = pstInputDataIdx->u32SegIdx;
    HI_U32 u32NodeIdx = pstInputDataIdx->u32NodeIdx;
    HI_U32 u32TotalStepNum = 0;

    /*get data size*/
    if (SVP_BLOB_TYPE_U8 <= pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].enType &&
        SVP_BLOB_TYPE_YVU422SP >= pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].enType) {
        u32VarSize = sizeof(HI_U8);
    } else {
        u32VarSize = sizeof(HI_U32);
    }

    /*fill src data*/
    if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].enType) {
        u32Dim = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].unShape.stSeq.u32Dim;
        u32Stride = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Stride;
        pu32StepAddr = (HI_U32 *)(pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].unShape.stSeq.u64VirAddrStep);
        pu8PicAddr = (HI_U8 *)(pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64VirAddr);
        for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Num; n++) {
            for (i = 0; i < *(pu32StepAddr + n); i++) {
                memcpy(pu8PicAddr, data, u32Dim * u32VarSize);
                data += u32Dim * u32VarSize;
                pu8PicAddr += u32Stride;
            }
            u32TotalStepNum += *(pu32StepAddr + n);
        }
        SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64PhyAddr,
                                   (HI_VOID *)pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64VirAddr,
                                   u32TotalStepNum * u32Stride);
    } else {
        u32Height = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].unShape.stWhc.u32Height;
        u32Width = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].unShape.stWhc.u32Width;
        u32Chn = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].unShape.stWhc.u32Chn;
        u32Stride = pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Stride;
        pu8PicAddr = (HI_U8 *)(pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64VirAddr);
        if (SVP_BLOB_TYPE_YVU420SP == pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].enType) {
            for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Num; n++) {
                for (i = 0; i < u32Chn * u32Height / 2; i++) {
                    memcpy(pu8PicAddr, data, u32Width * u32VarSize);
                    data += u32Width * u32VarSize;
                    pu8PicAddr += u32Stride;
                }
            }
        } else if (SVP_BLOB_TYPE_YVU422SP == pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].enType) {
            for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Num; n++) {
                for (i = 0; i < u32Height * 2; i++) {
                    memcpy(pu8PicAddr, data, u32Width * u32VarSize);
                    data += u32Width * u32VarSize;
                    pu8PicAddr += u32Stride;
                }
            }
        } else {
            for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Num; n++) {
                for (i = 0; i < u32Chn; i++) {
                    for (j = 0; j < u32Height; j++) {
                        memcpy(pu8PicAddr, data, u32Width * u32VarSize);
                        data += u32Width * u32VarSize;
                        pu8PicAddr += u32Stride;
                    }
                }
            }
        }
        SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64PhyAddr,
                                   (HI_VOID *)pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u64VirAddr,
                                   pstNnieParam->astSegData[u32SegIdx].astSrc[u32NodeIdx].u32Num * u32Chn * u32Height * u32Stride);
    }

    return HI_SUCCESS;
}

static HI_S32 NNIE_Forward(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam,
                                      SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S *pstInputDataIdx,
                                      SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S *pstProcSegIdx, HI_BOOL bInstant) {
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0, j = 0;
    HI_BOOL bFinish = HI_FALSE;
    SVP_NNIE_HANDLE hSvpNnieHandle = 0;
    HI_U32 u32TotalStepNum = 0;


    SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u64PhyAddr,
                               (HI_VOID *)pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u64VirAddr,
                               pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u32Size);

    /*set input blob according to node name*/
    if (pstInputDataIdx->u32SegIdx != pstProcSegIdx->u32SegIdx) {
        for (i = 0; i < pstNnieParam->pstModel->astSeg[pstProcSegIdx->u32SegIdx].u16SrcNum; i++) {
            for (j = 0; j < pstNnieParam->pstModel->astSeg[pstInputDataIdx->u32SegIdx].u16DstNum; j++) {
                if (0 == strncmp(pstNnieParam->pstModel->astSeg[pstInputDataIdx->u32SegIdx].astDstNode[j].szName,
                                 pstNnieParam->pstModel->astSeg[pstProcSegIdx->u32SegIdx].astSrcNode[i].szName,
                                 SVP_NNIE_NODE_NAME_LEN)) {
                    pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astSrc[i] =
                        pstNnieParam->astSegData[pstInputDataIdx->u32SegIdx].astDst[j];
                    break;
                }
            }
            if (j == pstNnieParam->pstModel->astSeg[pstInputDataIdx->u32SegIdx].u16DstNum) {
                // cout << "Error,can't find %d-th seg's %d-th src blob!" << endl;
                printf("Error, forward can't match seg and src blob!");
                return HI_FAILURE;
            }
        }
    }

    /*NNIE_Forward*/
    // struct timeval t0, t1;
    // gettimeofday(&t0, NULL);
    s32Ret = HI_MPI_SVP_NNIE_Forward(&hSvpNnieHandle,
                                     pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astSrc,
                                     pstNnieParam->pstModel, pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst,
                                     &pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx], bInstant);
    if (HI_SUCCESS != s32Ret) {
        // cout << "Error,HI_MPI_SVP_NNIE_Forward failed!" << endl;
        printf("Error, HI_MPI_SVP_NNIE_Forward failed!");
        return HI_FAILURE;
    }
    if (bInstant) {
        /*Wait NNIE finish*/
        while (HI_ERR_SVP_NNIE_QUERY_TIMEOUT == (s32Ret = HI_MPI_SVP_NNIE_Query(pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].enNnieId,
                                                                                hSvpNnieHandle, &bFinish, HI_TRUE)))
        {
            usleep(100);
            // cout << "HI_MPI_SVP_NNIE_Query Query timeout!" << endl;
            printf("forward HI_MPI_SVP_NNIE_Query Query timeout!");
        }
    }

    
    // gettimeofday(&t1, NULL);
    // float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
    // cout << "NNIE_Forward 2 : HI_MPI_SVP_NNIE_Forward time : " << cost_time << "ms" << endl;


    bFinish = HI_FALSE;
    for (i = 0; i < pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].u32DstNum; i++) {
        if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].enType) {
            for (j = 0; j < pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Num; j++) {
                u32TotalStepNum += *((HI_U32 *)(pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stSeq.u64VirAddrStep) + j);
            }
            SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64PhyAddr,
                                       (HI_VOID *)pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64VirAddr,
                                       u32TotalStepNum * pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Stride);
        } else {

            SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64PhyAddr,
                                       (HI_VOID *)pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64VirAddr,
                                       pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Num *
                                           pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stWhc.u32Chn *
                                           pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stWhc.u32Height *
                                           pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Stride);
        }
    }

    return s32Ret;
}

static HI_S32  NNIE_Get_Result(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam, Tensor *output_tensors) {
    HI_U32 u32SegNum = pstNnieParam->pstModel->u32NetSegNum;
    HI_U32 i = 0, j = 0, k = 0, n = 0;
    HI_U32 u32SegIdx = 0, u32NodeIdx = 0;
    HI_U32 *pu32StepAddr = NULL;
    HI_S32 *ps32ResultAddr = NULL;
    HI_U32 u32Height = 0, u32Width = 0, u32Chn = 0, u32Stride = 0, u32Dim = 0;

    for (u32SegIdx = 0; u32SegIdx < u32SegNum; u32SegIdx++) {
        for (u32NodeIdx = 0; u32NodeIdx < pstNnieParam->pstModel->astSeg[u32SegIdx].u16DstNum; u32NodeIdx++) {
            Tensor t;
            if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].enType) {
                u32Dim = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].unShape.stSeq.u32Dim;
                u32Stride = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u32Stride;
                pu32StepAddr = (HI_U32 *)(pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].unShape.stSeq.u64VirAddrStep);
                ps32ResultAddr = (HI_S32 *)(pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u64VirAddr);

                for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u32Num; n++) {
                    for (i = 0; i < *(pu32StepAddr + n); i++) {
                        for (j = 0; j < u32Dim; j++)
                        {
                        }
                        ps32ResultAddr += u32Stride / sizeof(HI_U32);
                    }
                }
            } else {
                u32Height = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].unShape.stWhc.u32Height;
                u32Width = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].unShape.stWhc.u32Width;
                u32Chn = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].unShape.stWhc.u32Chn;
                u32Stride = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u32Stride;
                ps32ResultAddr = (HI_S32 *)(pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u64VirAddr);

                printf("NNIE foward result (height: %d, width: %d, chn: %d, stride: %d\n", u32Height, u32Width, u32Chn, u32Stride);
                t.n = pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u32Num;
                t.channel = u32Chn;
                t.height = u32Height;
                t.width = u32Width;
                t.data = (int *)ps32ResultAddr;


                
				// yolov5后处理，不需要调整输出
                /*int *tmp = t.data;
                for (n = 0; n < pstNnieParam->astSegData[u32SegIdx].astDst[u32NodeIdx].u32Num; n++) {
                    for (i = 0; i < u32Chn; i++) {
                        for (j = 0; j < u32Height; j++) {
                            for (k = 0; k < u32Width; k++) {
                                *(tmp + k) = (*(ps32ResultAddr + k));  1.0f / SAMPLE_SVP_NNIE_QUANT_BASE;
                            }
                            ps32ResultAddr += u32Stride / sizeof(HI_U32);
                            // printf("ps32ResultAddr += %d\n", u32Stride / sizeof(HI_U32));
                            // printf("tmp += %d\n", t.width);
                            tmp += t.width;
                        }
                    }
                }*/

                output_tensors[u32NodeIdx] = t;
            }
        }
    }
    return HI_SUCCESS;
}

void NNIE_Forward_From_Data(const unsigned char* data, SAMPLE_SVP_NNIE_MODEL_S* s_stModel,
                            SAMPLE_SVP_NNIE_PARAM_S* s_stNnieParam, Tensor *output_tensors, int TestMode) {
    HI_S32 s32Ret = HI_SUCCESS;

    SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S stInputDataIdx = {0};
    SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S stProcSegIdx = {0};

    stInputDataIdx.u32SegIdx = 0;
    stInputDataIdx.u32NodeIdx = 0;

    if (TestMode) {
        struct timeval t0, t1;
        gettimeofday(&t0, NULL);
        s32Ret = NNIE_FillData(data, s_stNnieParam, &stInputDataIdx);
	    gettimeofday(&t1, NULL);
	    float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
	    cout << "NNIE_FillData time : " << cost_time << "ms" << endl;
    } else {
        s32Ret = NNIE_FillData(data, s_stNnieParam, &stInputDataIdx);
    }

    if (HI_SUCCESS != s32Ret) {
        cout << "Error,SAMPLE_SVP_NNIE_FillSrcData failed!" << endl;
    }

    /*NNIE process(process the 0-th segment)*/
    stProcSegIdx.u32SegIdx = 0;
    if (TestMode) {
        struct timeval t0, t1;
        gettimeofday(&t0, NULL);
        s32Ret = NNIE_Forward(s_stNnieParam, &stInputDataIdx, &stProcSegIdx, HI_TRUE);
	    gettimeofday(&t1, NULL);
	    float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
	    cout << "Net forward time : " << cost_time << "ms" << endl;
    } else {
        s32Ret = NNIE_Forward(s_stNnieParam, &stInputDataIdx, &stProcSegIdx, HI_TRUE);
    }

    if (HI_SUCCESS != s32Ret) {
        cout << "Error,SAMPLE_SVP_NNIE_Forward failed!" << endl;
        NNIE_Param_Deinit(s_stNnieParam, s_stModel);
    }

    if (TestMode) {
        struct timeval t0, t1;
        gettimeofday(&t0, NULL);
        s32Ret = NNIE_Get_Result(s_stNnieParam, output_tensors);
	    gettimeofday(&t1, NULL);
	    float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
	    cout << "Get result time : " << cost_time << "ms" << endl;
    } else {
        s32Ret = NNIE_Get_Result(s_stNnieParam, output_tensors);
    }

    if (HI_SUCCESS != s32Ret) {
        cout << "Error,SAMPLE_SVP_NNIE_PrintReportResult failed!" << endl;
    }

    return;
}



NnieNet::NnieNet(char* model_file_) {
    model_file = model_file_;
    TestMode = 0;
    output_tensors_ = NULL;
    self_name = "NnieNet Module: ";
}

NnieNet::~NnieNet() {
}

HI_S32 NnieNet::init() {
	// 根据模型路径加载模型
	HI_S32 s32Ret = loadModel();
	if (HI_SUCCESS != s32Ret) {
		printf("%s Load Model failed for %#x!", self_name, s32Ret);
        return s32Ret;
    }

    s32Ret = nnieParamInit(&s_stModel_, &stNnieCfg_, &s_stNnieParam_);
    if (HI_SUCCESS != s32Ret) {
    	printf("%s Param Init failed for %#x!", self_name, s32Ret);
        return s32Ret;
    }

    int OutputNodeNum_ = s_stModel_.stModel.astSeg[0].u16DstNum;
    // cout << "OutputNodeNum_ " << OutputNodeNum_ << endl;
    output_tensors_ = new Tensor[OutputNodeNum_];

    return HI_SUCCESS;
}

HI_S32 NnieNet::deInit() {
	// 与init顺序相反
	HI_S32 s32Ret = SAMPLE_COMM_SVP_NNIE_ParamDeinit(&s_stNnieParam_);
	if (HI_SUCCESS != s32Ret) {
		printf("%s SAMPLE_COMM_SVP_NNIE_ParamDeinit failed for %#x!", self_name, s32Ret);
    }

    s32Ret = SAMPLE_COMM_SVP_NNIE_UnloadModel(&s_stModel_);
    if (HI_SUCCESS != s32Ret) {
		printf("%s SAMPLE_COMM_SVP_NNIE_UnloadModel failed for %#x!", self_name, s32Ret);
    }

    if (output_tensors_ != NULL) {
    	delete [] output_tensors_;
    }

    return HI_SUCCESS;
}

HI_S32 NnieNet::loadModel() {
    HI_S32 s32Ret = SAMPLE_COMM_SVP_NNIE_LoadModel(model_file, &s_stModel_);
    // if (HI_SUCCESS != s32Ret) {
        // cout << "Error, SAMPLE_COMM_SVP_NNIE_LoadModel failed!" << endl;
        // return -1;
    // }
    return s32Ret;
}

HI_S32 NnieNet::nnieParamInit(SAMPLE_SVP_NNIE_MODEL_S *s_stModel, SAMPLE_SVP_NNIE_CFG_S *stNnieCfg, 
                                SAMPLE_SVP_NNIE_PARAM_S *s_stNnieParam) {
    stNnieCfg->u32MaxInputNum = 1;
    stNnieCfg->u32MaxRoiNum = 0;
    stNnieCfg->aenNnieCoreId[0] = SVP_NNIE_ID_0;
    s_stNnieParam->pstModel = &s_stModel->stModel;

    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = SAMPLE_COMM_SVP_NNIE_ParamInit(stNnieCfg, s_stNnieParam);
    if (HI_SUCCESS != s32Ret) {
        NNIE_Param_Deinit(s_stNnieParam, s_stModel);
        return s32Ret;
    }

    return s32Ret;
}

void NnieNet::run(const unsigned char *data) {
    NNIE_Forward_From_Data(data, &s_stModel_, &s_stNnieParam_, output_tensors_, TestMode);
}

void NnieNet::run(const char *file_path, int rgbOrder) {

    Mat img = imread(file_path);

    int hs = img.rows;
    int ws = img.cols;

    unsigned char *img_data = (unsigned char*)img.data;

    int input_length = hs * ws * 3;
    unsigned char *input_data = (unsigned char*)malloc(sizeof(unsigned char) * input_length);

    int hw = ws * hs;

    if (rgbOrder == 0) {
        for (int h = 0; h < hs; h ++) {
            for (int w = 0; w < ws; w ++) {
                for (int c = 2; c >= 0; c --) {
                    input_data[c * hw + h * ws + w] = (*img_data);
                    img_data ++;
                }
            }
        }
    } else if (rgbOrder == 1) {
        for (int h = 0; h < hs; h ++) {
            for (int w = 0; w < ws; w ++) {
                for (int c = 0; c < 3; c ++) {
                    input_data[c * hw + h * ws + w] = (*img_data);
                    img_data ++;
                }
            }
        }
    }

    NNIE_Forward_From_Data(input_data, &s_stModel_, &s_stNnieParam_, output_tensors_, TestMode);

    free(input_data);
}

void NnieNet::run(Mat &img, int rgbOrder) {
    int hs = img.rows;
    int ws = img.cols;

    unsigned char *img_data = (unsigned char*)img.data;

    int input_length = hs * ws * 3;
    unsigned char *input_data = (unsigned char*)malloc(sizeof(unsigned char) * input_length);

    int hw = ws * hs;

    if (rgbOrder == 0) {
        for (int h = 0; h < hs; h ++) {
            for (int w = 0; w < ws; w ++) {
                for (int c = 2; c >= 0; c --) {
                    input_data[c * hw + h * ws + w] = (*img_data);
                    img_data ++;
                }
            }
        }
    } else if (rgbOrder == 1) {
        for (int h = 0; h < hs; h ++) {
            for (int w = 0; w < ws; w ++) {
                for (int c = 0; c < 3; c ++) {
                    input_data[c * hw + h * ws + w] = (*img_data);
                    img_data ++;
                }
            }
        }
    }

    NNIE_Forward_From_Data(input_data, &s_stModel_, &s_stNnieParam_, output_tensors_, TestMode);

    free(input_data);
}

// 输入YUV420SP，进行前向推理
HI_S32 NnieNet::run(VIDEO_FRAME_INFO_S* pstFrmInfo) {
	HI_S32 s32Ret = HI_FAILURE;
	// int TestMode = 0;

    SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S stInputDataIdx = {0};
    SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S stProcSegIdx = {0};
    stInputDataIdx.u32SegIdx = 0;
    stInputDataIdx.u32NodeIdx = 0;
    /*SP420*/
    s_stNnieParam_.astSegData[stInputDataIdx.u32SegIdx].astSrc[stInputDataIdx.u32NodeIdx].u64VirAddr = pstFrmInfo->stVFrame.u64VirAddr[0];
    s_stNnieParam_.astSegData[stInputDataIdx.u32SegIdx].astSrc[stInputDataIdx.u32NodeIdx].u64PhyAddr = pstFrmInfo->stVFrame.u64PhyAddr[0];
    s_stNnieParam_.astSegData[stInputDataIdx.u32SegIdx].astSrc[stInputDataIdx.u32NodeIdx].u32Stride  = pstFrmInfo->stVFrame.u32Stride[0];

    /*NNIE process(process the 0-th segment)*/
    stProcSegIdx.u32SegIdx = 0;
    // if (TestMode) {
    //     struct timeval t0, t1;
    //     gettimeofday(&t0, NULL);
    //     s32Ret = NNIE_Forward(&s_stNnieParam_, &stInputDataIdx, &stProcSegIdx, HI_TRUE);
	   //  gettimeofday(&t1, NULL);
	   //  float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
	   //  cout << "Net forward time : " << cost_time << "ms" << endl;
    // } else {
        s32Ret = NNIE_Forward(&s_stNnieParam_, &stInputDataIdx, &stProcSegIdx, HI_TRUE);
    // }

    if (HI_SUCCESS != s32Ret) {
        // printf("Error, SAMPLE_SVP_NNIE_Forward failed %#x\n", s32Ret);
        printf("%s Error, SAMPLE_SVP_NNIE_Forward failed for %#x!", self_name, s32Ret);
        return s32Ret;
    }

    // if (TestMode) {
    //     struct timeval t0, t1;
    //     gettimeofday(&t0, NULL);
    //     s32Ret = NNIE_Get_Result(&s_stNnieParam_, output_tensors_);
	   //  gettimeofday(&t1, NULL);
	   //  float cost_time = (float)((t1.tv_sec*1000000 + t1.tv_usec) - (t0.tv_sec*1000000 + t0.tv_usec))/1000;
	   //  cout << "Get result time : " << cost_time << "ms" << endl;
    // } else {
        s32Ret = NNIE_Get_Result(&s_stNnieParam_, output_tensors_);
    // }

    if (HI_SUCCESS != s32Ret) {
        // printf("Error, NNIE_Get_Result failed %#x\n", s32Ret);
        printf("%s Error, NNIE_Get_Result failed for %#x!", self_name, s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

Tensor NnieNet::getOutputTensor(int index) {
    return output_tensors_[index];
}

void NnieNet::ShowModelInfo() {
    cout << "Model Info" << endl;
    cout << "-----------------------------------------------" << endl;
    int NetworkSegNum = s_stModel_.stModel.u32NetSegNum;
    cout << "NetworkSegNum : " << NetworkSegNum << endl;

    for (int i = 0; i < NetworkSegNum; i ++) {
        cout << "-----------------------------------------------" << endl;
        cout << "NetworkSeg : " << i << endl;

        int InputNodeNum = s_stModel_.stModel.astSeg[i].u16SrcNum;
        int OutputNodeNum = s_stModel_.stModel.astSeg[i].u16DstNum;
        int NetworkType = s_stModel_.stModel.astSeg[i].enNetType;
        string NetworkType_[3] = {"CNN", "CNN with ROI", "RNN or LSTM"};

        cout << "NetworkType : " << NetworkType_[NetworkType] << endl;
        cout << "InputNodeNum : " << InputNodeNum << endl;

        for (int j = 0; j < InputNodeNum; j ++) {
            string InputNodeType_[6] = {"S32", "U8", "YVU420SP", "YVU422SP", "VEC_S32", "SEQ_S32"};
            int InputNodeType = s_stModel_.stModel.astSeg[i].astSrcNode[j].enType;

            cout << "-----------------------" << endl;
            cout << "InputNode : " << j << endl;
            cout << "InputNodeType : " << InputNodeType_[InputNodeType] << "  ";
    		cout << "Inputwidth : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].unShape.stWhc.u32Width << "  ";
    		cout << "InputHeight : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].unShape.stWhc.u32Height << "  ";
    		cout << "InputChn : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].unShape.stWhc.u32Chn << endl;
    		// cout << "InputDim : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].unShape.u32Dim << endl;
    		cout << "InputNodeId : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].u32NodeId << "  ";
    		cout << "InputNodeName : " << s_stModel_.stModel.astSeg[i].astSrcNode[j].szName << endl;
        }

        cout << "OutputNodeNum : " << OutputNodeNum << endl;
        for (int j = 0; j < OutputNodeNum; j ++) {
            string OutputNodeType_[6] = {"S32", "U8", "YVU420SP", "YVU422SP", "VEC_S32", "SEQ_S32"};
            int OutputNodeType = s_stModel_.stModel.astSeg[i].astDstNode[j].enType;

            cout << "-----------------------" << endl;
            cout << "OutputNode : " << j << endl;
            cout << "OutputNodeType : " << OutputNodeType_[OutputNodeType] << "  ";
    		cout << "Outputwidth : " << s_stModel_.stModel.astSeg[i].astDstNode[j].unShape.stWhc.u32Width << "  ";
    		cout << "OutputHeight : " << s_stModel_.stModel.astSeg[i].astDstNode[j].unShape.stWhc.u32Height << "  ";
    		cout << "OutputChn : " << s_stModel_.stModel.astSeg[i].astDstNode[j].unShape.stWhc.u32Chn << endl;
    		// cout << "OutputDim : " << s_stModel_.stModel.astSeg[i].astDstNode[j].unShape.u32Dim << endl;
    		cout << "OutputNodeId : " << s_stModel_.stModel.astSeg[i].astDstNode[j].u32NodeId << "  ";
    		cout << "OutputNodeName : " << s_stModel_.stModel.astSeg[i].astDstNode[j].szName << endl;
        }
    }
}