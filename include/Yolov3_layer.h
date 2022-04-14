/******************************************************************************
  File Name     : Yolov3_layer.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : Yolov3 model header post process layer
******************************************************************************/

#ifndef YOLOV3_LAYER_H
#define YOLOV3_LAYER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <functional>

#include "nnie_forward.h"
#include "AnchorGenerator.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Yolov3_layer {
    public:
        Yolov3_layer();
        void init(int classNum, float confThres);
        // void forward(vector<Tensor> &feature, vector<Anchor> &boxes);
        void forward(int img_width, int img_height, vector<Tensor> &feature, vector<Anchor> &boxes);
    private:
        vector<vector<Size2f>> Anchors;
        int ClassNum;
        int BoxNum;
        int ScaleNum;
        int ConfThresInt;
        float ConfThres;
};

#endif /* __YOLOV3_LAYER_H__ */