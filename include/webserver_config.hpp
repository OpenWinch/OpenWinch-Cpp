/*
  webserver_config.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef WEBSERVER_CONFIG_HPP_
#define WEBSERVER_CONFIG_HPP_

#define HTTP_KEEPALIVE_MAX 3

#define HTTP_PAYLOAD_MAX 1024 * 1024 * 1 /* 1MB */

#define CPPHTTPLIB_THREAD_POOL_COUNT 5

#define HTTP_ROUTE_EXTRA "/extra"
#define HTTP_ROUTE_INDEX "/index"

#endif  // WEBSERVER_CONFIG_HPP_
