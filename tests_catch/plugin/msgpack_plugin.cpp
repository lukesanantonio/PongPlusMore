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
//#include <boost/optional/optional_io.hpp>
#include "catch/catch.hpp"
#include "io/external_io.h"
#include "rpc/plugins.h"
#include "rpc/dispatch.h"
#include "common/utility.h"

TEST_CASE("Msgpack plugin", "[rpclib]")
{
  auto out_pipe = std::make_unique<ug::Pipe_IO>();
  auto& write_pipe = out_pipe->counterpart();

  ug::Msgpack_Plugin plugin{std::move(out_pipe)};

  using namespace ug::literals;

  // Json: [0]
  write_pipe.write("\x91\x00"_buf);

  // Json: [0, []]
  write_pipe.write("\x92\x00\x90"_buf);

  // Json: [0, [], 1]
  write_pipe.write("\x93\x00\x90\x01"_buf);

  // Json: [2, ["Hello"], 1]
  write_pipe.write("\x93\x02\x91\xa5\x48\x65\x6c\x6c\x6f\x01"_buf);

  // Json: [2, ["Hello"]]
  write_pipe.write("\x92\x02\x91\xa5\x48\x65\x6c\x6c\x6f"_buf);

  // Json: [3, 2]
  write_pipe.write("\x92\x03\x02"_buf);

  write_pipe.step();

  ug::Request req;
  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 0);
  CHECK_FALSE(req.id);
  CHECK_FALSE(req.params);

  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 0);
  CHECK_FALSE(req.id);
  CHECK(req.params);
  REQUIRE(req.params->object.get().type == msgpack::type::ARRAY);
  REQUIRE(req.params->object.get().via.array.size == 0);

  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 0);
  CHECK(req.id);
  CHECK(req.params);
  REQUIRE(req.params->object.get().type == msgpack::type::ARRAY);
  REQUIRE(req.params->object.get().via.array.size == 0);

  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 2);
  CHECK(req.id);
  CHECK(req.params);
  REQUIRE(req.params->object.get().type == msgpack::type::ARRAY);
  REQUIRE(req.params->object.get().via.array.size == 1);
  REQUIRE(req.params->object.get().via.array.ptr[0].type == msgpack::type::STR);

  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 2);
  CHECK_FALSE(req.id);
  CHECK(req.params);
  REQUIRE(req.params->object.get().type == msgpack::type::ARRAY);
  REQUIRE(req.params->object.get().via.array.size == 1);
  REQUIRE(req.params->object.get().via.array.ptr[0].type == msgpack::type::STR);

  REQUIRE(plugin.poll_request(req));
  CHECK(req.fn == 3);
  CHECK(req.id);
  CHECK_FALSE(req.params);
}
TEST_CASE("Request dispatcher", "[rpclib]")
{
  int var0 = 0;
  int var1 = 0;

  auto func0 = [&var0](auto* run_context, auto& params)
  {
    var0 = 5;
  };
  auto func1 = [&var1](auto* run_context, auto& params)
  {
    ++var1;
  };

  std::vector<ug::method_t> methods{func0, func1};

  auto in = 5;
  ug::Request req;
  req.fn = 0;
  req.params = ug::Params{};

  msgpack::zone zone;
  std::vector<int> in_params{in};
  req.params->object = msgpack::clone(msgpack::object(in_params, zone));

  ug::dispatch(methods, req);
  REQUIRE(var0 == in);

  req.fn = 1;
  req.params = boost::none;

  ug::dispatch(methods, req);
  ug::dispatch(methods, req);
  REQUIRE(var1 == 2);
}
