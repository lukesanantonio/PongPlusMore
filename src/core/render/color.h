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
#pragma once
#include <cstdint>
#include "core/common/pif/helper.h"
namespace ug
{
  struct Color
  {
    uint8_t r, g, b, a;

    DECLARE_FORMATTED_AS_OBJECT;
    DECLARE_PROPERTIES_TUPLE(uint8_t, uint8_t, uint8_t, uint8_t);
    DECLARE_PROPERTIES(r, g, b, a);
    DECLARE_PROPERTY_VALUES(4, "r", "g", "b", "a");
  };
}
