#pragma once

struct Ray;

class IShotRaySetter {
public:
    virtual ~IShotRaySetter() = default;
    virtual void setShotRay(const Ray* ray) = 0;
};
