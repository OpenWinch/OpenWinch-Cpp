/**
 * @file constantes.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef CONSTANTES_HPP_
#define CONSTANTES_HPP_

/* Hardware */
#define MOTOR_MAX 38          // DEPRECATED prefere to use SPEED_MAX
#define MOTOR_PPR 18          // pulse by turn 45 on gear (but not from motor)
#define MOTOR_REDUCE (54/11)

/* LCD */
#define LCD_FPS 10
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

/* Diameter in meter. */
#define WINCH_DIAM 0.2

/* Distance in meter. */
#define WINCH_DISTANCE 300

/* Speed */
#define SPEED_MAX 38
#define SPEED_INIT 30
#define SPEED_MIN 1
#define SPEED_UNIT "Km/h"

/* Loop delay in milisecond*/
#define LOOP_DELAY 100
// #define LOOP_DELAY 10

#endif  // CONSTANTES_HPP_
