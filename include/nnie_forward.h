/******************************************************************************
  File Name     : nnie_forward.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : nnie_forward header, common utils for nnie
******************************************************************************/

#ifndef NNIE_FORWARD_H
#define NNIE_FORWARD_H

#include <iostream>
#include "hi_comm_sys.h"
#include <cmath>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"
#include "mpi_sys.h"
#include <sys/time.h>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM    2
#define SAMPLE_SVP_NNIE_QUANT_BASE 4096

/*16Byte align*/
#define SAMPLE_SVP_NNIE_ALIGN_16 16
#define SAMPLE_SVP_NNIE_ALIGN16(u32Num) ((u32Num + SAMPLE_SVP_NNIE_ALIGN_16-1) / SAMPLE_SVP_NNIE_ALIGN_16*SAMPLE_SVP_NNIE_ALIGN_16)
/*32Byte align*/
#define SAMPLE_SVP_NNIE_ALIGN_32 32
#define SAMPLE_SVP_NNIE_ALIGN32(u32Num) ((u32Num + SAMPLE_SVP_NNIE_ALIGN_32-1) / SAMPLE_SVP_NNIE_ALIGN_32*SAMPLE_SVP_NNIE_ALIGN_32)

#define SAMPLE_SVP_MMZ_FREE(phy,vir)\
do{\
	if ((0 != (phy)) && (0 != (vir)))\
	{\
		HI_MPI_SYS_MmzFree((phy),(void*)(vir));\
		(phy) = 0;\
		(vir) = 0;\
	}\
}while(0)

typedef struct Tensor_S {
    int n;
    int channel;
    int height;
    int width;
    int *data;
} Tensor;

typedef struct hiSAMPLE_SVP_NNIE_MODEL_S {
    SVP_NNIE_MODEL_S stModel;
    SVP_MEM_INFO_S stModelBuf;
} SAMPLE_SVP_NNIE_MODEL_S;

typedef struct hiSAMPLE_SVP_NNIE_SEG_DATA_S {
	SVP_SRC_BLOB_S astSrc[SVP_NNIE_MAX_INPUT_NUM];
	SVP_DST_BLOB_S astDst[SVP_NNIE_MAX_OUTPUT_NUM];
} SAMPLE_SVP_NNIE_SEG_DATA_S;

typedef struct hiSAMPLE_SVP_NNIE_PARAM_S {
    SVP_NNIE_MODEL_S* pstModel;
    HI_U32 u32TmpBufSize;
    HI_U32 au32TaskBufSize[SVP_NNIE_MAX_NET_SEG_NUM];
    SVP_MEM_INFO_S stTaskBuf;
	SVP_MEM_INFO_S stTmpBuf;
    SVP_MEM_INFO_S stStepBuf;
    SAMPLE_SVP_NNIE_SEG_DATA_S astSegData[SVP_NNIE_MAX_NET_SEG_NUM];
    SVP_NNIE_FORWARD_CTRL_S astForwardCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
	SVP_NNIE_FORWARD_WITHBBOX_CTRL_S astForwardWithBboxCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
} SAMPLE_SVP_NNIE_PARAM_S;

typedef struct hiSAMPLE_SVP_NNIE_CFG_S {
    HI_CHAR *pszPic;
    HI_U32 u32MaxInputNum;
    HI_U32 u32MaxRoiNum;
    HI_U64 au64StepVirAddr[2 * SVP_NNIE_MAX_NET_SEG_NUM];
	SVP_NNIE_ID_E aenNnieCoreId[SVP_NNIE_MAX_NET_SEG_NUM];
} SAMPLE_SVP_NNIE_CFG_S;

typedef struct hiSAMPLE_SVP_NNIE_BLOB_SIZE_S {
	HI_U32 au32SrcSize[SVP_NNIE_MAX_INPUT_NUM];
	HI_U32 au32DstSize[SVP_NNIE_MAX_OUTPUT_NUM];
} SAMPLE_SVP_NNIE_BLOB_SIZE_S;

typedef struct hiSAMPLE_SVP_NNIE_DATA_INDEX_S {
	HI_U32 u32SegIdx;
	HI_U32 u32NodeIdx;
} SAMPLE_SVP_NNIE_DATA_INDEX_S;

typedef SAMPLE_SVP_NNIE_DATA_INDEX_S  SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S;
typedef SAMPLE_SVP_NNIE_DATA_INDEX_S  SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S;

class NnieNet {
    public:
        NnieNet(char* model_file_);
        ~NnieNet();
        void ShowModelInfo();
        HI_S32 init();
        HI_S32 deInit();
        void run(const unsigned char *data);
        void run(const char *file_path, int rgbOrder);
        void run(Mat &img, int rgbOrder);
        HI_S32 run(VIDEO_FRAME_INFO_S* pstFrmInfo);
        Tensor getOutputTensor(int index);
        int TestMode;
        
    private:
        const char* model_file;
        char *self_name;

        SAMPLE_SVP_NNIE_MODEL_S s_stModel_ ;
        SAMPLE_SVP_NNIE_PARAM_S s_stNnieParam_ ;
        SAMPLE_SVP_NNIE_CFG_S   stNnieCfg_ ;
        Tensor *output_tensors_;

        HI_S32 loadModel();
        HI_S32 nnieParamInit(SAMPLE_SVP_NNIE_MODEL_S *s_stModel, SAMPLE_SVP_NNIE_CFG_S *stNnieCfg, SAMPLE_SVP_NNIE_PARAM_S *s_stNnieParam);
};

#endif /* __NNIE_FORWARD_H__ */