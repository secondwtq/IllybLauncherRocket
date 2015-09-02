//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_CONFIG_HXX
#define ILLYBLAUNCHER_CONFIG_HXX

struct LauncherConfig {
    unsigned short width, height;
    bool autoCenter, noBorder;

    LauncherConfig();
    static const LauncherConfig& instance();
};

#endif // ILLYBLAUNCHER_CONFIG_HXX
