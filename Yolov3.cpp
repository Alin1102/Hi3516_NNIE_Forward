/******************************************************************************
  File Name     : Yolov3.cpp
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : Yolov3 model source code
******************************************************************************/

#include "Yolov3.h"

// yolov3 模型参数初始化
YOLOV3::YOLOV3(char* modelPath, float nms, float anchor) {
	yolov3 = NULL;                      // yolov3模型
    classNum = 1;                       // 检测类别数量
    nms_threshold = nms;                // nms阈值
	anchor_threshold = anchor;             // anchor阈值
    yolo_layer.init(classNum, anchor_threshold);
    model_path = modelPath;       
}

// yolov3 模型析构函数
YOLOV3::~YOLOV3() {

}

// yolov3 模型初始化
bool YOLOV3::Init() {
	// NnieNet类声明定义
    printf("Begin To load Model: %s\n", model_path);
	yolov3 = new NnieNet(model_path);
	if (yolov3 == NULL) {
		printf("New a NnieNet failed !");
		return false;
	}
	
	// NnieNet初始化，加载模型，参数初始化
	HI_S32 s32Ret;
	s32Ret = yolov3->init();
	if (HI_SUCCESS != s32Ret) {
		return false;
    }

    yolov3->ShowModelInfo();
    return true;
}

// yolov3 模型前向推理
bool YOLOV3::Forward(Mat& imgdata) {
	bool ret;
	// 模型前向推理
    int isRgbOrder = 0;
    struct timeval timeBegin, timeEnd;
    
    gettimeofday(&timeBegin, NULL);
    yolov3->run(imgdata, isRgbOrder);
    gettimeofday(&timeEnd, NULL);
    float forwardTime = (float)((timeEnd.tv_sec * 1000.0 + timeEnd.tv_usec / 1000.0) - (timeBegin.tv_sec * 1000.0 + timeBegin.tv_usec / 1000.0));
    printf("Forward cost time: %.6f ms\n", forwardTime);


	// 后处理
	ret = PostProcess();
	if (ret == false) {
		return false;
	}
	
	return true;
}

// yolov3 模型后处理
bool YOLOV3::PostProcess() {
    vector<Tensor> output;
    output.push_back(yolov3->getOutputTensor(0));
    output.push_back(yolov3->getOutputTensor(1));
    output.push_back(yolov3->getOutputTensor(2));
    
    vector<Anchor> boxes;

    // 输出大于anchor_threshold的框，保存到boxes中
    yolo_layer.forward(DetectImgWidth, DetectImgHeight, output, boxes);


    printf("Befor nms box num: %d\n", boxes.size());
    // 进行NMS，得到result框
	NMS_cpu(boxes, result);


    printf("After nms box num: %d\n", result.size());
    // 清除大的框中包含的小的框
    cleanBoxInBox();

	return true;
}

bool YOLOV3::DeInit() {
	HI_S32 s32Ret;
	s32Ret = yolov3->deInit();
	if (HI_SUCCESS != s32Ret) {
		return false;
	}
	return true;
}

bool isBox1InBox2(Rect box1, Rect box2) {
    int center_x = (int)(box1.x + box1.width/2);
    int center_y = (int)(box1.y + box1.height/2);
    bool isInside = box2.contains(Point(center_x, center_y));   // box1的中心点在box2中
    if(isInside && box1.area() <= box2.area()) {
        return true;
    } else {
        return false;
    }
}

// 清除大的框中包含的小的框
void YOLOV3::cleanBoxInBox() {
    vector<bool> needRemove(result.size());
    for(int i = 0; i < result.size(); i++) {
        needRemove[i] = false;
    }

    for(int i = 0; i < result.size(); i++) {
        for(int j = 0; j < result.size(); j++) {
            if(i != j) {
                Rect box1 = result[i].box;
                Rect box2 = result[j].box;
                needRemove[i] = isBox1InBox2(box1, box2);
            }
        }
    }

    for(int i = result.size()-1; i >= 0; i--) {
        if(needRemove[i] == true) {
            result.erase(result.begin() + i);
            // cout << "Erase an box!!!!" << endl;
        }
    }
}

// 多类NMS，可支持单类
void YOLOV3::NMS_cpu(vector<Anchor> boxes, vector<Anchor>& filterOutBoxes) {
    filterOutBoxes.clear();
    if(boxes.size() == 0) {
        return;
    }
    std::vector<size_t> idx(boxes.size());
    for(unsigned i = 0; i < idx.size(); i++) {
        idx[i] = i;
    }

    // descending sort
    sort(boxes.begin(), boxes.end(), greater<Anchor>());

    while (idx.size() > 0) {
        int good_idx = idx[0];
        filterOutBoxes.push_back(boxes[good_idx]);

        vector<size_t> tmp = idx;
        idx.clear();
        for (unsigned i = 1; i < tmp.size(); i++) {
            int tmp_i = tmp[i];
            float inter_x1 = max( boxes[good_idx].box.x, boxes[tmp_i].box.x );
            float inter_y1 = max( boxes[good_idx].box.y, boxes[tmp_i].box.y );
            float inter_x2 = min( (boxes[good_idx].box.x + boxes[good_idx].box.width), (boxes[tmp_i].box.x + boxes[tmp_i].box.width));
            float inter_y2 = min( (boxes[good_idx].box.y + boxes[good_idx].box.height), (boxes[tmp_i].box.y + boxes[tmp_i].box.height));

            float w = max((inter_x2 - inter_x1 + 1), 0.0F);
            float h = max((inter_y2 - inter_y1 + 1), 0.0F);

            float inter_area = w * h;
            float area_1 = boxes[good_idx].box.area();
            float area_2 = boxes[tmp_i].box.area();

            float o = inter_area / (area_1 + area_2 - inter_area);

            if ( o <= nms_threshold || boxes[good_idx].classIndex != boxes[tmp_i].classIndex) {
                idx.push_back(tmp_i);
            }
        }
    }
}
