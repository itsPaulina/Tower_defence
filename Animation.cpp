#include "Animation.h"

Animation::Animation(int totalFrames, float fps)
    : totalFrames_(totalFrames), fps_(fps), currentFrame_(0), timer_(0.f) {}

void Animation::update(float dt) {
    timer_ += dt;
    if (timer_ >= 1.f / fps_) {
        timer_ = 0.f;
        currentFrame_ = (currentFrame_ + 1) % totalFrames_;
    }
}

int Animation::getCurrentFrame() const { 
    return currentFrame_; }
void Animation::reset() { 
    currentFrame_ = 0; 
    timer_ = 0.f; }