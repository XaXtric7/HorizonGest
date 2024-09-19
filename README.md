
# HorizonGest

HorizonGest is a C++ project that enables users to control system volume and brightness through hand gestures. The platform utilizes computer vision techniques powered by OpenCV to detect hand movements via a webcam, allowing for real-time adjustments of system settings without physical input devices.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Overview

HorizonGest leverages hand gesture recognition to control system settings like volume and screen brightness in real-time. Using a webcam feed, HorizonGest detects the user's hand, tracks its movement, and adjusts system settings based on specific gestures. The project uses OpenCV for image processing and gesture detection, making system control more intuitive and hands-free.

## Features

- **Hand Gesture Recognition**: Detects hand gestures using contours and convex hull techniques in OpenCV.
- **Volume Control**: Adjust system volume by moving your hand horizontally across the screen.
- **Brightness Control**: Modify screen brightness based on vertical hand movements.
- **Smooth Transitions**: Uses a smoothing algorithm to prevent abrupt changes in volume or brightness.
- **Real-Time Processing**: Processes video input in real-time for instant feedback and control.

## Technology Stack

- **Language**: C++ (Core logic and system control)
- **Computer Vision**: OpenCV (Hand detection and gesture recognition)
- **System Control**: Windows API (For controlling volume and brightness)
<<<<<<< HEAD
- **Preferred IDE**: Visual Studio 2022
=======
>>>>>>> 3803a3672e163012e44eb74d13636486a496b345

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/HorizonGest.git
   ```

<<<<<<< HEAD
   and copy HorizonGest.cpp

2. Open Visual Studio 2022:

   - Create new project
   - Choose C++ Console App
   - Paste the HorizonGest.cpp in it

3. Download & Install OpenCV:

   - Download the latest release : [OpenCV(click here)](https://opencv.org/releases/)
   - Add bin and lib folders to Environmental variables

4. Download & Install CMake:

   - Download the latest release : [OpenCV(click here)](https://opencv.org/releases/)
   - Add bin folder to Environmental variables

5. Go to project properties in VS 2022:

   - Change and edit the followings:
     i. VC++ Directories--> Include directories

   ```bash
   Locate OpenCV and add path C:\opencv\build\include
   ```

   ii. VC++ Directories--> Library directories

   ```bash
   Locate OpenCV and add path C:\opencv\build\x64\vc16\lib
   ```

   iii.Linker-->Input-->Additional Dependencies

   ```bash
   opencv_world4100d.lib
   ```

6. Run the application:
=======
2. Navigate to the project directory:

   ```bash
   cd HorizonGest
   ```

3. Install OpenCV:

   - Follow the [OpenCV installation guide](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html) for your platform.

4. Compile the project:

   ```bash
   g++ horizon_gest.cpp -o HorizonGest -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect
   ```

5. Run the application:
>>>>>>> 3803a3672e163012e44eb74d13636486a496b345

   ```bash
   ./HorizonGest
   ```

## Usage

1. **Control Volume**: Move your hand horizontally (left/right) to increase or decrease the system volume.
2. **Control Brightness**: Move your hand vertically (up/down) to adjust the screen brightness.
3. **Real-Time Detection**: The system will display the current volume and brightness levels on-screen as your hand moves.
4. **Smooth Transitions**: Volume and brightness adjustments are smoothed using a moving average to ensure smooth changes without abrupt jumps.

<<<<<<< HEAD
## Application Preview
=======
## Site Preview
>>>>>>> 3803a3672e163012e44eb74d13636486a496b345

![HorizonGest gif](https://github.com/yourusername/HorizonGest/assets/demo.gif)

## Contributing

We welcome contributions! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes.
4. Push your branch and submit a pull request.

## License

This project is licensed under the MIT License. See the [MIT LICENSE](LICENSE) file for more details.

## Contact

If you have any questions or suggestions, feel free to contact the project team:

<<<<<<< HEAD
- [Sarthak Dharmik (XaXtric7)](https://github.com/XaXtric7)
- [Ansh (PikachuGX)](https://github.com/PikachuGX)
- [Disha Jain (dishaj18)](https://github.com/dishaj18)
=======
- [Your Name](https://github.com/yourusername)
>>>>>>> 3803a3672e163012e44eb74d13636486a496b345
