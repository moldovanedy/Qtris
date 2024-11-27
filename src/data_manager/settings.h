#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <map>
#include <stdexcept>

namespace DataManager::Settings {
    enum SettingType {
        None = 0,
        Audio_MainVolume = 1,
        Audio_SfxVolume = 2,
        Audio_CustomSoundtrack = 3
    };

    int getIntSetting(SettingType settingType);
    bool getBoolSetting(SettingType settingType);
    float getFloatSetting(SettingType settingType);
    char *getStringSetting(SettingType settingType);

    bool setIntSetting(SettingType settingType, int value);
    bool setBoolSetting(SettingType settingType, bool value);
    bool setFloatSetting(SettingType settingType, float value);
    bool setStringSetting(SettingType settingType, char *value);
}
#endif