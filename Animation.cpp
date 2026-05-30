#include "Animation.h" // Includes the Animation class definition.

Animation::Animation(int totalFrames, float fps)
    : totalFrames_(totalFrames), // Sets how many frames the animation has.
      fps_(fps),                 // Sets how many frames should play per second.
      currentFrame_(0),          // Starts the animation at frame 0.
      timer_(0.f) {}             // Starts the timer at 0 seconds.

void Animation::update(float dt) {
    timer_ += dt; // Add the elapsed time since the last update.

    if (timer_ >= 1.f / fps_) { // Check if enough time passed to switch frame.
        timer_ = 0.f; // Reset the timer after changing the frame.
        currentFrame_ = (currentFrame_ + 1) % totalFrames_; // Move to next frame and loop back if needed.
    }
}

int Animation::getCurrentFrame() const {
    return currentFrame_; // Return the frame that should currently be displayed.
}

void Animation::reset() {
    currentFrame_ = 0; // Go back to the first frame.
    timer_ = 0.f;      // Reset the elapsed time.
}