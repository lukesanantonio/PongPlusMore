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
#include "MenuGameState.h"
#include "common/crash.hpp"
#include "common/center.hpp"
#include "PaddleGameState.h"

namespace pong
{
  MoPongAnimation::MoPongAnimation(Game& game) noexcept
                                   : game_(game),
                                     pong_("Pong", 40, {})
  {
    this->pong_.font_renderer(game_.font_renderer.get());

    math::vector<int> pos;
    pos.x = game.width;
    pos.y = game.height - this->pong_.getSurfaceHeight();
    this->pong_.position(pos);
  }

  void MoPongAnimation::step() noexcept
  {
    if(!on_) return;

    if(step_now_)
    {
      auto new_pos = this->pong_.position();
      if(new_pos.x <= 0 - pong_.getSurfaceWidth())
      {
        on_ = false;
        return;
      }

      --new_pos.x;
      this->pong_.position(new_pos);
    }

    step_now_ = !step_now_;
  }

  std::vector<std::unique_ptr<RenderableObject> >
  MoPongAnimation::objects() const noexcept
  {
    std::vector<std::unique_ptr<RenderableObject> > objs;

    using renderable_object_type = RenderableObjectTemplate<Label>;

    using std::make_unique;
    objs.push_back(std::make_unique<renderable_object_type>(this->pong_));

    return objs;
  }
  MenuGameState::MenuGameState(Game& game)
                             : title_("Pong Plus More", 120, {0,0}),
                               singleplayer_("Singleplayer"),
                               multiplayer_("Multiplayer"),
                               options_("Options"),
                               quit_("Quit"),
                               anim_(game)
  {
    FontRenderer* font_renderer = &(*game.font_renderer);
    this->title_.font_renderer(font_renderer);

    this->title_.position({center(0, game.width,
                                  this->title_.getSurfaceWidth()),
                           50});

    int width = 750;
    int height = 90;
    int position_x = center(0, game.width, width);
    int starting_y = 280;

    for(Button* button : {&singleplayer_, &multiplayer_, &options_, &quit_})
    {
      button->volume({math::vector<double>(position_x, starting_y),
                     static_cast<double>(width), static_cast<double>(height)});
      button->font_renderer(font_renderer);
      button->enabled(false);
      button->background_color({0xff, 0xff, 0xff, 0xff});
      button->text_color({0x00, 0x00, 0x00, 0xff});
      starting_y += height + 50;
    }

    this->singleplayer_.enabled(true);
    this->singleplayer_.onClick([&]()
    {
      game.game_state = std::make_shared<PaddleGameState>();
    });

    this->quit_.enabled(true);
    this->quit_.onClick([&]()
    {
      game.exiting = true;
    });
  }

  void MenuGameState::handleEvent(const SDL_Event& event)
  {
    this->singleplayer_.handleEvent(event);
    this->multiplayer_.handleEvent(event);
    this->options_.handleEvent(event);
    this->quit_.handleEvent(event);
  }

  void MenuGameState::update()
  {
    this->anim_.step();
  }

  void MenuGameState::render(SDL_Renderer* renderer) const
  {
    this->title_.render(renderer);
    this->singleplayer_.render(renderer);
    this->multiplayer_.render(renderer);
    this->options_.render(renderer);
    this->quit_.render(renderer);

    for(auto&& ptr : this->anim_.objects())
    {
      // TODO: Better interface, ideally a dedicated render function which
      // wraps exactly this.
      ptr->render(renderer);
    }
  }
}
