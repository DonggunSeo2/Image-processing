/*
디지털미디어학과 201921147 서동건
Connected Component 과제
*/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_world470d") 
#else
#pragma comment(lib,"opencv_world470")
#endif
using namespace cv;
using namespace std;

Mat image = imread("D:/아주대/2023/1학기/수업/영상처리/과제/Connected Component/connectedComponents.png"); // 첨부된 이미지 불러옴
Mat connectedComponent;

// 마우스 클릭 함수
void Mouseclick(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN) // 마우스 버튼을 입력했을때만 동작
    {
        Point seedPoint(x, y);
        Mat binaryImage = image.clone();
        threshold(binaryImage, binaryImage, 1, 255, THRESH_BINARY);
        connectedComponent = Mat::zeros(image.size(), CV_8UC1);
        Mat SE = getStructuringElement(MORPH_RECT, Size(3, 3));
        bool convergence = false;
        while (!convergence) {
            Mat previousComponent = connectedComponent.clone();
            dilate(connectedComponent, connectedComponent, SE, Point(-1, -1));
            bitwise_and(binaryImage, connectedComponent, connectedComponent); // black 이미지와 그냥 이미지 교집합해서 블랙이미지에 저장
            Mat Result;
            compare(connectedComponent, previousComponent, Result, CMP_EQ); // 이진 영상이 동일한지 비교
            int nonZeroPixels = countNonZero(Result);
            if (nonZeroPixels == connectedComponent.rows * connectedComponent.cols)
            {
                convergence = true;
            }
        }
        connectedComponent.setTo(255, connectedComponent == connectedComponent.at<uchar>(seedPoint));
        namedWindow("Connected Component", WINDOW_AUTOSIZE);
        imshow("Connected Image", connectedComponent); // 원본이미지에서 찾은 흰색이미지 블랙이미지에 출력 
        waitKey(5);
    }
}

int main()
{
    namedWindow("Input Image", WINDOW_AUTOSIZE);
    imshow("Input Image", image);
    setMouseCallback("Input Image", Mouseclick, NULL);
    waitKey(0);
    return 0;
}