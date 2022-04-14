/******************************************************************************
  File Name     : AnchorGenerator.h
  Version       : Initial
  Author        : TD1500
  Created       : 2020
  Description   : AnchorGenerator utils header
******************************************************************************/

#include "AnchorGenerator.h"

// Anchor生成类 构造函数
AnchorGenerator::AnchorGenerator() {
	anchor_stride = 0;

	// idPos = NULL;
	
	// clsDataPos_a0 = NULL;
	// clsDataPos_a1 = NULL;
	
	// boxPoint_a0 = NULL;
	// boxPoint_a1 = NULL;
	
	// regData_a0 = NULL;
	// regData_a1 = NULL;

	// ptsData_a0_x = NULL;
	// ptsData_a0_y = NULL;
	// ptsData_a1_x = NULL;
	// ptsData_a1_y = NULL;
}

// Anchor生成类 析构函数
AnchorGenerator::~AnchorGenerator() {
	// if (idPos != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] idPos[i];
	// 	}
	// 	delete [] idPos;
	// }
	// idPos = NULL;

	// if (clsDataPos_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] clsDataPos_a0[i];
	// 	}
	// 	delete [] clsDataPos_a0;
	// }
	// clsDataPos_a0 = NULL;

	// if (clsDataPos_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {	
	// 		delete [] clsDataPos_a1[i];
	// 	}
	// 	delete [] clsDataPos_a1;
	// }
	// clsDataPos_a1 = NULL;

	// if (boxPoint_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] boxPoint_a0[i];
	// 	}
	// 	delete [] boxPoint_a0;
	// }
	// boxPoint_a0 = NULL;

	// if (boxPoint_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] boxPoint_a1[i];
	// 	}
	// 	delete [] boxPoint_a1;
	// }
	// boxPoint_a1 = NULL;

	// if (pred_w_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_w_a0[i];
	// 	}
	// 	delete [] pred_w_a0;
	// }
	// pred_w_a0 = NULL;

	// if (pred_h_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_h_a0[i];
	// 	}
	// 	delete [] pred_h_a0;
	// }
	// pred_h_a0 = NULL;

	// if (pred_x_ctr_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_x_ctr_a0[i];
	// 	}
	// 	delete [] pred_x_ctr_a0;
	// }
	// pred_x_ctr_a0 = NULL;

	// if (pred_y_ctr_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_y_ctr_a0[i];
	// 	}
	// 	delete [] pred_y_ctr_a0;
	// }
	// pred_y_ctr_a0 = NULL;

	// if (pred_w_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_w_a1[i];
	// 	}
	// 	delete [] pred_w_a1;
	// }
	// pred_w_a1 = NULL;

	// if (pred_h_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_h_a1[i];
	// 	}
	// 	delete [] pred_h_a1;
	// }
	// pred_h_a1 = NULL;

	// if (pred_x_ctr_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_x_ctr_a1[i];
	// 	}
	// 	delete [] pred_x_ctr_a1;
	// }
	// pred_x_ctr_a1 = NULL;

	// if (pred_y_ctr_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		delete [] pred_y_ctr_a1[i];
	// 	}
	// 	delete [] pred_y_ctr_a1;
	// }
	// pred_y_ctr_a1 = NULL;

	// if (regData_a0 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] regData_a0[i][j];
	// 		}
	// 		delete [] regData_a0[i];
	// 	}
	// 	delete [] regData_a0;
	// }
	// regData_a0 = NULL;

	// if (regData_a1 != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] regData_a1[i][j];
	// 		}
	// 		delete [] regData_a1[i];
	// 	}
	// 	delete [] regData_a1;
	// }
	// regData_a1 = NULL;

	// if (ptsData_a0_x != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] ptsData_a0_x[i][j];
	// 		}
	// 		delete [] ptsData_a0_x[i];
	// 	}
	// 	delete [] ptsData_a0_x;
	// }
	// ptsData_a0_x = NULL;

	// if (ptsData_a0_y != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] ptsData_a0_y[i][j];
	// 		}
	// 		delete [] ptsData_a0_y[i];
	// 	}
	// 	delete [] ptsData_a0_y;
	// }
	// ptsData_a0_y = NULL;

	// if (ptsData_a1_x != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] ptsData_a1_x[i][j];
	// 		}
	// 		delete [] ptsData_a1_x[i];
	// 	}
	// 	delete [] ptsData_a1_x;
	// }
	// ptsData_a1_x = NULL;

	// if (ptsData_a1_y != NULL) {
	// 	for(int i = 0; i < cls_width; i++) {
	// 		for(int j = 0; j < cls_height; j++) {
	// 			delete [] ptsData_a1_y[i][j];
	// 		}
	// 		delete [] ptsData_a1_y[i];
	// 	}
	// 	delete [] ptsData_a1_y;
	// }
	// ptsData_a1_y = NULL;
}

