#include "ssharp-deflate.h"

namespace ssharp::utils::deflate
{
#define checkSuccess(x) if(x)throw unsuccess_zlib_operation("zlib operation not success")
    buff_pair_t deflateBuff(const buff_pair_t& inpair)
	{
        auto& inbuff = inpair.first;
        auto insize = (uint32_t)inpair.second;
        auto inb = (Bytef*)inbuff.get();
        
        z_stream stream;
        int err;
        const uInt max = (uInt)-1;
        uLong left;

        stream.zalloc = (alloc_func)0;
        stream.zfree = (free_func)0;
        stream.opaque = (voidpf)0;

        checkSuccess(deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -(MAX_WBITS), DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY));
        left = deflateBound(&stream, insize);
        buff_t outbuff = make_shared<char[]>(left);

        auto outb = (Bytef*)outbuff.get();
        size_t outsize = 0;

        stream.next_out = outb;
        stream.avail_out = 0;
        stream.next_in = (z_const Bytef*)inb;
        stream.avail_in = 0;

        do {
            if (stream.avail_out == 0) {
                stream.avail_out = left > (uLong)max ? max : (uInt)left;
                left -= stream.avail_out;
            }
            if (stream.avail_in == 0) {
                stream.avail_in = insize > (uLong)max ? max : (uInt)insize;
                insize -= stream.avail_in;
            }
            err = ::deflate(&stream, insize ? Z_NO_FLUSH : Z_FINISH);
        } while (err == Z_OK);

        outsize = stream.total_out;
        deflateEnd(&stream);
        checkSuccess(!Z_STREAM_END);
        return buff_pair_t(std::move(outbuff), outsize);
	}

    buff_pair_t inflateBuff(const buff_pair_t& inpair, uint32_t outsize)
    {
        auto& inbuff = inpair.first;
        auto insize = (uint32_t)inpair.second;
        z_stream stream;
        int err;
        const uInt max = (uInt)-1;
        uLong len, left;
        Byte buf[1];    /* for detection of incomplete stream when *destLen == 0 */

        buff_t outbuff = nullptr;
        len = insize;

        if (outsize) {
            left = outsize;
            outsize = 0;
            outbuff = make_shared<char[]>(left);
        }
        else {
            left = 1;
            outbuff = shared_ptr<char[]>((char*)buf);
        }
        auto inb = (Bytef*)inbuff.get();
        auto outb = (Bytef*)outbuff.get();
        stream.next_in = (z_const Bytef*)inb;
        stream.avail_in = 0;
        stream.zalloc = (alloc_func)0;
        stream.zfree = (free_func)0;
        stream.opaque = (voidpf)0;

        checkSuccess(inflateInit2(&stream, -(DEF_WBITS)));

        stream.next_out = outb;
        stream.avail_out = 0;

        do {
            if (stream.avail_out == 0) {
                stream.avail_out = left > (uLong)max ? max : (uInt)left;
                left -= stream.avail_out;
            }
            if (stream.avail_in == 0) {
                stream.avail_in = len > (uLong)max ? max : (uInt)len;
                len -= stream.avail_in;
            }
            err = inflate(&stream, Z_NO_FLUSH);
            if (err == Z_OK && !left)
                err = Z_STREAM_END;
        } while (err == Z_OK);

        insize -= len + stream.avail_in;
        if (outb != buf)
            outsize = stream.total_out;
        else if (stream.total_out && err == Z_BUF_ERROR)
            left = 1;

        inflateEnd(&stream);
        checkSuccess(err != Z_STREAM_END);
        return buff_pair_t(std::move(outbuff),outsize);
    }

    uint32_t crc32Buff(const buff_pair_t& inpair)
    {
        return crc32(0, (Bytef*)inpair.first.get(), (uint32_t)inpair.second);
    }

    uint32_t adler32Buff(const buff_pair_t& inpair)
    {
        return adler32(0, (Bytef*)inpair.first.get(), (uint32_t)inpair.second);
    }
}