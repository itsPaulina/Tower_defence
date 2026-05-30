#pragma once // Ensures this header file is included only once.

class Animation {
public:
    explicit Animation(int totalFrames = 1, float fps = 8.f); // Constructor that creates an animation with a given number of frames and speed.

    void update(float dt);        // Updates the animation using elapsed time.
    int getCurrentFrame() const;  // Returns the current frame index.
    void reset();                 // Resets the animation to the beginning.

private:
    int totalFrames_;   // Total number of frames in the animation.
    float fps_;         // Frames per second, controls animation speed.
    int currentFrame_;  // Stores the currently active frame.
    float timer_;       // Stores elapsed time used to switch frames.
};