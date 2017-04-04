#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <time.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;
using namespace cv;

//------------Define bag of bow variable in here-------------
//
//
//---------Mask variable---------------------------------
Mat src;
bool mousedown;
vector<vector<Point> > contours;
vector<Point> pts;
int runMask = 0;
Mat mask;
//--------------------------
// Draw mask using mouse
void onMouse(int event, int x, int y, int flags, void* userdata)
{
	Mat img = *((Mat *)userdata);

	if (event == EVENT_LBUTTONDOWN)
	{
		mousedown = true;
		contours.clear();
		pts.clear();
	}

	if (event == EVENT_LBUTTONUP)
	{
		mousedown = false;
		if (pts.size() > 2)
		{
			runMask = 1;
			mask = Mat(img.size(), CV_8UC1);
			//mask = 0;
			contours.push_back(pts);
			drawContours(mask, contours, 0, Scalar(255), -1);
			Mat masked(img.size(), CV_8UC3, Scalar(255, 255, 255));
			src.copyTo(masked, mask);
			//src.copyTo(cloneimg);
			imshow("masked", masked);

		}
	}

	if (mousedown)
	{
		if (pts.size() > 2)
			line(img, Point(x, y), pts[pts.size() - 1], Scalar(0, 255, 0));

		pts.push_back(Point(x, y));

		imshow("Create Mask", img);
	}
} 
//Create Mask Image
Mat CreateMask()
{

	namedWindow("Create Mask", WINDOW_AUTOSIZE);
	setMouseCallback("Create Mask", onMouse, &src);
	imshow("Create Mask", src);
	waitKey(0);
	if (runMask == 1) {
		cv::destroyWindow("Create Mask");
		return mask;
	}
}

int main() {
	char ExitKey;
	cv::VideoCapture cap(1);


	for (;;)
	{
		clock_t st = clock();
		Mat frame;

		cap >> frame;
		if (runMask == 0) {
			src = frame.clone();
			CreateMask();
		}
	

		Mat masked(frame.size(), CV_8UC3, Scalar(255, 255, 255));
		frame.copyTo(masked, mask);
		imshow("masked", masked);//Show the mask
		
		
		cv::imshow("Frame", frame);
		ExitKey = cv::waitKey(1);
		printf("Time taken: %.5fs\n", (double)(clock() - st) / CLOCKS_PER_SEC);
		if (ExitKey == 27)
			break;


	}
	return 0;
}