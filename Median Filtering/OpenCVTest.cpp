/*
디지털미디어학과 201921147 서동건
Median Filtering 과제
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

//1. Median filfer 함수는 입력 영상과 filter의 크기를 매개변수로 받는다.
cv::Mat MedianFilter(const Mat& img, const cv::Size sz) {
    assert(sz.width % 2 == 1 && sz.height % 2 == 1);
    //1. 이때 filter의 크기는 항상 홀수이고 크기가 sz라고 할 때 filter의 범위는 [-(sz-1)/2, (sz-1)/2]
    Mat ret(img.size(), img.type());
    int wx = (sz.width - 1) / 2;
    int wy = (sz.height - 1) / 2;
    //Cash Locality 잘 활용하고자 한줄로 적음 -> 전체이미지 모든픽셀에 대해 전부 해보겠다
    for (int y = 0; y < ret.rows; y++) for (int x = 0; x < ret.cols; x++) {
        //2. 출력 영상의 한 픽셀의 값을 얻기 위해 원본 영상의 해당 위치 주변의 픽셀 값을 수집
        std::vector<uchar> Dars; //2. 이때 filter의 크기의 제한이 없으므로 stl의 vector 을 사용하여 동적 배열 사용
        for (int dy = -wy; dy <= wy; dy++) for (int dx = -wx; dx <= wx; dx++) {
            int xx = x + dx;
            int yy = y + dy;
/*          3.또한, 범위 밖의 픽셀을 얻기 위해 replicate padding을 적용
            if (xx < 0) xx = 0;
            else if (xx > img.cols - 1) xx = img.cols - 1;
            if (yy < 0) yy = 0;
            else if (yy > img.rows - 1) yy = img.rows - 1;               */
            //3.또한, 범위 밖의 픽셀을 얻기 위해 zero padding을 적용
            if (xx >= 0 && yy >= 0 && xx < img.cols && yy < img.rows)\
                //0. 이미지 8bit, 1channel로 읽어와서 행렬에서 인덱싱은 끝에서부터 커지므로 y부터, 이미지 안에서 데이터(픽셀) 가져옴
                Dars.push_back(img.at<uchar>(yy, xx));
        }
        std::sort(Dars.begin(), Dars.end());                    //4. 출력 영상의 한 픽셀 값을 얻기 위해 stl등 기존의 정렬 알고리즘을 사용해 수집된 주변 픽셀 값들을 정렬
        ret.at<uchar>(y, x) = Dars[(sz.width * sz.height - 1) / 2]; //5. 정렬된 배열로 부터 중간 값을 찾고, 출력 영상의 해당 픽셀에 저장
    }
    //6. 출력 영상이 완성되면 return한다
    return ret;
}


int main()
{
    //0. 이미지 8bit, 1channel로 읽어와서 7. main 함수에서는 적당한 영상을 흑백으로 읽고(flags = 0)
    Mat image = cv::imread("C:/Users/sdgsk/OneDrive/바탕 화면/testimg.png", 0);
    //7. 위의 함수를 사용하여 median filtering을 수행한 후에 결과를 화면에 출력
    Mat img2 = MedianFilter(image, Size(3, 3)); //** 이때 다양한 크기의 filter를 적용하여
    Mat img3 = MedianFilter(image, Size(5, 5)); //계산시간과 filter 크기의 관계를 확인
    Mat img4 = MedianFilter(image, Size(7, 7));

    Mat CVMF;
    medianBlur(image, CVMF, 5);    //open cv 라이브러리와 비교 테스트
    
    imshow("Image", image);           //원본이미지
    imshow("OpenCV MF", CVMF);        //open cv사용
    imshow("My MedianFilter1", img2); //MedianFilter (3,3)만든거
    imshow("My MedianFilter2", img3); //MedianFilter (5,5)만든거
    imshow("My MedianFilter3", img4); //MedianFilter (7,7)만든거
    waitKey();
}