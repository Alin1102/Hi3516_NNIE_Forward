GCC = arm-himix200-linux-g++ -g -std=c++11 -O3 -Wno-write-strings -Wno-deprecated-declarations

INCLUDE = -I./include/

LIBs = -L./lib -lmpi -lnnie -ldnvqe -lupvqe -lVoiceEngine -ldl -lpthread -lsecurec -lopencv_imgcodecs -lopencv_imgproc -lopencv_core



FILES = Yolov3.cpp Yolov3_layer.cpp AnchorGenerator.cpp nnie_forward.cpp main.cpp

OBJECT = Yolov3.o Yolov3_layer.o AnchorGenerator.o nnie_forward.o main.o

TARGET = testModel

all: Main



Main:
	$(GCC) $(INCLUDE) -c $(FILES)

	$(GCC) -o $(TARGET) $(OBJECT) $(LIBs)
	
anchor:
	$(GCC) $(INCLUDE) -c AnchorGenerator.cpp -o AnchorGenerator.o

main:
	$(GCC) $(INCLUDE) -c main.cpp -o main.o

yolo:
	$(GCC) $(INCLUDE) -c Yolov3.cpp -o Yolov3.o

yololayer:
	$(GCC) $(INCLUDE) -c Yolov3_layer.cpp -o Yolov3_layer.o

nnie:
	$(GCC) $(INCLUDE) -c nnie_forward.cpp -o nnie_forward.o



.PHONY: clean
clean:
	rm -f *.o
	rm $(TARGET)