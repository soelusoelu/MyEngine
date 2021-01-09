#pragma once

class AnimationViewer {
public:
    AnimationViewer();
    ~AnimationViewer();

private:
    AnimationViewer(const AnimationViewer&) = delete;
    AnimationViewer& operator=(const AnimationViewer&) = delete;
};
