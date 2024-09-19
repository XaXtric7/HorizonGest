#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <highlevelmonitorconfigurationapi.h>

#pragma comment(lib, "Dxva2.lib")

using namespace cv;
using namespace std;

// Function to get the system volume and set it
static void setVolume(float volume)
{
    HRESULT hr;
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();

    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();

    hr = endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);

    endpointVolume->Release();
    CoUninitialize();
}

// Function to control screen brightness
static BOOL SetBrightnessLevel(DWORD level)
{
    HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
    DWORD nMonitors = 0;
    if (GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &nMonitors) == FALSE)
    {
        return FALSE;
    }

    PHYSICAL_MONITOR *pPhysicalMonitors = new PHYSICAL_MONITOR[nMonitors];
    if (GetPhysicalMonitorsFromHMONITOR(hMonitor, nMonitors, pPhysicalMonitors) == FALSE)
    {
        delete[] pPhysicalMonitors;
        return FALSE;
    }

    BOOL result = SetMonitorBrightness(pPhysicalMonitors[0].hPhysicalMonitor, level);

    DestroyPhysicalMonitors(nMonitors, pPhysicalMonitors);
    delete[] pPhysicalMonitors;

    return result;
}

// Function to detect hand using contours and convex hull
static vector<Point> detectHand(Mat &img, Mat &imgThresh)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgThresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int largestContourIdx = -1;
    double largestArea = 0;
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > 5000)
        { // Reduced minimum area threshold for better palm detection
            if (area > largestArea)
            {
                largestArea = area;
                largestContourIdx = (int)i;
            }
        }
    }

    vector<Point> hull;
    if (largestContourIdx != -1)
    {
        convexHull(contours[largestContourIdx], hull);
    }

    return hull;
}

// Function to smooth out the fluctuations using a moving average
static float smoothValue(float newValue, float prevValue, float alpha = 0.9)
{
    return alpha * prevValue + (1 - alpha) * newValue;
}

int main()
{
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cout << "Error: Camera not accessed!" << endl;
        return -1;
    }

    float volumeLevel = 0.5f;
    int brightnessLevel = 50;
    float prevVolume = 0.5f;
    int prevBrightness = 50;

    // Variables for gesture stability
    Point prevCenter(0, 0);
    bool isHandDetected = false;
    int stableFrameCount = 0;
    const int STABLE_FRAME_THRESHOLD = 5; // Reduced for quicker response

    while (true)
    {
        Mat img, imgHSV, imgYCrCb, imgThresh;
        cap.read(img);
        flip(img, img, 1);

        // Convert to YCrCb color space for better skin detection
        cvtColor(img, imgYCrCb, COLOR_BGR2YCrCb);

        // Define range for skin color in YCrCb
        Scalar lowerSkin(0, 135, 85);
        Scalar upperSkin(255, 180, 135);
        inRange(imgYCrCb, lowerSkin, upperSkin, imgThresh);

        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        erode(imgThresh, imgThresh, kernel);
        dilate(imgThresh, imgThresh, kernel);

        vector<Point> handHull = detectHand(img, imgThresh);

        if (!handHull.empty())
        {
            polylines(img, handHull, true, Scalar(0, 255, 0), 3);

            Moments m = moments(handHull, true);
            Point center(m.m10 / m.m00, m.m01 / m.m00);

            // Check for hand stability
            if (abs(center.x - prevCenter.x) < 30 && abs(center.y - prevCenter.y) < 30)
            {
                stableFrameCount++;
            }
            else
            {
                stableFrameCount = 0;
            }

            if (stableFrameCount >= STABLE_FRAME_THRESHOLD)
            {
                isHandDetected = true;
            }
            else
            {
                isHandDetected = false;
            }

            prevCenter = center;

            if (isHandDetected)
            {
                // Control volume
                float volumeRange = (float)img.cols / 2;
                float volumePosition = center.x / volumeRange;
                volumeLevel = smoothValue(volumePosition, prevVolume);
                volumeLevel = min(volumeLevel, 1.2f); // Cap volume at 100%
                prevVolume = volumeLevel;
                setVolume(volumeLevel);

                // Control brightness
                float brightnessRange = (float)img.rows;
                float brightnessPosition = (img.rows - center.y) / brightnessRange;
                brightnessLevel = (int)(brightnessPosition * 100);
                brightnessLevel = max(0, min(100, brightnessLevel));
                brightnessLevel = (int)smoothValue((float)brightnessLevel, (float)prevBrightness);
                prevBrightness = brightnessLevel;

                // Convert brightness from 0-100 to 0-255 range
                DWORD brightnessValue = (DWORD)((brightnessLevel / 100.0) * 255);
                if (!SetBrightnessLevel(brightnessValue))
                {
                }

                // Draw controls
                rectangle(img, Point(50, 50), Point(150, 150), Scalar(0, 0, 255), 4);
                rectangle(img, Point(50, 150 - (int)(volumeLevel * 100)), Point(150, 150), Scalar(0, 0, 255), FILLED);
                putText(img, "Volume: " + to_string((int)(volumeLevel * 100)) + "%", Point(10, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);

                rectangle(img, Point(200, 50), Point(300, 150), Scalar(255, 255, 0), 4);
                rectangle(img, Point(200, 150 - brightnessLevel), Point(300, 150), Scalar(255, 255, 0), FILLED);
                putText(img, "Brightness: " + to_string(brightnessLevel) + "%", Point(10, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
            }
        }
        else
        {
            isHandDetected = false;
            stableFrameCount = 0;
        }

        imshow("Hand Detection", img);

        if (waitKey(1) == 32)
        {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}