// init different anchors
int AnchorGenerator::Init(int stride, const AnchorCfg& cfg, bool dense_anchor) {
	CRect2f base_anchor(0, 0, cfg.BASE_SIZE - 1, cfg.BASE_SIZE - 1);   //都是(0,0,15,15)
	vector<CRect2f> ratio_anchors;
	// get ratio anchors
	_ratio_enum(base_anchor, cfg.RATIOS, ratio_anchors);
	_scale_enum(ratio_anchors, cfg.SCALES, preset_anchors);

	anchor_stride = stride;

	anchor_num = preset_anchors.size();

	// 优化使用变量 初始化
	// int w = 0, h = 0;
	// if (anchor_stride == 32) {
	// 	w = 40;
	// 	h = 23;
	// } else if (anchor_stride == 16) {
	// 	w = 80;
	// 	h = 45;
	// } else if (anchor_stride == 8) {
	// 	w = 160;
	// 	h = 90;
	// }

	// cls_width = w;
	// cls_height = h;
	// stepConst = w * h;

	// idPos 			= new int* [w];
	// clsDataPos_a0 	= new int* [w];
	// clsDataPos_a1 	= new int* [w];
	// boxPoint_a0 	= new int* [w];
	// boxPoint_a1 	= new int* [w];
	// regData_a0 		= new int** [w];
	// regData_a1 		= new int** [w];
	// ptsData_a0_x 	= new int** [w];
	// ptsData_a0_y 	= new int** [w];
	// ptsData_a1_x 	= new int** [w];
	// ptsData_a1_y 	= new int** [w];

	// for(int i = 0; i < w; i++) {
	// 	idPos[i] 			= new int [h];
	// 	clsDataPos_a0[i] 	= new int [h];
	// 	clsDataPos_a1[i] 	= new int [h];
	// 	regData_a0[i] 		= new int* [h];
	// 	regData_a1[i] 		= new int* [h];
	// 	ptsData_a0_x[i] 	= new int* [h];
	// 	ptsData_a0_y[i] 	= new int* [h];
	// 	ptsData_a1_x[i] 	= new int* [h];
	// 	ptsData_a1_y[i] 	= new int* [h];
	// 	for(int j = 0; j < h; j++) {
	// 		idPos[i][j] = j * w + i;
	// 		clsDataPos_a0[i][j] = (anchor_num + 0) * stepConst + idPos[i][j];
	// 		clsDataPos_a1[i][j] = (anchor_num + 1) * stepConst + idPos[i][j];

	// 		regData_a0[i][j] = new int [4];
	// 		regData_a1[i][j] = new int [4];
	// 		for(int p = 0; p < 4; p++) {
	// 			regData_a0[i][j][p] = (0 * 4 + p) * stepConst + idPos[i][j];
	// 			regData_a1[i][j][p] = (1 * 4 + p) * stepConst + idPos[i][j];
	// 		}

	// 		ptsData_a0_x[i][j] = new int [5];
	// 		ptsData_a0_y[i][j] = new int [5];
	// 		ptsData_a1_x[i][j] = new int [5];
	// 		ptsData_a1_y[i][j] = new int [5];
	// 		for(int p = 0; p < 5; p++) {
	// 			ptsData_a0_x[i][j][p] = (0 * 10 + p * 2) * stepConst + idPos[i][j];
	// 			ptsData_a0_y[i][j][p] = (0 * 10 + p * 2 + 1) * stepConst + idPos[i][j];
	// 			ptsData_a1_x[i][j][p] = (1 * 10 + p * 2) * stepConst + idPos[i][j];
	// 			ptsData_a1_y[i][j][p] = (1 * 10 + p * 2 + 1) * stepConst + idPos[i][j];
	// 		}

	// 	}
	// 	boxPoint_a0[i] = new int [4];
	// 	boxPoint_a1[i] = new int [4];
	// 	for(int p = 0; p < 4; p++) {
	// 		boxPoint_a0[i][p] = i * anchor_stride + preset_anchors[0][p];
	// 		boxPoint_a1[i][p] = i * anchor_stride + preset_anchors[1][p];
	// 	}
	// }

	// pred_w_a0 		= new int* [w];
	// pred_h_a0 		= new int* [w];
	// pred_x_ctr_a0 	= new int* [w];
	// pred_y_ctr_a0 	= new int* [w];
	// pred_w_a1 		= new int* [w];
	// pred_h_a1 		= new int* [w];
	// pred_x_ctr_a1 	= new int* [w];
	// pred_y_ctr_a1 	= new int* [w];
	// for(int i = 0; i < w; i++) {
	// 	pred_w_a0[i] 		= new int [h];
	// 	pred_h_a0[i] 		= new int [h];
	// 	pred_x_ctr_a0[i] 	= new int [h];
	// 	pred_y_ctr_a0[i] 	= new int [h];
	// 	pred_w_a1[i] 		= new int [h];
	// 	pred_h_a1[i] 		= new int [h];
	// 	pred_x_ctr_a1[i] 	= new int [h];
	// 	pred_y_ctr_a1[i] 	= new int [h];
	// 	for(int j = 0; j < h; j++) {
	// 		pred_w_a0[i][j] = boxPoint_a0[i][2] - boxPoint_a0[i][0] + 1;
	// 		pred_h_a0[i][j] = boxPoint_a0[j][3] - boxPoint_a0[j][1] + 1;
	// 		pred_x_ctr_a0[i][j] = boxPoint_a0[i][0] + 0.5 * (pred_w_a0[i][j] - 1);
	// 		pred_y_ctr_a0[i][j] = boxPoint_a0[j][1] + 0.5 * (pred_h_a0[i][j] - 1);

	// 		pred_w_a1[i][j] = boxPoint_a1[i][2] - boxPoint_a1[i][0] + 1;
	// 		pred_h_a1[i][j] = boxPoint_a1[j][3] - boxPoint_a1[j][1] + 1;
	// 		pred_x_ctr_a1[i][j] = boxPoint_a1[i][0] + 0.5 * (pred_w_a1[i][j] - 1);
	// 		pred_y_ctr_a1[i][j] = boxPoint_a1[j][1] + 0.5 * (pred_h_a1[i][j] - 1);
	// 	}
	// }

	return anchor_num;
}

