/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/Debug.h>
#include <Kernel/FileSystem/OpenFileDescription.h>
#include <Kernel/Process.h>

namespace Kernel {

using BlockFlags = Thread::FileBlocker::BlockFlags;

static ErrorOr<OpenFileDescription*> open_readable_file_description(Process::OpenFileDescriptions const& fds, int fd)
{
    auto description = TRY(fds.open_file_description(fd));
    if (!description->is_readable())
        return EBADF;
    if (description->is_directory())
        return EISDIR;
    return description;
}

static ErrorOr<void> check_blocked_read(OpenFileDescription* description)
{
    if (description->is_blocking()) {
        if (!description->can_read()) {
            auto unblock_flags = BlockFlags::None;
            if (Thread::current()->block<Thread::ReadBlocker>({}, *description, unblock_flags).was_interrupted())
                return EINTR;
            if (!has_flag(unblock_flags, BlockFlags::Read))
                return EAGAIN;
            // TODO: handle exceptions in unblock_flags
        }
    }
    return {};
}

ErrorOr<FlatPtr> Process::sys$readv(int fd, Userspace<const struct iovec*> iov, int iov_count)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    if (iov_count < 0)
        return EINVAL;

    // Arbitrary pain threshold.
    if (iov_count > (int)MiB)
        return EFAULT;

    u64 total_length = 0;
    Vector<iovec, 32> vecs;
    if (!vecs.try_resize(iov_count))
        return ENOMEM;
    TRY(copy_n_from_user(vecs.data(), iov, iov_count));
    for (auto& vec : vecs) {
        total_length += vec.iov_len;
        if (total_length > NumericLimits<i32>::max())
            return EINVAL;
    }

    auto description = TRY(open_readable_file_description(fds(), fd));

    int nread = 0;
    for (auto& vec : vecs) {
        TRY(check_blocked_read(description));
        auto buffer = UserOrKernelBuffer::for_user_buffer((u8*)vec.iov_base, vec.iov_len);
        if (!buffer.has_value())
            return EFAULT;
        auto nread_here = TRY(description->read(buffer.value(), vec.iov_len));
        nread += nread_here;
    }

    return nread;
}

ErrorOr<FlatPtr> Process::sys$read(int fd, Userspace<u8*> buffer, size_t size)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    if (size == 0)
        return 0;
    if (size > NumericLimits<ssize_t>::max())
        return EINVAL;
    dbgln_if(IO_DEBUG, "sys$read({}, {}, {})", fd, buffer.ptr(), size);
    auto description = TRY(open_readable_file_description(fds(), fd));
    TRY(check_blocked_read(description));
    auto user_buffer = UserOrKernelBuffer::for_user_buffer(buffer, size);
    if (!user_buffer.has_value())
        return EFAULT;
    return TRY(description->read(user_buffer.value(), size));
}

ErrorOr<FlatPtr> Process::sys$pread(int fd, Userspace<u8*> buffer, size_t size, off_t offset)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    if (size == 0)
        return 0;
    if (size > NumericLimits<ssize_t>::max())
        return EINVAL;
    if (offset < 0)
        return EINVAL;
    dbgln_if(IO_DEBUG, "sys$pread({}, {}, {}, {})", fd, buffer.ptr(), size, offset);
    auto description = TRY(open_readable_file_description(fds(), fd));
    if (!description->file().is_seekable())
        return EINVAL;
    TRY(check_blocked_read(description));
    auto user_buffer = UserOrKernelBuffer::for_user_buffer(buffer, size);
    if (!user_buffer.has_value())
        return EFAULT;
    return TRY(description->read(user_buffer.value(), offset, size));
}

}
