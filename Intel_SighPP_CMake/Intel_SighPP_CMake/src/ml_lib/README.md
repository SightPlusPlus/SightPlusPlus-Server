# ML modules and Object tracking
## Overview
- Object Detection Module, responsible for detecting objects in each frame, trakcing objects and sending results to proritiser.

## Documentation
### Components
- ml_interface.hpp: object detection network interfaces
- caffe_impl.cpp & yolo_impl.cpp:  Implementation of Caffe-based network (MobileNetSSD) and darknet-based network (YOLOv3) respectively
- model_helper.hpp & model_creator.hpp: Header files containing simple functions for MobileNetSSD network and YOLOv3 network implementation
- object_tracking.hpp & object_tracking.cpp: Object tracking system

### Design Decisions
The Caffe-based Network (MobileNetSSD) is used as the primary network due in the system to its good performance. The darknet-based network (YOLOv3) is also implemented here as the second option. Harr Cascade Classifiers was considered but not implementedd and used because training-related fucntions are not included in opencv4.

### Limitations
- Poor performance when the image resolution is very high.
- High requirements for the training dataset for object detection network (a large set of labelled images should be used to train the networks)
- Difficulty when defining the classes of the objects when preparing training dataset (e.g. cups vs. mugs vs. glass cups).

### Visuals and diagrams
![bottle_new](https://user-images.githubusercontent.com/39096904/93452877-db0a6900-f8d0-11ea-909c-4b73c53b1967.png)

## Development Manual
-
-

### Requirements
- OpenCV
- TBB
- SPDLOG (for logging)

### Future development
* Improve the performance of the object detection moule (execuutting with GPU; applying OpenVINOetc.).
* Train more object detection models with higher accuracy.