// Anchor过滤
int AnchorGenerator::FilterAnchor(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, float confidence_threshold, int w, int h) {
	int pts_length = 5;
	int step = h * w;
	int threshold = (int)(confidence_threshold * 4096);

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			int id = j * w + i;
			for (int a = 0; a < anchor_num; ++a) {
				if(clsData[ (anchor_num + a) * step + id ] > threshold) {
					CRect2f box(i * anchor_stride + preset_anchors[a][0],
								j * anchor_stride + preset_anchors[a][1],
								i * anchor_stride + preset_anchors[a][2],
								j * anchor_stride + preset_anchors[a][3]);

					//printf("box::%f %f %f %f\n", box[0], box[1], box[2], box[3]);
					CRect2f delta(regData[(a * 4 + 0) * step + id] / 4096.0,
								  regData[(a * 4 + 1) * step + id] / 4096.0,
								  regData[(a * 4 + 2) * step + id] / 4096.0,
								  regData[(a * 4 + 3) * step + id] / 4096.0);

					Anchor res;
					res.anchor = cv::Rect2f(box[0], box[1], box[2], box[3]);
					bbox_pred(box, delta, res.finalbox);
					res.score = clsData[(anchor_num + a) * step + id] / 4096.0;
					res.center = cv::Point(i, j);
					//printf("center %d %d\n", i, j);

					vector<cv::Point2f> pts_delta(pts_length);
					for (int p = 0; p < pts_length; ++p) {
						pts_delta[p].x = ptsData[a * pts_length * 2 + p * 2 + id] / 4096.0;
						pts_delta[p].y = ptsData[a * pts_length * 2 + p * 2 + 1 + id] / 4096.0;
					}                   
					landmark_pred(box, pts_delta, res.pts);

					result.push_back(res);
				}
			}
		}
	}
	return 0;
}

