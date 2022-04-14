/******************************************************************************
  File Name     : QRDetector.cpp
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : QRDetector module source code
******************************************************************************/
#include "nnie_forward.h"
#include "AnchorGenerator.h"
#include "opencv2/opencv.hpp"
#include "Yolov3_layer.h"
#include "Yolov3.h"
#include "Value_Pos.h"

#include <dirent.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <functional>
#include <sys/time.h>
#include <cstdlib>

using namespace std;
using namespace cv;

#define MAX_DETECT_NUM 200
#define UTILS_MIN(a,b)  (((a) < (b)) ? (a) : (b))

int main(int argc, char** argv) {
    bool ret;
    if(argc != 5) {
        printf("Usage: %s modelPath nms_threshold confidence imagePath\n", argv[0]);
        return -1;
    }

    char* modelPath = argv[1];
    float nms_threshold = atof(argv[2]);
    float anchor_threshold = atof(argv[3]);

    printf("Load NNIE Model ...\n");
    YOLOV3 *yolov3 = new YOLOV3(modelPath, nms_threshold, anchor_threshold);
    if (yolov3 == NULL) {
        printf("new a Yolo Detect failed !\n");
        return -1;
    }

    // 模型初始化
    ret = yolov3->Init();
    if (ret == false) {
        printf("Yolo Init failed !\n");
        return -1;
    }

    string fileName = string(argv[4]);
    int fileNameLength = fileName.length();
    string outFile = fileName.substr(0, fileNameLength-4) + ".txt";       // 预测结果标签保存路径
    string resultImgPath = fileName.substr(0, fileNameLength-4) + ".jpg"; // 预测结果图片保存路径

    printf("Begin process File: %s\n", fileName.c_str());

    Mat img = imread(fileName.c_str());
    int imgWidth = img.cols;
    int imgHeight = img.rows;
    printf("Img width: %d, Height: %d\n", imgWidth, imgHeight);

    Mat resizeImg;
    cv::resize(img, resizeImg, cv::Size(DetectImgWidth, DetectImgHeight));
    printf("Resize Img to %d*%d\n", resizeImg.cols, resizeImg.rows);

    // 用于根据检测框置信度排序
    vector<Anchor> detectResultVecTmp;
    vector<Value_Pos<float>> scoreVec;

    yolov3->Forward(resizeImg);
    
    detectResultVecTmp.clear();
    scoreVec.clear();

    for(int i = 0, pos = 0; i < yolov3->result.size(); i++) {
        detectResultVecTmp.push_back(yolov3->result[i]);
        scoreVec.push_back(Value_Pos<float>(yolov3->result[i].score, pos++));
    }


    // 如果二维码框大于30张，则按检测的置信度进行排序，取置信度最高的30个
    if (detectResultVecTmp.size() >= MAX_DETECT_NUM) {
        sort(scoreVec.begin(), scoreVec.end(), cmpLarge2Small<float>);
    }
    int detect_num = UTILS_MIN(MAX_DETECT_NUM, scoreVec.size());

    // 将预测框的结果 画在原图上并将预测框保存到txt文件中。
    FILE* fpt = fopen(outFile.c_str(), "w");
    for(int i = 0; i < detect_num; i++) {
        int p = scoreVec[i].pos;

        int xmin = detectResultVecTmp[p].box.x * imgWidth / DetectImgWidth;
        int ymin = detectResultVecTmp[p].box.y * imgHeight / DetectImgHeight;
        int width = detectResultVecTmp[p].box.width * imgWidth / DetectImgWidth;
        int height = detectResultVecTmp[p].box.height * imgHeight / DetectImgHeight;
        char scoreBuf[200];
        sprintf(scoreBuf, "%.6f", scoreVec[i].value);

        Rect curBox(xmin, ymin, width, height);
        cv::rectangle(img, curBox, Scalar(255, 0, 0), 1, LINE_8, 0);
        cv::putText(img, string(scoreBuf), cv::Point(xmin, ymin), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));

        int classId = detectResultVecTmp[p].classIndex;
        float boxX = (detectResultVecTmp[p].box.x + detectResultVecTmp[p].box.width * 0.5) * 1.0 / imgWidth;
        float boxY = (detectResultVecTmp[p].box.y + detectResultVecTmp[p].box.height * 0.5) * 1.0 / imgHeight;
        float boxWidth = detectResultVecTmp[p].box.width * 1.0  / imgWidth;
        float boxHeight = detectResultVecTmp[p].box.height * 1.0 / imgHeight;

        fprintf(fpt, "%d %.6f %.6f %.6f %.6f\n", classId, boxX, boxY, boxWidth, boxHeight);
    }
    fclose(fpt);
    detectResultVecTmp.clear();
    scoreVec.clear();

    // 保存预测图片。
    cv::imwrite(resultImgPath.c_str(), img);

    return 0;
}