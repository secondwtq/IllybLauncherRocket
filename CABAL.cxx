//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/10.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include <physfs.h>

#include "CABAL.hxx"

#include <cassert>
#include <stack>
#include <string>

namespace CABAL {

struct FileHandleImpl {
    PHYSFS_File *ptr; };

bool init(const char *argv0) {
    assert(argv0);
    return (PHYSFS_init(argv0) != 0);
}

bool dispose() {
    return (PHYSFS_deinit() != 0);
}

bool addSearchPath(const char *dir, PathOrder order) {
    assert(dir);
    return ((PHYSFS_mount(dir, nullptr, (order == APPEND))) != 0);
}

bool mountSearchPath(const char *dir, const char *mountdir, PathOrder order) {
    assert(dir);
    assert(mountdir);
    return ((PHYSFS_mount(dir, mountdir, (order == APPEND))) != 0);
}

const char *getHomeDirectory() {
    return PHYSFS_getUserDir(); }

bool exists(const char *path) {
    assert(path);
    return PHYSFS_exists(path);
}

bool remove(const char *path) {
    assert(path);
    return (PHYSFS_delete(path) != 0);
}

long getModificationTime(const char *path) {
    assert(path);
    return PHYSFS_getLastModTime(path);
}

bool isDirectory(const char *path) {
    assert(path);
    return (PHYSFS_isDirectory(path) != 0);
}

FileHandle *open(const char *path, OpenMode mode) {
    assert(path);
    PHYSFS_File *f = nullptr;
    switch(mode) {
        case READ:
            f = PHYSFS_openRead(path);
            break;
        case WRITE:
            f = PHYSFS_openWrite(path);
            break;
    }
    if (!f) {
        return nullptr; }
    FileHandleImpl *r = new FileHandleImpl;
    r->ptr = f;
    return new FileHandle(r);
}

FileHandle FileHandle::invalid(nullptr);

bool FileHandle::close() {
    assert(m_impl && m_impl->ptr);
    bool ret = (PHYSFS_close(m_impl->ptr) != 0);
    delete m_impl;
    m_impl = nullptr;
    delete this;
    return ret;
}

// -1 undetermined
fsizetG FileHandle::size() const {
    assert(m_impl && m_impl->ptr);
    return PHYSFS_fileLength(m_impl->ptr);
}

fsizetG FileHandle::read(void *buf, fsizet count) {
    assert(m_impl && m_impl->ptr && buf);
    return PHYSFS_read(m_impl->ptr, buf, 1, count);
}

fsizetG FileHandle::write(const void *buf, fsizet count) {
    assert(m_impl && m_impl->ptr && buf);
    return PHYSFS_write(m_impl->ptr, buf, 1, count);
}

fsizetG FileHandle::tell() const {
    assert(m_impl && m_impl->ptr);
    return PHYSFS_tell(m_impl->ptr);
}

// from WebKit:
//  Returns the resulting offset from the beginning
//  of the file if successful, -1 otherwise.
fsizetG FileHandle::seek(fsizetG offset, SeekOrigin origin) {
    assert(m_impl && m_impl->ptr);
    long pos = -1;
    switch (origin) {
        case BEGINNING:
            pos = offset;
            break;
        case CURRENT:
            pos = tell() + offset;
            break;
        case END:
            pos = size() - offset;
            break;
    }
    int ret = PHYSFS_seek(m_impl->ptr, pos);
    if (ret == -1) {
        return -1; }
    return pos;
}

bool setWriteDirectory(const char *path) {
    assert(path);
    return (PHYSFS_setWriteDir(path) != 0);
}

const char *getWriteDirectory() {
    return PHYSFS_getWriteDir(); }

static std::stack<std::string> writeDirectories;

bool pushWriteDirectory(const char *path) {
    assert(path);
    const char *org_cstr = getWriteDirectory();
    if (org_cstr) {
        writeDirectories.push({ org_cstr }); }
    return setWriteDirectory(path);
}

bool popWriteDirectory() {
    if (writeDirectories.empty()) {
        return setWriteDirectory(nullptr); }
    bool ret = setWriteDirectory(writeDirectories.top().c_str());
    writeDirectories.pop();
    return ret;
}

}