// Anchor过滤
int AnchorGenerator::FilterAnchor(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, float ratio_w, float ratio_h, float confidence_threshold, int w, int h) {
	ratiow = ratio_w;
	ratioh = ratio_h;
	int pts_length = 5;
	int threshold = (int)(confidence_threshold * 4096);
	int step = h * w;

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			int id = j * w + i;
			for (int a = 0; a < anchor_num; ++a) {
				if(clsData[(anchor_num + a) * step + id] > threshold) {
					// std::cout <<"score: "<< clsData[(anchor_num + a)*step + id] << std::endl;
					CRect2f box(i * anchor_stride + preset_anchors[a][0],
								j * anchor_stride + preset_anchors[a][1],
								i * anchor_stride + preset_anchors[a][2],
								j * anchor_stride + preset_anchors[a][3]);

					CRect2f delta(regData[(a * 4 + 0) * step + id] / 4096.0,
								  regData[(a * 4 + 1) * step + id] / 4096.0,
								  regData[(a * 4 + 2) * step + id] / 4096.0,
								  regData[(a * 4 + 3) * step + id] / 4096.0);
					Anchor res;
					res.anchor = cv::Rect2f(box[0], box[1], box[2], box[3]);
					bbox_pred(box, delta, res.finalbox);
					res.score = clsData[(anchor_num + a) * step + id] / 4096.0;
					res.center = cv::Point(i, j);

					vector<cv::Point2f> pts_delta(pts_length);
					for (int p = 0; p < pts_length; ++p) {
						pts_delta[p].x = ptsData[(a * 10 + p * 2) * step + id] / 4096.0;
						pts_delta[p].y = ptsData[(a * 10 + p * 2 + 1) * step + id] / 4096.0;
					}                     
					landmark_pred(box, pts_delta, res.pts);

					result.push_back(res);
				}
			}
		}
	}

	return 0;
}

// // Anchor过滤 优化
// int AnchorGenerator::FilterAnchor_Opt(const int* clsData, const int* regData, const int* ptsData, vector<Anchor>& result, int *confidence_threshold, float *constDiv4096) {
// 	ratiow = 1;
// 	ratioh = 1;
// 	int pts_length = 5;

// 	#pragma omp parallel
// 	#pragma omp for
// 	for (int i = 0; i < cls_width; ++i) {
// 		for (int j = 0; j < cls_height; ++j) {
// 			// a = 0
// 			if (clsData[clsDataPos_a0[i][j]] > *confidence_threshold) {
// 				CRect2f box(boxPoint_a0[i][0], boxPoint_a0[j][1], boxPoint_a0[i][2], boxPoint_a0[j][3]);
// 				CRect2f delta(regData[regData_a0[i][j][0]] * (*constDiv4096),
// 							  regData[regData_a0[i][j][1]] * (*constDiv4096),
// 						      regData[regData_a0[i][j][2]] * (*constDiv4096),
// 							  regData[regData_a0[i][j][3]] * (*constDiv4096));
// 				Anchor res;
// 				res.anchor = cv::Rect2f(box[0], box[1], box[2], box[3]);
// 				bbox_pred_Opt_a0(box, delta, res.finalbox, i, j);
// 				res.score = clsData[clsDataPos_a0[i][j]] * (*constDiv4096);
// 				res.center = cv::Point(i, j);

// 				vector<cv::Point2f> pts_delta(pts_length);
// 				for (int p = 0; p < pts_length; ++p) {
// 					pts_delta[p].x = ptsData[ptsData_a0_x[i][j][p]] * (*constDiv4096);
// 					pts_delta[p].y = ptsData[ptsData_a0_y[i][j][p]] * (*constDiv4096);
// 				}                     
// 				landmark_pred_Opt_a0(box, pts_delta, res.pts, i, j);

