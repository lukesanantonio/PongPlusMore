/*!
 * \file render_text.cpp
 * \brief Definitions for everything in render_text.h.
 */
#include "render_text.h"
#include <vector>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
namespace pong
{
  void setupGrayscalePalette(SDL_Surface*& surface, std::size_t num_colors)
  {
    SDL_Color colors[num_colors];
    for(int i = 0; i < num_colors; ++i)
    {
      colors[i].r = i;
      colors[i].g = i;
      colors[i].b = i;
    }
    SDL_SetColors(surface, colors, 0, num_colors);
  }
  SDL_Surface* render_text(const std::string& text, std::size_t pixel_size,
                           SDL_Color text_color, SDL_Color background_color)
  {
    FT_Library library = nullptr;
    FT_Face face = nullptr;

    if(FT_Init_FreeType(&library))
    {
      //Ba!
      throw std::runtime_error("FreeType failed to initialize!");
    }

    if(FT_New_Face(library,
                   "/usr/share/fonts/truetype/ubuntu-font-family"
                                                            "/UbuntuMono-R.ttf",
                   0,
                   &face))
    {
      throw std::runtime_error("FreeType failed to load a new face!");
    }

    if(FT_Set_Pixel_Sizes(face, 0, pixel_size))
    {
      throw std::runtime_error("Failed to set pixel sizes in FreeType!");
    }

    //Find surface height and surface width.

    int max_top = 0;
    int max_bottom = 0;
    int width = 0;
    for(const char c : text)
    {
      if(FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME |
                               FT_LOAD_TARGET_MONO))
      {
        throw std::runtime_error("Failed to FT_LOAD_RENDER: " +
                                 std::to_string(c) + "!");
      }

      max_top = std::max(max_top, face->glyph->bitmap_top);
      max_bottom = std::max(max_bottom,
                            face->glyph->bitmap.rows - face->glyph->bitmap_top);
      width += face->glyph->advance.x / 64;
    }

    int height = max_top + max_bottom;

    //Now actually render the string of text.
    SDL_Surface* image = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                              width, height, 8,
                                              0, 0, 0, 0);
    if(!image)
    {
      throw std::runtime_error("Failed to initialize text surface!");
    }
    SDL_Color colors[] = {background_color, text_color};
    SDL_SetColors(image, colors, 0, 2);

    int pen_x = 0;
    //Set pen_y to the baseline...
    int pen_y = max_top;
    for(const char c : text)
    {
      if(FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME |
                               FT_LOAD_TARGET_MONO))
      {
        throw std::runtime_error("Failed to FT_LOAD_RENDER: " +
                                 std::to_string(c));
      }
      SDL_Surface* glyph = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                face->glyph->bitmap.width,
                                                face->glyph->bitmap.rows,
                                                8,
                                                0, 0, 0, 0);
      if(!glyph)
      {
        throw std::runtime_error("Failed to initialize glyph surface!");
      }
      SDL_SetColors(glyph, colors, 0, 2);

      SDL_LockSurface(glyph);
      {
        //Each pixel, a full byte, will only use it's first bit. A horrible
        //waste of memory, but I'm not sure if I can create an SDL_Surface*
        //with 8 pixels per byte.
        for(int row = 0; row < glyph->h; ++row)
        {
          for(int pix = 0; pix < glyph->w; ++pix)
          {
            uint8_t* sdlByte =
                static_cast<uint8_t*>(glyph->pixels) + row * glyph->pitch + pix;
            *sdlByte = static_cast<bool>(
                    face->glyph->bitmap.buffer[row * face->glyph->bitmap.pitch +
                                             (pix / 8)] & (1 << (7 - pix % 8)));
          }
        }
      }
      SDL_UnlockSurface(glyph);

      //Okay, so we have a solid glyph image, now blit it to the main surface.
      SDL_Rect dest;
      dest.x = pen_x;
      dest.y = pen_y - face->glyph->bitmap_top;
      SDL_BlitSurface(glyph, NULL, image, &dest);

      //Free our temp glyph surface.
      SDL_FreeSurface(glyph);

      //Update pen_x
      pen_x += face->glyph->advance.x / 64;
    }

    //We should be good, as in, we have our text... Now uninitialize FreeType
    //and then return our image.

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return image;
  }

  void invertPalette(SDL_Surface*& surface)
  {
    //Make our surface's palette easier to reference.
    SDL_Palette* palette = surface->format->palette;

    //Get a vector of all the colors...
    std::vector<SDL_Color> colors(palette->colors,
                                  palette->colors + palette->ncolors);
    //Reverse our colors.
    std::reverse(colors.begin(), colors.end());

    //Set our palette to the reversed copy of the original.
    SDL_SetColors(surface, &colors[0], 0, palette->ncolors);
  }

  SDL_Surface* generateRectangle(std::size_t width, std::size_t height,
                                 SDL_Color color)
  {
    SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                width, height, 8,
                                                0, 0, 0, 0);
    if(!surface)
    {
      throw std::runtime_error("Failed to create an SDL_Surface* with "
                               "SDL_CreateRGBSurface().");
    }
    SDL_SetColors(surface, &color, 0, 1);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format,
                                              color.r,
                                              color.g,
                                              color.b));
    return surface;
  }
};
