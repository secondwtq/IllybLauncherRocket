//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/20.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_ROCKETEVENTLISTENER_HXX
#define ILLYBLAUNCHER_ROCKETEVENTLISTENER_HXX

#include <Rocket/Core/EventListener.h>

class RocketEventListener: public Rocket::Core::EventListener {
    public:

    RocketEventListener(const Rocket::Core::String& v) : value(v) { }

    virtual void ProcessEvent(Rocket::Core::Event& event) override;

    virtual void OnDetach(Rocket::Core::Element *element) override { delete this; }

    private:

    Rocket::Core::String value;

};

#endif // ILLYBLAUNCHER_ROCKETEVENTLISTENER_HXX
