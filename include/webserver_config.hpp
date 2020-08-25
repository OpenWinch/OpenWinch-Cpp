/**
 * @file webserver_config.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef WEBSERVER_CONFIG_HPP_
#define WEBSERVER_CONFIG_HPP_

#define HTTP_KEEPALIVE_MAX 3

#define HTTP_PAYLOAD_MAX 1024 * 1024 * 1 /* 1MB */

#define CPPHTTPLIB_THREAD_POOL_COUNT 5

#define HTTP_ROUTE_EXTRA "/extra"
#define HTTP_ROUTE_INDEX "/index"

#endif  // WEBSERVER_CONFIG_HPP_
