/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
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
#include "gtest/gtest.h"
#include "core/io/external_io.h"
#include "core/plugin/plugins.h"
#include "core/common/utility.h"
#include "core/common/pif/helper.h"

std::vector<char> write_json_value(Json::Value const& json) noexcept
{
  Json::FastWriter write;
  auto str = write.write(json);
  return std::vector<char>(str.begin(), str.end());
}

TEST(Json_Plugin_Tests, Sanity)
{
  using namespace pong::literals;
  using pong::Json_Plugin;
  using pong::Pipe_IO;
  using pong::Request;
  using pong::Response;

  Json_Plugin plugin(std::make_unique<Pipe_IO>());
  Pipe_IO* pipe = static_cast<Pipe_IO*>(&plugin.io());

  Request req;
  req.id = 1;
  req.method = "Test";

  auto req_json = write_json_value(FORMATTER_TYPE(Request)::dump(req));
  pipe->counterpart().write(req_json);

  Request result_req;
  EXPECT_TRUE(plugin.poll_request(result_req));
  EXPECT_EQ(req, result_req);

  Response res;
  res.id = req.id;
  res.result = ".";

  auto res_json = write_json_value(FORMATTER_TYPE(Response)::dump(res));
  pipe->counterpart().set_read_callback([&](std::vector<char> const& buf)
  {
    EXPECT_EQ(res_json, buf);
  });

  plugin.post_response(res);
}

struct Err_Res_Code_Test_Visitor : public boost::static_visitor<bool>
{
  Err_Res_Code_Test_Visitor(int code) noexcept : code_(code) {}
  bool operator()(pong::Error_Response const& err) const noexcept
  {
    return code_ == err.code;
  }
  template <typename T>
  bool operator()(T const& els) const noexcept
  {
    return false;
  }
private:
  int code_;
};

TEST(Json_Plugin_Tests, Parse_Error)
{
  using namespace pong::literals;
  using pong::Json_Plugin;
  using pong::Pipe_IO;
  using pong::Request;
  using pong::Response;

  Json_Plugin plugin(std::make_unique<Pipe_IO>());
  Pipe_IO* pipe = static_cast<Pipe_IO*>(&plugin.io());

  // Write some invalid json.
  pipe->counterpart().write(R"({"id":1 "method":test"})"_buf);
  pipe->counterpart().set_read_callback([&](std::vector<char> const& buf)
  {
    Json::Reader read;
    Json::Value val;
    // Expect the json parse to be successful.
    EXPECT_TRUE(read.parse(std::string(buf.begin(), buf.end()), val));

    pong::Response res = FORMATTER_TYPE(Response)::parse(val);
    // The id should be null.
    EXPECT_TRUE(pong::is_null(res.id));
    // The error code should be -32700.
    EXPECT_TRUE(boost::apply_visitor(Err_Res_Code_Test_Visitor(-32700),
                res.result));
  });

  pong::Request req;
  EXPECT_FALSE(plugin.poll_request(req));
}
TEST(Json_Plugin_Tests, Invalid_Request_Error)
{
  using namespace pong::literals;
  using pong::Json_Plugin;
  using pong::Pipe_IO;

  Json_Plugin plugin(std::make_unique<Pipe_IO>());
  Pipe_IO& json_input = static_cast<Pipe_IO*>(&plugin.io())->counterpart();

  json_input.write(R"({"id":1, "params":[5, 1, 5]})"_buf);
  json_input.set_read_callback([&](std::vector<char> const& buf)
  {
    Json::Reader read;
    Json::Value val;

    EXPECT_TRUE(read.parse(std::string(buf.begin(), buf.end()), val));
    pong::Response res = FORMATTER_TYPE(pong::Response)::parse(val);

    EXPECT_TRUE(pong::req_id_t(1) == res.id.value());
    EXPECT_TRUE(boost::apply_visitor(Err_Res_Code_Test_Visitor(-32600),
                res.result));
  });

  pong::Request req;
  EXPECT_FALSE(plugin.poll_request(req));
}
