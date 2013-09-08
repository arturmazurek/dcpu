//
//  Hardware.h
//  dcpu
//
//  Created by Artur Mazurek on 08.09.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __dcpu__Hardware__
#define __dcpu__Hardware__

#include <cstdint>

#include <memory>

class Core;

class Hardware {
public:
    Hardware(uint32_t hardwareId, uint16_t version, uint32_t manufacturer);
    
    void attachedTo(std::weak_ptr<Core> ownerCore);
    
    // blocks caller until the hardware finishes its doing
    void receiveInterrupt();
    
    uint32_t hardwareId() const;
    uint16_t version() const;
    uint32_t manufacturer() const;
    
protected:
    void sendInterrupt(uint16_t message) const;
    
    virtual void doReceiveInterrupt(Core& from) = 0;
    
private:
    std::weak_ptr<Core> m_ownerCore;
    
    const uint32_t m_hardwareId;
    const uint16_t m_version;
    const uint32_t m_manufacturer;
    
private:
    Hardware(const Hardware&);
    Hardware& operator=(const Hardware&);

};

#endif /* defined(__dcpu__Hardware__) */
