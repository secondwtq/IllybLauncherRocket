#ifndef CABAL_PORT_ROCKET_H
#define CABAL_PORT_ROCKET_H

#include <Rocket/Core/String.h>
#include <Rocket/Core/FileInterface.h>

class ShellFileInterface : public Rocket::Core::FileInterface {
public:

	virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path) override;
	virtual void Close(Rocket::Core::FileHandle file) override;
	virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file) override;
	virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin) override;
	virtual size_t Tell(Rocket::Core::FileHandle file) override;
	virtual size_t Length(Rocket::Core::FileHandle file) override;

};

#endif
