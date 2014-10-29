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
#include <iostream>
#include <chrono>
#include <thread>

#include "core/plugin/req.h"
#include "core/common/vector.h"
#include "core/common/pif/helper.h"

void send_request(pong::Request const& req)
{
  Json::FastWriter write;
  std::string str = write.write(FORMATTER_TYPE(pong::Request)::dump(req));
  std::cout << str << std::flush;
}

int main(int argc, char** argv)
{
  // Sleep a bit before starting.
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Start the engine with 1000x1000 window.
  pong::Request req;
  req.method = "Core.Start";

  req.params = Json::Value(Json::arrayValue);

  namespace math = pong::math;
  const math::vector<int> bounds = {1000, 1000};
  req.params.value().append(FORMATTER_TYPE(math::vector<int>)::dump(bounds));
  req.params.value().append("Pong Plus More");

  send_request(req);

  // Wait for 2 seconds before exiting.
  std::this_thread::sleep_for(std::chrono::seconds(2));

  req.method = "Core.Exit";
  req.params = boost::none_t{};
  send_request(req);
}
