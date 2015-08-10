//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/10.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_CABAL_HXX
#define ILLYBLAUNCHER_CABAL_HXX

namespace CABAL {

enum PathOrder {
    APPEND, PREPEND };

enum OpenMode {
    READ, WRITE };

enum SeekOrigin {
    BEGINNING, CURRENT, END };

struct FileHandleImpl;

typedef long fsizetG;
typedef long fsizet;

bool init(const char *argv0);
bool dispose();

bool addSearchPath(const char *dir, PathOrder order = APPEND);
bool mountSearchPath(const char *dir, const char *mountdir, PathOrder order = APPEND);

const char *getHomeDirectory();

bool exists(const char *path);
bool remove(const char *path);
long getModificationTime(const char *path);
bool isDirectory(const char *path);

bool setWriteDirectory(const char *path);
const char *getWriteDirectory();
bool pushWriteDirectory(const char *path);
bool popWriteDirectory();

struct FileHandle {
public:

    bool close();
    bool isInvalid() const { return *this == invalid; }
    bool operator == (const FileHandle& other) const {
        return this->m_impl == other.m_impl; }
    fsizetG size() const;

    fsizetG read(void *buf, fsizet count);
    fsizetG write(const void *buf, fsizet count);

    fsizetG tell() const;
    fsizetG seek(fsizetG offset, SeekOrigin origin);

    static FileHandle invalid;

private:
    FileHandleImpl *m_impl;

    FileHandle(FileHandleImpl *impl) : m_impl(impl) { }

    friend FileHandle *open(const char *path, OpenMode mode);
};

// should we return a 'handle', or a pointer?
//  NOTE: the 'handle' is not actually a 'handle',
//      it's a handle class with helpers, and a
//      pointer effectively inside it
FileHandle *open(const char *path, OpenMode mode = READ);

}

#endif // ILLYBLAUNCHER_CABAL_HXX