// 				result.push_back(res);
// 			}

// 			// a = 1
// 			if (clsData[clsDataPos_a1[i][j]] > *confidence_threshold) {
// 				CRect2f box(boxPoint_a1[i][0], boxPoint_a1[j][1], boxPoint_a1[i][2], boxPoint_a1[j][3]);
// 				CRect2f delta(regData[regData_a1[i][j][0]] * (*constDiv4096),
// 							  regData[regData_a1[i][j][1]] * (*constDiv4096),
// 						      regData[regData_a1[i][j][2]] * (*constDiv4096),
// 							  regData[regData_a1[i][j][3]] * (*constDiv4096));
// 				Anchor res;
// 				res.anchor = cv::Rect2f(box[0], box[1], box[2], box[3]);
// 				bbox_pred_Opt_a1(box, delta, res.finalbox, i, j);
// 				res.score = clsData[clsDataPos_a1[i][j]] * (*constDiv4096);
// 				res.center = cv::Point(i, j);

// 				vector<cv::Point2f> pts_delta(pts_length);
// 				for (int p = 0; p < pts_length; ++p) {
// 					pts_delta[p].x = ptsData[ptsData_a1_x[i][j][p]] * (*constDiv4096);
// 					pts_delta[p].y = ptsData[ptsData_a1_y[i][j][p]] * (*constDiv4096);
// 				}                     
// 				landmark_pred_Opt_a1(box, pts_delta, res.pts, i, j);

// 				result.push_back(res);
// 			}
// 		}
// 	}

// 	// printf("anchor num : %d\n", result.size());

// 	return 0;
// }

// 根据ratio生成anchor
void AnchorGenerator::_ratio_enum(const CRect2f& anchor, const vector<float>& ratios, vector<CRect2f>& ratio_anchors) {
	float w = anchor[2] - anchor[0] + 1;
	float h = anchor[3] - anchor[1] + 1;
	float x_ctr = anchor[0] + 0.5 * (w - 1);
	float y_ctr = anchor[1] + 0.5 * (h - 1);

	ratio_anchors.clear();
	float sz = w * h;
	for (int s = 0; s < ratios.size(); ++s) {
		float r = ratios[s];
		float size_ratios = sz / r;
		float ws = std::sqrt(size_ratios);
		float hs = ws * r;
		ratio_anchors.push_back(CRect2f(x_ctr - 0.5 * (ws - 1),
										y_ctr - 0.5 * (hs - 1),
										x_ctr + 0.5 * (ws - 1),
										y_ctr + 0.5 * (hs - 1)));
	}
}

// 根据scale生成anchor
void AnchorGenerator::_scale_enum(const vector<CRect2f>& ratio_anchor, const vector<float>& scales, vector<CRect2f>& scale_anchors) {
	scale_anchors.clear();
	for (int a = 0; a < ratio_anchor.size(); ++a) {
		CRect2f anchor = ratio_anchor[a];
		float w = anchor[2] - anchor[0] + 1;
		float h = anchor[3] - anchor[1] + 1;
		float x_ctr = anchor[0] + 0.5 * (w - 1);
		float y_ctr = anchor[1] + 0.5 * (h - 1);

		for (int s = 0; s < scales.size(); ++s) {
			float ws = w * scales[s];
			float hs = h * scales[s];
			scale_anchors.push_back(CRect2f(x_ctr - 0.5 * (ws - 1),
											y_ctr - 0.5 * (hs - 1),
											x_ctr + 0.5 * (ws - 1),
											y_ctr + 0.5 * (hs - 1)));
		}
	}
}

