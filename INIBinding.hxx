//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/30.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_INIBINDING_HXX
#define ILLYBLAUNCHER_INIBINDING_HXX

extern "C" {
struct lua_State;
}

void registerInterfaceINI(lua_State *L);

#endif // ILLYBLAUNCHER_INIBINDING_HXX
