/*
 * Generic reader interface for MP3FS
 *
 * Copyright (C) 2018 K. Henriksson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef MP3FS_READER_H_
#define MP3FS_READER_H_

#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <string>

class Reader {
 public:
    /** Read bytes into the internal buffer and into the given buffer. */
    virtual ssize_t read(char* buff, off_t offset, size_t len) = 0;

    virtual ~Reader() = default;
};

class FileReader : public Reader {
 public:
    explicit FileReader(int fd) : fd_(fd) {}

    ~FileReader() override { close(fd_); }

    ssize_t read(char* buff, off_t offset, size_t len) override {
        return pread(fd_, buff, len, offset);
    }

 private:
    int fd_;
};

class MemoryReader : public Reader {
 public:
    explicit MemoryReader(std::string data) : data_(std::move(data)) {}

    ssize_t read(char* buff, off_t offset, size_t len) override {
        if (offset < 0 || static_cast<size_t>(offset) >= data_.size()) {
            return 0;
        }

        const size_t remaining = data_.size() - static_cast<size_t>(offset);
        const size_t bytes = std::min(len, remaining);
        memcpy(buff, data_.data() + offset, bytes);
        return static_cast<ssize_t>(bytes);
    }

 private:
    std::string data_;
};

#endif  // MP3FS_READER_H_
