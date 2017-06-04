// Reddit:		https://www.reddit.com/user/IBNai/
// Instagram:	https://www.instagram.com/ib_nai/
// Facebook:	https://www.facebook.com/IB.Nye
// YouTube:   https://youtu.be/L7EqKJ02lK4

// https://www.elecrow.com/wiki/index.php?title=USB_Host_Shield_for_Arduino

#include <hidboot.h>
#include <usbhid.h>
#include <usbhub.h>

#include <avr/power.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

class BTHKBAdapter : public HIDReportParser 
{
    protected:
    virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t Length, uint8_t *buf);
};

void BTHKBAdapter::Parse(USBHID *hid, bool is_rpt_id, uint8_t Length, uint8_t *buf) {
    for (uint8_t i = 0; i < Length; i++) {
            Serial.write((uint8_t)buf[i]);
    }
    Serial.write("");
};

class KbdRptParser : public BTHKBAdapter 
{
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t Length, uint8_t *buf);
};

void KbdRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t Length, uint8_t *buf) {
    Serial.write((uint8_t)0xFD);
    Serial.write((uint8_t)Length+1);
    Serial.write((uint8_t)0x01);
    BTHKBAdapter::Parse(hid,is_rpt_id,Length,buf);
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

uint32_t next_time;
KbdRptParser Prs;
void setup()
{
    ADCSRA = 0; //disable ADC by setting ADCSRA register to 0
    power_adc_disable(); //disable the clock to the ADC module
    Serial.begin( 115200 );

#if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

    //Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );

    next_time = millis() + 5000;

    HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);

}

void loop()
{
    Usb.Task();
}








