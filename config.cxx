//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "config.hxx"

LauncherConfig::LauncherConfig() :
    width(800), height(600)
{ }

const LauncherConfig& LauncherConfig::instance() {
    static LauncherConfig config;
    return config;
}
