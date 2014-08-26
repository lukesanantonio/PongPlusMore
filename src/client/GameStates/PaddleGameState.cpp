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
#include "common/serialize.h"
namespace pong
{
  void dump(std::string filename, Json::Value v) noexcept
  {
    std::fstream file(filename, std::fstream::out | std::fstream::trunc);
    Json::StyledStreamWriter("  ").write(file, v);
  }

  PaddleGameState::PaddleGameState(Game& g, Volume v,
                                   PaddleOrientation o) : g_(g), server_(v)
  {
    this->ball_ = this->server_.createBall({{475,475}, 25, 25});
    math::vector<double> velocity;
    velocity.x = (o == PaddleOrientation::Vertical ? 0.1 : 0.25);
    velocity.y = (o == PaddleOrientation::Vertical ? 0.25 : 0.1);
    this->server_.setVelocity(this->ball_, velocity);

    Volume paddle_volume;
    paddle_volume.width = (o == PaddleOrientation::Vertical ? 200 : 30);
    paddle_volume.height = (o == PaddleOrientation::Vertical ? 30 : 200);
    this->top_ = this->server_.createPaddle(paddle_volume);
    this->server_.setDestination(this->top_, paddle_volume.pos);

    paddle_volume.pos.x = (o == PaddleOrientation::Vertical ? 0 : 970);
    paddle_volume.pos.y = (o == PaddleOrientation::Vertical ? 970 : 0);
    this->bottom_ = this->server_.createPaddle(paddle_volume);
    this->server_.setDestination(this->bottom_, paddle_volume.pos);
  }
  void PaddleGameState::handleEvent(const SDL_Event& event)
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.scancode)
        {
          case SDL_SCANCODE_E:
          {
            this->render_quadtree_ = !this->render_quadtree_;
            break;
          }
          case SDL_SCANCODE_R:
          {
            this->render_constraints_ = !this->render_constraints_;
            break;
          }
          case SDL_SCANCODE_T:
          {
            g_.slow = !g_.slow;
            break;
          }
          case SDL_SCANCODE_Y:
          {
            ++g_.start_count;
            break;
          }
          case SDL_SCANCODE_U:
          {
            if(g_.start_count <= 1) break;
            --g_.start_count;
            break;
          }
          case SDL_SCANCODE_TAB:
          {
            // Print out the Object Manager and the quadtree.
            dump("objectmanager.out", dumpJSON(this->server_));
            dump("quadtree.out", dumpJSON(this->server_.quadtree()));
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
        dump("objectmanager.out", dumpJSON(this->server_));
        dump("quadtree.out", dumpJSON(this->server_.quadtree()));
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
      const Object& obj = this->server_.getObject(id);
      if(this->render_constraints_)
      {
        render_box(renderer, obj.volume);
        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
        render_volume_sides(renderer, obj.volume,
                            obj.physics_options.constraints, .2);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
      }
      else
      {
        pong::render(renderer, obj.volume);
      }
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
