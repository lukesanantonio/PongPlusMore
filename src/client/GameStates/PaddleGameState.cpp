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
#include <algorithm>
#include <random>
#include <fstream>
#include "PaddleGameState.h"
#include "../render.h"
#include "common/crash.hpp"
#include "server/collision_util.h"
#include "common/serialize.h"
namespace pong
{
  void PaddleGameState::handleEvent(const SDL_Event& event)
  {
    switch(event.type)
    {
      case SDL_MOUSEBUTTONDOWN:
      {
        if(top_) break;
        top_ = this->server_.createPaddle({{0, 0}, 200, 50});
        break;
      }
      case SDL_MOUSEMOTION:
      {
        if(!this->top_) break;
        this->server_.setDestination(this->top_,
                         math::vector<double>(event.motion.x, event.motion.y));
        break;
      }
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.scancode)
        {
          case SDL_SCANCODE_E:
          {
            this->render_quadtree_ = !this->render_quadtree_;
            break;
          }
          case SDL_SCANCODE_SPACE:
          {
            this->ball_ = this->server_.createBall({{500,500}, 25, 25});

            if(!this->ball_) break;

            // Get our mouse state.
            int x, y;
            SDL_GetMouseState(&x, &y);

            math::vector<double> click(x, y);

            math::vector<double> vel =
                                     click - math::vector<double>{500.0,500.0};
            vel = math::normalize(vel) * .25;

            this->server_.setVelocity(this->ball_, vel);
            break;
          }
          case SDL_SCANCODE_Q:
          {
            if(this->bottom_) break;
            bottom_ = this->server_.createPaddle({{0, 0}, 200, 50});
            break;
          }
          case SDL_SCANCODE_W:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).volume.pos;
            pos.y -= 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_A:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).volume.pos;
            pos.x -= 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_S:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).volume.pos;
            pos.y += 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_D:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).volume.pos;
            pos.x += 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_TAB:
          {
            // Print out the Object Manager.
            std::fstream obj_file("objectmanager.out",
                                  std::fstream::out | std::fstream::trunc);
            Json::StyledStreamWriter("  ").write(obj_file,
                                           dumpJSON(this->server_));

            // Print out the Quadtree.
            std::fstream qt_file("quadtree.out",
                                 std::fstream::out | std::fstream::trunc);
            Json::StyledStreamWriter("  ").write(qt_file,
                                           dumpJSON(this->server_.quadtree()));
            break;
          }
          default:
            break;
        }
        break;
      }
      case SDL_QUIT:
      {
        pong::crash("Exiting...");
        break;
      }
    }
  }

  void PaddleGameState::update()
  {
    this->server_.step();
  }

  void render_box(SDL_Renderer* renderer, const Volume& v)
  {
    constexpr int num_points = 5;
    SDL_Point points[num_points];

    auto from_point = [](math::vector<double> pos)
    {
      return SDL_Point{static_cast<int>(pos.x),
                       static_cast<int>(pos.y)};
    };

    auto pos = v.pos;

    points[0] = from_point(pos);

    pos.y = v.pos.y + v.height - 1;
    points[1] = from_point(pos);

    pos.x = v.pos.x + v.width - 1;
    points[2] = from_point(pos);

    pos.y = v.pos.y;
    points[3] = from_point(pos);

    pos = v.pos;
    points[4] = from_point(pos);

    SDL_RenderDrawLines(renderer, points, num_points);
  }

  void PaddleGameState::render(SDL_Renderer* renderer) const
  {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    for(id_type id : this->server_.objects())
    {
      pong::render(renderer, this->server_.getObject(id).volume);
    }

    if(this->render_quadtree_)
    {
      std::vector<Volume> vols_to_render;

      for(const Quadtree::node_type& n : *this->server_.quadtree().root())
      {
        vols_to_render.push_back(n.get_data()->v);
      }
      for(const Volume& v : vols_to_render)
      {
        render_box(renderer, v);
      }
    }
  }
}
