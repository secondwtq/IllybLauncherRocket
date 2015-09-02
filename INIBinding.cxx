//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/30.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "INIBinding.hxx"

#include "thirdpt/luajit.hxx"
#include "thirdpt/luabridge.hxx"

#include "thirdpt/simpleini.hxx"

class CSimpleIniAWrap {
public:
    CSimpleIniAWrap() : m_impl(new CSimpleIniA()) { }

    int loadFromString(const std::string& data) {
        return static_cast<int>(this->m_impl->LoadData(data)); }

    int setValue(const char *section, const char *key, const char *value) {
        return static_cast<int>(this->m_impl->SetValue(section, key, value, nullptr, false)); }

    int setOrReplaceValue(const char *section, const char *key, const char *value) {
        return static_cast<int>(this->m_impl->SetValue(section, key, value, nullptr, true)); }

    std::string saveToString() {
        std::string ret;
        this->m_impl->Save(ret);
        return ret;
    }

    // TODO: should we solve it in mozjs?
    int saveToStringRef(std::string& str) {
        return static_cast<int>(this->m_impl->Save(str)); }

    int saveToFile(const char *filename) {
        return static_cast<int>(this->m_impl->SaveFile(filename)); }

    ~CSimpleIniAWrap() { delete m_impl; }

private:
    CSimpleIniA *m_impl;
};

void registerInterfaceINI(lua_State *L) {
    luabridge::getGlobalNamespace(L).
    beginNamespace("Facer").
    // TODO: enum, what we need to provide support in mozjs
//        beginClass<SI_Error>("SI_Error").endClass().
        beginClass<CSimpleIniAWrap>("CSimpleIniAWrap")
            .addConstructor<void (*)()>()
            .addFunction("loadFromString", &CSimpleIniAWrap::loadFromString)
            .addFunction("saveToString", &CSimpleIniAWrap::saveToString)
            .addFunction("saveToStringRef", &CSimpleIniAWrap::saveToStringRef)
            .addFunction("saveToFile", &CSimpleIniAWrap::saveToFile)
            .addFunction("setValue", &CSimpleIniAWrap::setValue)
            .addFunction("setOrReplaceValue", &CSimpleIniAWrap::setOrReplaceValue)
        .endClass()
    .endNamespace();
}
