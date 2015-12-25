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
#include "external_io.h"
#include "common.h"
#include "../common/utility.h"
namespace ug
{
  void post_pipe_buffer(ipc::Pipe* p) noexcept
  {
    External_IO* io = (External_IO*) p->user_data;
    io->post(*p->buf);
  }
  void post_error_from_buffer(ipc::Pipe* p) noexcept
  {
    External_IO* io = (External_IO*) p->user_data;
    io->post_error(*p->buf);
  }

  Child_Process::Child_Process(ipc::Spawn_Options& opt)
  {
    uv_loop_init(&loop_);

    process_ = ipc::create_process(&loop_, opt);

    process_->io.in.user_data = this;
    process_->io.in.action_cb = post_pipe_buffer;
    uv_read_start((uv_stream_t*) &process_->io.in, alloc, ipc::collect_lines);

    process_->err.user_data = this;
    process_->err.action_cb = post_error_from_buffer;
    uv_read_start((uv_stream_t*) &process_->err, alloc, ipc::collect_lines);

    // Start the boll rollin'!
    *process_->io.out.buf = vec_from_string("PpM");
    ipc::write_buffer(&process_->io.out);
  }
  Child_Process::~Child_Process() noexcept
  {
    ipc::delete_process(process_);

    uv_run(&loop_, UV_RUN_DEFAULT);
    uv_loop_close(&loop_);
  }
  void Child_Process::write(std::vector<uchar> const& buf) noexcept
  {
    step();
    if(!process_->running) return;
    *process_->io.out.buf = buf;
    ipc::write_buffer(&process_->io.out);
  }
  void Child_Process::step() noexcept
  {
    uv_run(&loop_, UV_RUN_NOWAIT);
  }

  void post_net_buffer(net::Pipe* pipe)
  {
    Net_IO* io = (Net_IO*) pipe->user_data;
    io->post(*pipe->out.buf);
  }

  Net_IO::Net_IO(std::string const& bind_ip,
                 uint16_t const bind_port,
                 std::string const& write_ip,
                 uint16_t const write_port) noexcept
  {
    // Initialize the loop
    uv_loop_init(&loop_);
    // Initialize the network pipe.
    net::init_pipe(pipe_, &loop_, bind_ip, bind_port);
    pipe_.read_cb = post_net_buffer;

    // Initialize the address that will be written to.
    uv_ip4_addr(write_ip.c_str(), write_port, &write_addr_);
  }
  Net_IO::~Net_IO() noexcept
  {
    // Uninitialize the net pipe
    net::uninit_pipe(pipe_);
    // Uninit the loop
    uv_loop_close(&loop_);
  }

  void Net_IO::write(std::vector<uchar> const& buf) noexcept
  {
    *pipe_.out.buf = buf;
    net::write_buffer(pipe_, (struct sockaddr*) &write_addr_);
  }
  void Net_IO::step() noexcept
  {
    uv_run(&loop_, UV_RUN_NOWAIT);
  }

  Pipe_IO::Pipe_IO() noexcept
    : cp_(make_owned_maybe(new Pipe_IO(*this))) {}

  Pipe_IO::Pipe_IO(Pipe_IO& io) noexcept
    : cp_(&io, false) {}

  Pipe_IO& Pipe_IO::counterpart() noexcept
  {
    return *cp_;
  }

  void Pipe_IO::write(std::vector<uchar> const& buf) noexcept
  {
    // Write to the counterpart's input.
    counterpart().input_.push(buf);
  }
  void Pipe_IO::step() noexcept
  {
    step_(true);
  }
  void Pipe_IO::step_(bool recursive) noexcept
  {
    // Read our pending input.
    while(!input_.empty())
    {
      std::vector<uchar> const& buf = input_.front();
      post(buf);
      input_.pop();
    }

    // Let our counterpart do the same.
    if(recursive)
    {
      counterpart().step_(false);
    }
  }
}
