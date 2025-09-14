#define _IR_ENABLE_DEFAULT_ false
#define SEND_SAMSUNG_AC true
#define DECODE_SAMSUNG_AC false

#include "esphome.h"
#include "ir_Samsung.h"
#include "samsung.h"

namespace esphome
{
    namespace samsung
    {
        // copied from ir_Samsung.cpp
        const uint16_t kSamsungAcHdrMark = 690;
        const uint16_t kSamsungAcHdrSpace = 17844;
        const uint8_t kSamsungAcSections = 2;
        const uint16_t kSamsungAcSectionMark = 3086;
        const uint16_t kSamsungAcSectionSpace = 8864;
        const uint16_t kSamsungAcSectionGap = 2886;
        const uint16_t kSamsungAcBitMark = 586;
        const uint16_t kSamsungAcOneSpace = 1432;
        const uint16_t kSamsungAcZeroSpace = 436;

        static const char *const TAG = "samsung.climate";

        void SamsungClimate::setup()
        {
            climate_ir::ClimateIR::setup();
            this->apply_state();
        }

        void SamsungClimate::transmit_state()
        {
            this->apply_state();
            this->send();
        }

        void SamsungClimate::send()
        {
            uint8_t *message = this->ac_.getRaw();

            mark(kSamsungAcHdrMark);
            space(kSamsungAcHdrSpace);

            sendGeneric(
                this->transmitter_,
                kSamsungAcSectionMark, kSamsungAcSectionSpace,
                kSamsungAcBitMark, kSamsungAcOneSpace, 
                kSamsungAcBitMark, kSamsungAcZeroSpace, 
                kSamsungAcBitMark, kSamsungAcSectionGap,
                message, kSamsungAcSectionLength,
                38000
            );

            space(kDefaultMessageGap - kSamsungAcSectionGap);
        }

        void SamsungClimate::apply_state()
        {
            if (this->mode == climate::CLIMATE_MODE_OFF)
            {
                this->ac_.off();
            }
            else
            {
                this->ac_.setTemp(this->target_temperature);

                switch (this->mode)
                {
                case climate::CLIMATE_MODE_AUTO:
                    this->ac_.setMode(kSamsungAcAuto);
                    break;
                case climate::CLIMATE_MODE_HEAT:
                    this->ac_.setMode(kSamsungAcHeat);
                    break;
                case climate::CLIMATE_MODE_COOL:
                    this->ac_.setMode(kSamsungAcCool);
                    break;
                case climate::CLIMATE_MODE_DRY:
                    this->ac_.setMode(kSamsungAcDry);
                    break;
                case climate::CLIMATE_MODE_FAN_ONLY:
                    this->ac_.setMode(kSamsungAcFan);
                    break;
                }

                if (this->fan_mode.has_value())
                {
                    switch (this->fan_mode.value())
                    {
                    case climate::CLIMATE_FAN_AUTO:
                        this->ac_.setFan(kSamsungAcFanAuto);
                        break;
                    case climate::CLIMATE_FAN_LOW:
                        this->ac_.setFan(kSamsungAcFanLow);
                        break;
                    case climate::CLIMATE_FAN_MEDIUM:
                        this->ac_.setFan(kSamsungAcFanMed);
                        break;
                    case climate::CLIMATE_FAN_HIGH:
                        this->ac_.setFan(kSamsungAcFanHigh);
                        break;
                    }
                }

                switch (this->swing_mode)
                {
                case climate::CLIMATE_SWING_OFF:
                    this->ac_.setSwingH(false);
                    this->ac_.setSwing(false);
                    break;
                case climate::CLIMATE_SWING_VERTICAL:
                    this->ac_.setSwingH(false);
                    this->ac_.setSwing(true);
                    break;
                case climate::CLIMATE_SWING_HORIZONTAL:
                    this->ac_.setSwingH(true);
                    this->ac_.setSwing(false);
                    break;
                case climate::CLIMATE_SWING_BOTH:
                    this->ac_.setSwingH(true);
                    this->ac_.setSwing(true);
                    break;
                }

                this->ac_.on();
            }

            ESP_LOGI(TAG, "%s", this->ac_.toString().c_str());
        }
    } // namespace samsung
} // namespace esphome
