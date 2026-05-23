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

#include <stdio.h>
#include <string.h>

#include <microhttpd.h>

#include "plugin_api.h"


static const char PAGE[] =
  "<html><head><title>websrv plugin demo</title></head>"
  "<body>"
  "<h1>plugin/demo</h1>"
  "<form method=\"POST\">"
  "<label>msg <input name=\"msg\" value=\"hello\"></label>"
  "<button type=\"submit\">POST</button>"
  "</form>"
  "</body></html>";


static enum MHD_Result
demo_queue_response(struct MHD_Connection* conn, unsigned int status,
                    const char* mime, const char* body, size_t body_len) {
  struct MHD_Response* resp;
  enum MHD_Result ret = MHD_NO;

  if((resp=MHD_create_response_from_buffer(body_len, (void*)body,
                                           MHD_RESPMEM_MUST_COPY))) {
    MHD_add_response_header(resp, MHD_HTTP_HEADER_CONTENT_TYPE, mime);
    MHD_add_response_header(resp, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN,
                            "*");
    ret = MHD_queue_response(conn, status, resp);
    MHD_destroy_response(resp);
  }

  return ret;
}


const char*
demo_plugin_register_url(void) {
  return "plugin/demo";
}


enum MHD_Result
demo_plugin_handle_request(struct MHD_Connection* conn, const char* url,
                           const char* method, const plugin_post_data_t* post) {
  char body[4096];
  size_t len;

  (void)url;

  if(!strcmp(method, MHD_HTTP_METHOD_GET) ||
     !strcmp(method, MHD_HTTP_METHOD_HEAD)) {
    return demo_queue_response(conn, MHD_HTTP_OK, "text/html", PAGE,
                               sizeof(PAGE) - 1);
  }

  if(!strcmp(method, MHD_HTTP_METHOD_POST)) {
    int first = 1;

    len = snprintf(body, sizeof(body), "{\"fields\":{");
    for(; post && len < sizeof(body) - 64; post=post->next) {
      if(!first) {
        len += snprintf(body + len, sizeof(body) - len, ",");
      }
      first = 0;
      len += snprintf(body + len, sizeof(body) - len, "\"%s\":\"", post->key);
      if(post->val && post->len) {
        len += snprintf(body + len, sizeof(body) - len, "%.*s",
                        (int)post->len, (const char*)post->val);
      }
      len += snprintf(body + len, sizeof(body) - len, "\"");
    }
    len += snprintf(body + len, sizeof(body) - len, "}}");
    return demo_queue_response(conn, MHD_HTTP_OK, "application/json",
                               body, len);
  }

  return MHD_NO;
}
