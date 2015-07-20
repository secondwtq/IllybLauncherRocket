//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/20.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_ROCKETEVENTINSTANCER_HXX
#define ILLYBLAUNCHER_ROCKETEVENTINSTANCER_HXX

#include <Rocket/Core/EventListenerInstancer.h>

#include "RocketEventListener.hxx"

class RocketEventInstancer : public Rocket::Core::EventListenerInstancer {
    public:

    virtual Rocket::Core::EventListener *InstanceEventListener(const Rocket::Core::String& value,
                                                               Rocket::Core::Element *element) override {
        return new RocketEventListener(value); }

    virtual void Release() override { delete this; }

};

#endif // ILLYBLAUNCHER_ROCKETEVENTINSTANCER_HXX
