/******************************************************************************
  File Name     : Yolov3_layer.cpp
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : Yolov3 model post process layer source code
******************************************************************************/

#include "Yolov3_layer.h"

float Sigmoid(float x) {
    return 1.0 / (1.0 + exp(-x));
}

// 多类检测时使用
void Softmax(vector<float> &classes) {
    float sum = 0;
    std::transform(classes.begin(), classes.end(), classes.begin(),
                    [&sum](float score) -> float {
                        float exp_score = exp(score/4096.0);
                        sum += exp_score;
                        return exp_score;
                    });
    std::transform(classes.begin(), classes.end(), classes.begin(),
                    [sum](float score) -> float { return score / sum; });
}

Yolov3_layer::Yolov3_layer() {
    BoxNum = 3;
    ScaleNum = 3;
    ClassNum = 1;
    ConfThres = 0.2;
    // COCO anchors
    Anchors = {{{10,13}, {16,30}, {33,23}}, {{30,61}, {62,45}, {59,119}}, {{116,90}, {156,198}, {373,326}}};

}

void Yolov3_layer::init(int classNum, float confThres) {
    ClassNum = classNum;
    ConfThres = confThres;
    ConfThresInt = log(confThres/(1 - confThres)) * 4096;
}

void Yolov3_layer::forward(int img_width, int img_height, vector<Tensor> &feature, vector<Anchor> &boxes) {
    for (int i = 0; i < ScaleNum; i ++) {
        Tensor feature_tmp = feature[i];
        vector<Size2f> anchors = Anchors[i];

        int Box_h = feature[i].height;
        int Box_w = feature[i].width;

        int Box_length = Box_h * Box_w;

        // 新版YOLOV3后处理
        int Stride = img_width / Box_w;
        
        for (int cy = 0; cy < Box_h; cy++) {
            for (int cx = 0; cx < Box_w; cx++) {
                for (int box = 0; box < BoxNum; box++) {
                    int channel = box * (ClassNum + 5);

                    int tc = feature_tmp.data[cx + cy * Box_w + Box_length * (channel + 4)];
                    if (tc > ConfThresInt) {
                        float confidence = Sigmoid(float(tc)/4096.0);
                        int tx = feature_tmp.data[cx + cy * Box_w + channel * Box_length];
                        int ty = feature_tmp.data[cx + cy * Box_w + (channel + 1) * Box_length];
                        int tw = feature_tmp.data[cx + cy * Box_w + (channel + 2) * Box_length];
                        int th = feature_tmp.data[cx + cy * Box_w + (channel + 3) * Box_length];


                        // YOLOV5后处理
                        float x = (Sigmoid(float(tx)/4096.0) * 2.0 - 0.5 + cx) * Stride;
                        float y = (Sigmoid(float(ty)/4096.0) * 2.0 - 0.5 + cy) * Stride;
                        float w = pow(Sigmoid(float(tw)/4096.0) * 2.0, 2) * anchors[box].width;
                        float h = pow(Sigmoid(float(th)/4096.0) * 2.0, 2) * anchors[box].height;                        

                        int index = 0;
                        if (ClassNum > 1) {
                            vector<float> classes(ClassNum);
                            for (int i = 0; i < ClassNum; ++i) {
                                classes[i] = feature_tmp.data[cx + cy * Box_w + (channel + 5 + i) * Box_length];
                            }
                            auto max_itr = std::max_element(classes.begin(), classes.end());
                            index = static_cast<int>(max_itr - classes.begin());
                            Softmax(classes);
                            confidence = confidence * classes[index];
                        }


                        // YOLOV5后处理
                        int center_x = (int)(x);
                        int center_y = (int)(y);
                        int width = (int)(w);
                        int height = (int)(h);
                        int left = static_cast<int>(center_x - width / 2);
                        int top = static_cast<int>(center_y - height / 2);

                        Rect tmp(left, top, width, height);
                        Anchor temp(tmp, confidence, index);
                        boxes.push_back(temp);
                    }
                }
            }
        }
    }
}