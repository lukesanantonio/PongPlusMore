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

#include <chrono>
#include <thread>

#include "json/json.h"

#include "core/plugin/req.h"

int main(int argc, char** argv)
{
  // Log our presence... For the hell of it!
  pong::Request req = pong::Log_Req(0, pong::Severity::Info, "HELLO!");
  Json::FastWriter write;

  std::string out = write.write(FORMATTER_TYPE(pong::Request)::dump(req));
  std::cout << out << std::flush;

  // Wait a couple of seconds.
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Make the exit request.
  req = pong::Exit_Req(0);

  // Dump the json to a string.
  out = write.write(FORMATTER_TYPE(pong::Request)::dump(req));

  // Send that request to the engine.
  std::cout << out;
  return 0;
}
