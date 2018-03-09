#include <iostream>

#include <orca/orca.h>
#include <future>
#include <orca/base/libuv_cpp11/uv/uv11.h>

using namespace  std;

class MyMessage
{
public:
    MyMessage(const char* str)
    {
        memset(message, 0x00, Size);
        memcpy(message, str, strlen(str));
    }
    ~MyMessage()
    {
        std::cout << "release" << std::endl;
    }
    const unsigned long size()
    {
        return Size;
    }
    const void* enter()
    {
        return static_cast<const void*>(message);
    }
private:
    static const unsigned long Size = 512;
    char message[Size];
};

REGISTER_MESSAGE_TYPE(MyMessage);

int main(int argc, char** args)
{
    orca::log::init("test.txt",1024*1024*100,1024,1024);
    orca::log::error("test");
    orca::MessagePack message;
    message.create("my message type");

    cout << (char*)message.enter() <<" :"<< message.size() << endl;

    orca::Framework framework;

    //orca::core::Actor<> a(&framework);

    orca::Actor actor(&framework);
    actor.registerHandler([](const orca::MessagePack&, const orca::Address&)
    {
    });

    auto async = std::async( []( )
    {
        uv::EventLoop loop(uv::EventLoop::NewLoop);
        uv::Timer<void*> timer(&loop, 1000, 1000 ,[](uv::Timer<void*>*, void*)
        {

        },nullptr);
        timer.start();
        loop.run();

    });
    async.get();
    
}