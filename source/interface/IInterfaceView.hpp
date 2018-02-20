#pragma once

class IInterfaceView
{
public:
    IInterfaceView() = default;
    virtual ~IInterfaceView() = default;
    virtual void render() = 0;
};

