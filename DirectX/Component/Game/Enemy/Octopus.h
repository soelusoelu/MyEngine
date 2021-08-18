#pragma once

#include "../../Component.h"
#include <memory>

class OctopusFootManager;
class OctopusHead;

class Octopus
    : public Component
{
public:
    Octopus();
    ~Octopus();
    Octopus(const Octopus&) = delete;
    Octopus& operator=(const Octopus&) = delete;

    virtual void start() override;

    OctopusFootManager& getFootManager() const;
    OctopusHead& getHead() const;

private:
    std::shared_ptr<OctopusFootManager> mFootManager;
    std::shared_ptr<OctopusHead> mHead;
};
