#include "Octopus.h"
#include "OctopusFootManager.h"
#include "OctopusHead.h"

Octopus::Octopus()
    : Component()
{
}

Octopus::~Octopus() = default;

void Octopus::start() {
    mFootManager = getComponent<OctopusFootManager>();
    mHead = getComponent<OctopusHead>();
}

OctopusFootManager& Octopus::getFootManager() const {
    return *mFootManager;
}

OctopusHead& Octopus::getHead() const {
    return *mHead;
}
