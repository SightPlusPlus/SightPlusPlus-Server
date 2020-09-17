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
- Are there any issues with this Design

### Visuals and diagrams
![bottle_new](https://user-images.githubusercontent.com/39096904/93452877-db0a6900-f8d0-11ea-909c-4b73c53b1967.png)

## Development Manual
-What do future developers need to understand here to develop it further.
### Component Developemnt
- How do I develop this component further? Where do I go?
### Requirements
- libs,framworks and apis that are required for this module.
### Future development
- Ideas for what could be done in the future. Can be bullet form.
