
#include "CABALPortRocket.hxx"

#include <Rocket/Core/String.h>
#include <Rocket/Core/FileInterface.h>

#include "CABAL.hxx"

Rocket::Core::FileHandle ShellFileInterface::Open(const Rocket::Core::String& path) {
	return (Rocket::Core::FileHandle) CABAL::open(path.CString(), CABAL::READ); }

void ShellFileInterface::Close(Rocket::Core::FileHandle file) {
	CABAL::FileHandle *h = reinterpret_cast<CABAL::FileHandle *>(file);
	h->close();
}

size_t ShellFileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file) {
	CABAL::FileHandle *h = reinterpret_cast<CABAL::FileHandle *>(file);
	return static_cast<size_t>(h->read(buffer, size));
}

bool ShellFileInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin) {
	CABAL::SeekOrigin oric;
	switch (origin) {
		case SEEK_SET:
			oric = CABAL::BEGINNING;
			break;
		case SEEK_CUR:
			oric = CABAL::CURRENT;
			break;
		case SEEK_END:
			oric = CABAL::END;
			break;
		default:
			return false;
	}
	CABAL::FileHandle *h = reinterpret_cast<CABAL::FileHandle *>(file);
	return (h->seek(offset, oric) != -1);
}

size_t ShellFileInterface::Tell(Rocket::Core::FileHandle file) {
	CABAL::FileHandle *h = reinterpret_cast<CABAL::FileHandle *>(file);
	return static_cast<size_t>(h->tell());
}

size_t ShellFileInterface::Length(Rocket::Core::FileHandle file) {
	CABAL::FileHandle *h = reinterpret_cast<CABAL::FileHandle *>(file);
	return static_cast<size_t>(h->size());
}
