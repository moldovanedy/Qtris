#include "settings.h"

using namespace DataManager::Settings;

static std::map<SettingType, int> _intSettings;
static std::map<SettingType, bool> _boolSettings;
static std::map<SettingType, float> _floatSettings;
static std::map<SettingType, char *> _stringSettings;

static std::map<SettingType, int> _settingsDataTypes
{
    {SettingType::Audio_MainVolume, 0},
    {SettingType::Audio_SfxVolume, 0},
    {SettingType::Audio_CustomSoundtrack, 3},
};

int DataManager::Settings::getIntSetting(SettingType settingType) {
    if (_settingsDataTypes[settingType] != 0) {
        throw std::invalid_argument("settingType");
    }
    return 0;
}

bool DataManager::Settings::getBoolSetting(SettingType settingType) {
    if (_settingsDataTypes[settingType] != 1) {
        throw std::invalid_argument("settingType");
    }
    return false;
}

float DataManager::Settings::getFloatSetting(SettingType settingType) {
    if (_settingsDataTypes[settingType] != 2) {
        throw std::invalid_argument("settingType");
    }
    return 0;
}

char *DataManager::Settings::getStringSetting(SettingType settingType) {
    if (_settingsDataTypes[settingType] != 3) {
        throw std::invalid_argument("settingType");
    }
    return new char[1] {'\0'};
}


bool DataManager::Settings::setIntSetting(SettingType settingType, int value) {
    if (_settingsDataTypes[settingType] != 0) {
        return false;
    }
    return true;
}

bool DataManager::Settings::setBoolSetting(SettingType settingType, bool value) {
    if (_settingsDataTypes[settingType] != 1) {
        return false;
    }
    return true;
}

bool DataManager::Settings::setFloatSetting(SettingType settingType, float value) {
    if (_settingsDataTypes[settingType] != 2) {
        return false;
    }
    return true;
}

bool DataManager::Settings::setStringSetting(SettingType settingType, char *value) {
    if (_settingsDataTypes[settingType] != 3) {
        return false;
    }
    return true;
}