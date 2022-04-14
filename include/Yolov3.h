/******************************************************************************
  File Name     : Yolov3.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : Yolov3 model header
******************************************************************************/

#ifndef YOLOV3_H
#define YOLOV3_H

#include "nnie_forward.h"
#include "Yolov3_layer.h"

#define DetectImgWidth 512
#define DetectImgHeight 288
// Yolov3模型
class YOLOV3 {
public:
	// yolov3 模型参数初始化
	YOLOV3(char* modelPath, float nms, float anchor);
	// yolov3 模型析构函数
	~YOLOV3();
	// yolov3 模型初始化
	bool Init();
	// yolov3 模型前向推理
	bool Forward(Mat& imgdata);
	// yolov3 模型后处理
	bool PostProcess();
	// yolov3 模型去初始化
	bool DeInit();
	// 最后的结果
	vector<Anchor> result;

private:
	NnieNet *yolov3;				// yolov3模型
	Yolov3_layer yolo_layer;		// yolo layer层

	int classNum;					// 检测类别
	char* model_path;				// 模型加载路径
	float nms_threshold;			// nms阈值
	float anchor_threshold;			// anchor 阈值

	// 非极大值抑制
	void NMS_cpu(vector<Anchor> boxes, vector<Anchor>& filterOutBoxes);
	void cleanBoxInBox();
};

#endif /* __YOLOV3_H__ */