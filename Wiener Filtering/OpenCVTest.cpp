/*
디지털미디어학과 201921147 서동건
Wiener Filtering 과제
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

Mat Wiener(const Mat& src, const Mat& kernel, double k) {
	Mat sf, kf, rf, ret;
	dft(src, sf, DFT_COMPLEX_OUTPUT);         // 소스 sf에 넣고
	dft(kernel, kf, DFT_COMPLEX_OUTPUT);  // 커널도 kf에 넣음

	rf.create(sf.size(), sf.type());

	for (Point pt(0, 0); pt.y < rf.rows; pt.y++) { //3. DFT 값은 complex 값임을 주의하여
		for (pt.x = 0; pt.x < rf.cols; pt.x++) {

			float a = sf.at<Vec2f>(pt)[0];
			float b = sf.at<Vec2f>(pt)[1];
			float c = kf.at<Vec2f>(pt)[0];
			float d = kf.at<Vec2f>(pt)[1];
			
			float dx = pt.x > kf.cols / 2 ? kf.cols - pt.x:pt.x;
			float dy = pt.y > kf.rows / 2 ? kf.rows - pt.y:pt.y;
			float D = sqrtf(dx * dx + dy * dy);
			//if (D > 25) rf.at<Vec2f>(pt) = Vec2f(0,0);
			//else rf.at<Vec2f>(pt) = Vec2f((a * c + b * d) / (c * c + d * d) + k * sf.at<Vec2f>(pt)[0], (b * c - a * d) / (c * c + d * d) + k * sf.at<Vec2f>(pt)[1]);
			rf.at<Vec2f>(pt) = Vec2f(a * c + b * d, b * c - a * d) / ((c * c + d * d) + k);
	
		}
	}

	dft(rf, ret, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
	return ret;
}

int main()
{
    Mat img1 = cv::imread("C:/Users/sdgsk/OneDrive/바탕 화면/Wiener_Input1.png", 0);
	//resize(img1, img1, Size(), 0.5, 0.5);
	Mat img2 = cv::imread("C:/Users/sdgsk/OneDrive/바탕 화면/Wiener_Input2.png", 0);
	//resize(img2, img2, Size(), 0.5, 0.5);
	Mat kernel = cv::imread("C:/Users/sdgsk/OneDrive/바탕 화면/Wiener_Kernel.png", 0);

	double K1 = 0.00132;    //1. 각각 다른 K 값을 사용
	double K2 = 0.00603;    //1. 각각 다른 K 값을 사용

	img1.convertTo(img1, CV_32F, 1 / 255.f);
	img2.convertTo(img2, CV_32F, 1 / 255.f);
	kernel.convertTo(kernel, CV_32F, 1 / 255.f); //2. 파일을 읽고, float로 바꾼 후
	kernel /= sum(kernel)[0]; //2. 전체 값의 합으로 한번 나누기

	Mat res1 = Wiener(img1, kernel, K1);
	Mat res2 = Wiener(img2, kernel, K2);

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("Wiener1", res1);
	imshow("Wiener2", res2);
	//imshow("kernel", kernel);
    waitKey();
}