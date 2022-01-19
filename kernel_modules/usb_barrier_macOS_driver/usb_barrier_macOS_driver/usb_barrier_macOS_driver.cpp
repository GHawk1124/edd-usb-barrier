#include "usb_barrier_macOS_driver.hpp"

bool com_garrettcomes_USBBlockerDevice::init(OSDictionary *pDictionary) {
    if(!IOService::init(pDictionary)) {
        return false;
    }

    // Initialize here

    return true;
}
 
IOService* com_garrettcomes_USBBlockerDevice::probe(IOService *pProvider, SInt32 *pi32Score) {
    // bool shouldBlockDevice = decideTheVerdict(pProvider);
    bool shouldBlockDevice = true;
    if(shouldBlockDevice)
    {
        *pi32Score = INT32_MAX;
        return this;
    }

    return nullptr;
}
