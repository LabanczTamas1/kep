//
// kep meretenek, tipusanak lekerdezese
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;




int main() {

	// egy kep beolvasasa
	Mat img = imread("Ostriches.webp",IMREAD_GRAYSCALE);
	VideoCapture cap("vid.mp4");
	if (!cap.isOpened()) {
		cerr << "Error: Could not open the video file." << endl;
		return -1;
	}

	int frame_number = 500;
	while (true) {
		cv::Mat frame;
		cap >> frame; // Read a frame from the video

		// Break the loop if the end of the video is reached
		if (frame.empty()) {
			break;
		}

		// Save the frame as an image file
		std::string filename = "frame_" + std::to_string(frame_number) + ".png";
		cv::imwrite(filename, frame);
		Mat frame2;
		cvtColor(frame, frame2, COLOR_BGR2GRAY);


		

		// Increment the frame number
		frame_number++;





		if (img.empty()) {
			cout << "Upsz. Ez nem nyithato." << endl;
			exit(-1);
		}


		GaussianBlur(frame2, frame2, Size(5, 5), 10);

		Mat dx, dy;
		Mat G, dest, deste;
		Sobel(frame2, dx, CV_32F, 1, 0); //CV_32F – gyökvonás miatt
		Sobel(frame2, dy, CV_32F, 0, 1);


		cv::sqrt(dx.mul(dx) + dy.mul(dy), G);
		convertScaleAbs(G, dest);

		cout << threshold(dest, deste, 30, 255, THRESH_BINARY_INV);

		vector<Vec3f> circles;
		HoughCircles(dest, circles, HOUGH_GRADIENT_ALT, 1, dest.rows / 8, 100, 0.7, 10, 100);


		for (size_t i = 0; i < circles.size(); i++) {
			Vec3i circlet = circles[i];
			Point center = Point(circlet[0], circlet[1]);
			int radius = circlet[2];
			circle(frame, center, 3, Scalar(0, 255, 20), -1, 8, 0);
			circle(frame, center, radius, Scalar(0, 20, 255), 3, 8, 0);
			circle(frame2, center, 3, Scalar(0, 255, 20), -1, 8, 0);
			circle(frame2, center, radius, Scalar(0, 20, 255), 3, 8, 0);
		}
		cout << circles.size() << endl;


		imshow("olvasztott1", img);
		imshow("olvasztott2", G);
		imshow("olvasztott3", dest);
		imshow("fin", deste);
		imshow("vid", frame);
		imshow("vid2", frame2);
		waitKey(16);

	}
	waitKey();
	cap.release();
}