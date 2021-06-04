# ImageComparator

![Index](Documentation/MainView.png?raw=true)

A copy of the application can be found here [ImageComparator.exe](https://github.com/AmitRungta/ImageComparator/Releasex64/ImageComparator.exe)
## Introductions
This tool is developed in MSVC2019 using MFC. It has been created to show the pixel level difference between 2 images side by side. 

The main view region of the application is divided into 4 parts which shows the two original images, the absolute difference between the pixels in the images and a masked region between the 2 images which have a difference above the given threashold.



## ToolBar :
![Index](Documentation/Toolbar.png?raw=true)

Main toolbar is divided into 3 sections of options. 

* ![](Documentation/ImageOpen.png?raw=true) Image Open : This option is used for loading the 2 images for comparision. The tool shows the difference between the images only whne both of the images are opened.

* ![](Documentation/ImageZoom.png?raw=true) Zoom View : This option is useful for chnaging the zoom level of the displayed images. User can change the zoom level by pressing Ctrl key and scrolling the Mouse Wheel. 

* ![](Documentation/ImageThreshold.png?raw=true) Threshold : This option is useful for defining the threshold value between the pixels of the image above which we should make the image as different. Larger the threshold value lesser will the pixels covered in the difference region. 
