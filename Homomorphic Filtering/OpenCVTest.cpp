/*
디지털미디어학과 201921147 서동건
Homomorphic Filtering 과제
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

int main()
{
    Mat img = cv::imread("C:/Users/sdgsk/OneDrive/바탕 화면/homomorphicInput.jpeg", 0);
	img.convertTo(img, CV_32F, 1 / 255.f);
	imshow("image", img);
	const float D0 = 12;       // 주파수 파라미터 - 어디까지 조명변화, 효과인가 [10~20]
	const float gamma_L = 0.8; // 낮은주파수 파라미터 - 로우 얼만큼까지 줄어들지 [0.5~0.9]
	const float gamma_H = 4.7; // 높은주파수 파라미터 - 하이 얼마만큼 올릴지 [2~10]
	const float c = 1;
	img += 1;                  // f의 범위 0~1 로그취하면 -무한대~0 막기위해 1 더해줌  
	log(img, img);     // 로그 취해주고
	Mat F, img2;
	dft(img, F, DFT_COMPLEX_OUTPUT); // DFT 취한 다음

	for (Point pt(0,0); pt.y<F.rows;pt.y++)        // H filtering 적용하고
		for (pt.x = 0; pt.x < F.cols; pt.x++) {
			Vec2f v = F.at<Vec2f>(pt);
			float dx = pt.x < F.cols / 2 ? pt.x : F.cols - pt.x;
			float dy = pt.y < F.rows / 2 ? pt.y : F.rows - pt.y;
			float D = sqrtf(dx * dx + dy * dy);
			float H = (gamma_H - gamma_L) * (1 - exp(-c * D * D / (D0 * D0))) + gamma_L;
			F.at<Vec2f>(pt) = v*H;
		}

	dft(F,img2,DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE); // IDFT 취한 다음
	exp(img2, img2);  // exp 적용하고
	img2 -= 1;                // 마지막에 다시 1 빼줌

	imshow("img", img);
	imshow("homomorphic", img2);
    waitKey();
}