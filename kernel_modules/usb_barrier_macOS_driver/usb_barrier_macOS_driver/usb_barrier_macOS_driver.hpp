#include <IOKit/IOService.h>
 
class com_garrettcomes_USBBlockerDevice : public IOService
{
    OSDeclareDefaultStructors(com_garrettcomes_USBBlockerDevice);
public:
    virtual IOService *probe(IOService *pProvider, SInt32 *pi32Score) override;
    virtual bool init(OSDictionary *pDictionary) override;
};
OSDefineMetaClassAndStructors(com_garrettcomes_USBBlockerDevice, IOService);
