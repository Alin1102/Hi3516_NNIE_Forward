1 需要使用交叉编译工具编译该项目

2 需要修改 

    (1) include/Yolov3.h 检测图片的宽高：DetectImgWidth， DetectImgHeight

    (2) Yolov3.cpp 中类别数量： classNum
    
    (3) Yolov3_layer.cpp 中Anchor设置： Anchors
