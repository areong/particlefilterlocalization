#include "OpenniCamera.h"

OpenniCamera::OpenniCamera(void) {
}

OpenniCamera::~OpenniCamera(void) {
}

int OpenniCamera::initialize() {
    // Initialize OpenNI2.
    Status status = OpenNI::initialize();

    // Use any available device.
    device.open(ANY_DEVICE);

    // Create a depth video stream from device.
    videoStreamDepth.create(device, SENSOR_DEPTH);

    // Start streaming.
    videoStreamDepth.start();

    // Take a first shot to get width and height data.
    videoStreamDepth.readFrame(&videoFrameRefDepth);

    // Get resolutions AFTER readFrame(). (BEDORE causes wrong.)
    widthFrame = videoFrameRefDepth.getWidth();
    heightFrame = videoFrameRefDepth.getHeight();

    cout << widthFrame << '\t' << heightFrame << endl;

    return 0;
}

void OpenniCamera::takeNewDepthPhoto(double xFront, double yFront, double zFront, 
                                     double xTop, double yTop, double zTop) {
    // Get a frame referenced by mVideoFrameRef.
    videoStreamDepth.readFrame(&videoFrameRefDepth);

    // Get the data array of the frame.
    depthPhoto = (uint16_t *)videoFrameRefDepth.getData();

    cout << depthPhoto[15000] << endl;
}