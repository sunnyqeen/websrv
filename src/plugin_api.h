/* Copyright (C) 2025 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#pragma once

#include <microhttpd.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Native plugins are shared objects named <soname>.so. websrv resolves:
 *
 *   <soname>_plugin_register_url
 *   <soname>_plugin_handle_request
 *
 * For example, demo.so must export demo_plugin_register_url and
 * demo_plugin_handle_request.
 *
 * <soname>_plugin_register_url — return the URL prefix without a leading
 * slash, e.g. "plugin/demo" (serves /plugin/demo and sub-paths).
 *
 * <soname>_plugin_handle_request — handle HTTP requests routed to that
 * prefix. For POST, @p post is a linked list of parsed form fields (or NULL
 * for GET, HEAD, and POST with no body).
 */
typedef struct plugin_post_data {
  const char* key;
  const uint8_t* val;
  size_t len;
  struct plugin_post_data* next;
} plugin_post_data_t;
