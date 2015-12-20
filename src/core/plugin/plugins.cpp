/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "plugins.h"
#include "../common/utility.h"
#include "../common/result.h"

#include <msgpack.hpp>
namespace pong
{
  Msgpack_Plugin::Msgpack_Plugin(std::unique_ptr<External_IO> io) noexcept
                                 : io_(std::move(io))
  {
    io_->set_read_callback([this](const std::vector<char>& buf)
    {
      unpacker.reserve_buffer(buf.size());
      std::memcpy(unpacker.nonparsed_buffer(), &buf[0], buf.size());
    });
  }

  enum class Req_Err
  {
    NoArray,
    InvalidArraySize,
    BadFnType,
    BadParamsType,
    BadIdType,
    Unknown
  };

  Result<bool, Req_Err>
    try_make_request(Request& req, msgpack::object const& obj) noexcept
  {
    if(obj.type != msgpack::type::ARRAY)
    {
      return err(Req_Err::NoArray);
    }

    // So far so good, we have an array. It must have between 1 and 3 elements
    // (inclusive).
    if(obj.via.array.size < 1 || 3 < obj.via.array.size)
    {
      return err(Req_Err::InvalidArraySize);
    }

    // The first element must be an unsigned, positive integer.

    if(obj.via.array.ptr[0].type != msgpack::type::POSITIVE_INTEGER)
    {
      return err(Req_Err::BadFnType);
    }
    fn_t fn = obj.via.array.ptr[0].via.u64;

    // If we have a second parameter
    if(obj.via.array.size >= 2)
    {
      // The second element must either be an array or an id
      if(obj.via.array.ptr[1].type == msgpack::type::ARRAY)
      {
        Params p;
        p.object = msgpack::object(obj.via.array.ptr[1], p.zone);

        // Do we have a third parameter?
        if(obj.via.array.size >= 3)
        {
          if(obj.via.array.ptr[2].type != msgpack::type::POSITIVE_INTEGER)
          {
            return err(Req_Err::BadIdType);
          }

          req.fn = fn;
          req.id = obj.via.array.ptr[2].via.u64;

          req.params = std::move(p);
          return ok(true);
        }
        else
        {
          // That's that, no request id
          req.fn = fn;
          req.id = boost::none;
          req.params = std::move(p);
          return ok(true);
        }
      }
      else if(obj.via.array.ptr[1].type == msgpack::type::POSITIVE_INTEGER)
      {
        // Get the id then we are done, the client can't put params after the
        // id.

        req.fn = fn;
        req.id = obj.via.array.ptr[1].via.u64;
        req.params = boost::none;
        return ok(true);
      }
      else
      {
        return err(Req_Err::BadParamsType);
      }
    }
    // Just use the function id and assume no parameters, etc.
    else
    {
      req.fn = fn;
      req.id = boost::none;
      req.params = boost::none;
      // This is a little redundant since we know if there wasn't an error that
      // it was successful. That's alright for now, I feel.
      return ok(true);
    }
  }

  bool Msgpack_Plugin::poll_request(Request& req)
  {
    io_->step();

    msgpack::unpacked unpacked;

    // If we have one...
    if(unpacker.next(&unpacked))
    {
      // make sense of it and try to return it.
      msgpack::object obj = unpacked.get();

      if(*try_make_request(req, obj).ok())
      {
        return true;
      }
      return false;
    }

    // Otherwise just bail out
    return false;
  }

  void Msgpack_Plugin::post_request(Request const& res) noexcept
  {
    std::ostringstream buf;
    msgpack::packer<std::ostringstream> packer(buf);

    if(res.params && res.id)
    {
      packer.pack_array(3);
      packer.pack(res.fn);
      packer.pack(res.params->object);
      packer.pack(*res.id);
    }
    else if(res.params && !res.id)
    {
      packer.pack_array(2);
      packer.pack(res.fn);
      packer.pack(res.params->object);
    }
    else if(!res.params && res.id)
    {
      packer.pack_array(2);
      packer.pack(res.fn);
      packer.pack(*res.id);
    }
    // They both must be missing
    else
    {
      packer.pack_array(1);
      packer.pack(res.fn);
    }
    io_->write(vec_from_string(buf.str()));
    io_->step();
  }
}
