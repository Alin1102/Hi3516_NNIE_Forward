/******************************************************************************
  File Name     : AnchorGenerator.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : AnchorGenerator utils header
******************************************************************************/

#ifndef ANCHOR_GENERTOR_H
#define ANCHOR_GENERTOR_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <map>
using namespace std;
using namespace cv;

// Anchor cfg类
class AnchorCfg {
public:
	// Anchor cfg 缺省构造函数
	AnchorCfg() {}
	// Anchor cfg 析构函数
	~AnchorCfg() {}
	// Anchor cfg 构造函数
	AnchorCfg(const std::vector<float> s, const std::vector<float> r, int size) {
		SCALES = s;
		RATIOS = r;
		BASE_SIZE = size;
	}

	std::vector<float> SCALES;
	std::vector<float> RATIOS;
	int BASE_SIZE;
};


// 自定义矩形类 x1, y1, x2, y2定义
class CRect2f {
public:
	// 构造函数
	CRect2f(float x1, float y1, float x2, float y2) {
	    val[0] = x1;
	    val[1] = y1;
	    val[2] = x2;
	    val[3] = y2;
	}

	// 重载[]运算符
	float& operator[](int i) {
		return val[i];
	}

	// 重载[]运算符
	float operator[](int i) const {
		return val[i];
	}

	// 输出
	void print() {
		printf("rect %f %f %f %f\n", val[0], val[1], val[2], val[3]);
	}
	
	float val[4];
};


// Anchor
class Anchor {
public:
	// Anchor构造函数
	Anchor() {
	}

	Anchor(Rect box_, float score_, int classIndex_) {
        box = box_;
        score = score_;
		classIndex = classIndex_;
    }

	// Anchor析构函数
	~Anchor() {
	}

	bool operator<(const Anchor &t) const {
	    return score < t.score;
	}

	bool operator>(const Anchor &t) const {
	    return score > t.score;
	}

	// 重载[]运算符
	float& operator[](int i) {
	    if (i == 0) return finalbox.x;
	    if (i == 1) return finalbox.y;
	    if (i == 2) return finalbox.width;
	    if (i == 3) return finalbox.height;
	}

	// 重载[]运算符
	float operator[](int i) const {
	    if (i == 0) return finalbox.x;
	    if (i == 1) return finalbox.y;
	    if (i == 2) return finalbox.width;
	    if (i == 3) return finalbox.height;
	}

	// 输出
	void print() {
	    printf("finalbox x1:%f y1:%f x2:%f y2:%f, score %f\n", finalbox.x, finalbox.y, finalbox.width, finalbox.height, score);
	    printf("landmarks ");
	    for (int i = 0; i < pts.size(); ++i) {
			printf("%f %f, ", pts[i].x, pts[i].y);
	    }
	    printf("\n");
	}

	float score; 

	// 人脸检测
	cv::Rect2f anchor; 				// x1,y1,x2,y2
	float reg[4]; 					// offset reg
	cv::Point center; 				// anchor feat center
	vector<cv::Point2f> pts; 		// pred pts
	cv::Rect2f finalbox;			// final box res

	// 头肩检测
	Rect box;
	int classIndex;
};


// Anchor生成类
class AnchorGenerator {
public:
	// Anchor生成类 构造函数
	AnchorGenerator();

	// Anchor生成类 析构函数
	~AnchorGenerator();

	// init different anchors
	int Init(int stride, const AnchorCfg& cfg, bool dense_anchor);

	// anchor plane
	int Generate(int fwidth, int fheight, int stride, float step, vector<int>& size, vector<float>& ratio, bool dense_anchor);

	// filter anchors and return valid anchors
	// int FilterAnchor(const caffe::Blob<float>* cls, const caffe::Blob<float>* reg, const caffe::Blob<float>* pts, std::vector<Anchor>& result);

	// Anchor过滤
	int FilterAnchor(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, float confidence_threshold, int w, int h);

	// Anchor过滤
	int FilterAnchor(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, float ratio_w, float ratio_h, float confidence_threshold, int w, int h);

	// 优化的Anchor过滤
	int FilterAnchor_Opt(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, int *confidence_threshold, float * constDiv4096);

private:
	// 根据ratio生成anchor
	void _ratio_enum(const CRect2f& anchor, const vector<float>& ratios, vector<CRect2f>& ratio_anchors);

	// 根据scale生成anchor
	void _scale_enum(const vector<CRect2f>& ratio_anchor, const vector<float>& scales, vector<CRect2f>& scale_anchors);

	// 得到最后的box  是左上角坐标与左下角坐标
	void bbox_pred(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box);

	inline void bbox_pred_Opt_a0(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box, int &i, int &j);
	inline void bbox_pred_Opt_a1(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box, int &i, int &j);

	// 得到在原图中的landmark  delta是对于设定锚点的偏移量
	void landmark_pred(const CRect2f anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts);

	inline void landmark_pred_Opt_a0(const CRect2f& anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts, int &i, int &j);
	inline void landmark_pred_Opt_a1(const CRect2f& anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts, int &i, int &j);

	vector<vector<Anchor>> anchor_planes; // corrspont to channels

	vector<int> anchor_size;
	vector<float> anchor_ratio;
	float anchor_step; 				// scale step
	int anchor_stride; 				// anchor tile stride
	int feature_w; 					// feature map width
	int feature_h; 					// feature map height

	vector<CRect2f> preset_anchors;
	int anchor_num; 				// anchor type num

	float ratiow = 1.0;
	float ratioh = 1.0;

	// 优化使用变量
	// int **idPos;

	// int stepConst;
	
	// int **clsDataPos_a0;
	// int **clsDataPos_a1;
	
	// int **boxPoint_a0;
	// int **boxPoint_a1;

	// int **pred_w_a0;
	// int **pred_h_a0;
	// int **pred_x_ctr_a0;
	// int **pred_y_ctr_a0;
	// int **pred_w_a1;
	// int **pred_h_a1;
	// int **pred_x_ctr_a1;
	// int **pred_y_ctr_a1;
	
	// int ***regData_a0;
	// int ***regData_a1;

	// int ***ptsData_a0_x;
	// int ***ptsData_a0_y;
	// int ***ptsData_a1_x;
	// int ***ptsData_a1_y;

	// int cls_width;
	// int cls_height;
};

#endif  /* __ANCHOR_GENERTOR_H__ */
