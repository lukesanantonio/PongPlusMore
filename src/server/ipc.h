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
#pragma once
#include <uv.h>
#include <vector>
#include "Server.h"
namespace pong
{
  struct Process;
  struct Pipe
  {
    uv_pipe_t pipe;
    std::vector<char>* buf;
    Process* proc;
  };

  Pipe* create_pipe(Process* = nullptr) noexcept;
  void delete_pipe(Pipe*) noexcept;

  using on_write_cb = void (*)(Pipe*);
  void write_buffer(Pipe* pipe, on_write_cb after_write = nullptr) noexcept;

  struct Process
  {
    uv_process_t proc;
    Pipe* out_pipe;
    Pipe* in_pipe;
    Pipe* err_pipe;
    Server* server;
    uv_loop_t* loop;
  };

  struct SpawnOptions
  {
    char** args;
    const char* cwd;
  };

  Process* create_process(uv_loop_t* loop,
                          const SpawnOptions& spawn_opt,
                          Server& server) noexcept;
  void delete_process(Process*) noexcept;
};
