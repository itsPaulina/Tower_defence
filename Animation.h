#pragma once

class Animation {
public:
    explicit Animation(int totalFrames = 1, float fps = 8.f);

    void update(float dt);
    int getCurrentFrame() const;
    void reset();

private:
    int totalFrames_;
    float fps_;
    int currentFrame_;
    float timer_;
};