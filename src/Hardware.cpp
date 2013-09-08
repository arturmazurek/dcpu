//
//  Hardware.cpp
//  dcpu
//
//  Created by Artur Mazurek on 08.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Hardware.h"

#include <cassert>

#include "Core.h"
#include "DCPUException.h"

Hardware::Hardware(uint32_t hardwareId, uint16_t version, uint32_t manufacturer) :
m_hardwareId{hardwareId}, m_version{version}, m_manufacturer{manufacturer} {
    
}

void Hardware::attachedTo(std::weak_ptr<Core> ownerCore) {
    m_ownerCore = ownerCore;
}

void Hardware::receiveInterrupt() {
    auto shared = m_ownerCore.lock();
    
    assert(shared);
    if(!shared) {
        throw UnattachedHardware("Sending interrupt to unattached hardware");
    }
    
    doReceiveInterrupt(*shared);
}

void Hardware::sendInterrupt(uint16_t message) const {
    
}

uint32_t Hardware::hardwareId() const {
    return m_hardwareId;
}

uint16_t Hardware::version() const {
    return m_version;
}

uint32_t Hardware::manufacturer() const {
    return m_manufacturer;
}