// 得到最后的box  是左上角坐标与左下角坐标
void AnchorGenerator::bbox_pred(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box) {
	float w = anchor[2] - anchor[0] + 1;
	float h = anchor[3] - anchor[1] + 1;
	float x_ctr = anchor[0] + 0.5 * (w - 1);
	float y_ctr = anchor[1] + 0.5 * (h - 1);

	float dx = delta[0];
	float dy = delta[1];
	float dw = delta[2];
	float dh = delta[3];

	float pred_ctr_x = dx * w + x_ctr;
	float pred_ctr_y = dy * h + y_ctr;
	float pred_w = std::exp(dw) * w;
	float pred_h = std::exp(dh) * h;

	box = cv::Rect2f((pred_ctr_x - 0.5 * (pred_w - 1.0)) * ratiow,
					 (pred_ctr_y - 0.5 * (pred_h - 1.0)) * ratioh,
					 (pred_ctr_x + 0.5 * (pred_w - 1.0)) * ratiow,
					 (pred_ctr_y + 0.5 * (pred_h - 1.0)) * ratioh);
	// cout << "ratio" << ratioh << " " << ratiow << endl;
}

// inline void AnchorGenerator::bbox_pred_Opt_a0(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box, int &i, int &j) {
// 	float pred_ctr_x = delta[0] * pred_w_a0[i][j] + pred_x_ctr_a0[i][j];
// 	float pred_ctr_y = delta[1] * pred_h_a0[i][j] + pred_y_ctr_a0[i][j];
// 	float pred_w = std::exp(delta[2]) * pred_w_a0[i][j];
// 	float pred_h = std::exp(delta[3]) * pred_h_a0[i][j];

// 	box = cv::Rect2f((pred_ctr_x - 0.5 * (pred_w - 1.0)),
// 					 (pred_ctr_y - 0.5 * (pred_h - 1.0)),
// 					 (pred_ctr_x + 0.5 * (pred_w - 1.0)),
// 					 (pred_ctr_y + 0.5 * (pred_h - 1.0)));
// }

// inline void AnchorGenerator::bbox_pred_Opt_a1(const CRect2f& anchor, const CRect2f& delta, cv::Rect2f& box, int &i, int &j) {
// 	float pred_ctr_x = delta[0] * pred_w_a1[i][j] + pred_x_ctr_a1[i][j];
// 	float pred_ctr_y = delta[1] * pred_h_a1[i][j] + pred_y_ctr_a1[i][j];
// 	float pred_w = std::exp(delta[2]) * pred_w_a1[i][j];
// 	float pred_h = std::exp(delta[3]) * pred_h_a1[i][j];

// 	box = cv::Rect2f((pred_ctr_x - 0.5 * (pred_w - 1.0)),
// 					 (pred_ctr_y - 0.5 * (pred_h - 1.0)),
// 					 (pred_ctr_x + 0.5 * (pred_w - 1.0)),
// 					 (pred_ctr_y + 0.5 * (pred_h - 1.0)));
// }

// 得到在原图中的landmark  delta是对于设定锚点的偏移量
void AnchorGenerator::landmark_pred(const CRect2f anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts) {
	float w = anchor[2] - anchor[0] + 1;
	float h = anchor[3] - anchor[1] + 1;
	float x_ctr = anchor[0] + 0.5 * (w - 1);
	float y_ctr = anchor[1] + 0.5 * (h - 1);

	pts.resize(delta.size());
	for (int i = 0; i < delta.size(); ++i) {
		pts[i].x = (delta[i].x * w + x_ctr) * ratiow;
		pts[i].y = (delta[i].y * h + y_ctr) * ratioh;
	}
}

// inline void AnchorGenerator::landmark_pred_Opt_a0(const CRect2f& anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts, int &i, int &j) {
// 	pts.resize(delta.size());
// 	for (int i = 0; i < delta.size(); ++i) {
// 		pts[i].x = (delta[i].x * pred_w_a0[i][j] + pred_x_ctr_a0[i][j]);
// 		pts[i].y = (delta[i].y * pred_h_a0[i][j] + pred_y_ctr_a0[i][j]);
// 	}
// }

// inline void AnchorGenerator::landmark_pred_Opt_a1(const CRect2f& anchor, const vector<cv::Point2f>& delta, vector<cv::Point2f>& pts, int &i, int &j) {
// 	pts.resize(delta.size());
// 	for (int i = 0; i < delta.size(); ++i) {
// 		pts[i].x = (delta[i].x * pred_w_a1[i][j] + pred_x_ctr_a1[i][j]);
// 		pts[i].y = (delta[i].y * pred_h_a1[i][j] + pred_y_ctr_a1[i][j]);
// 	}
// }