
#ifndef __PROTO_H__
#define __PROTO_H__

#define PROTO_VERSION 1

typedef uint8_t msgType_t;      /** message type. See message_t.type */

/** various message types
 * Note: #define, because enum is too big.
 */
#define T_COMMAND 0
#define T_DATA_WII 1
#define T_DATA_JOY 2

/** Message send from client to server
 */
typedef struct message {
   uint8_t   version;   /**< protocol version */
   uint8_t   size;      /**< message size */
   msgType_t type;      /**< message type. will define the payload */

   /** messager payload. depends on control device (joystick, nunchuk, android ...)
    */
   union {
      char txt[17];     /**< command text */

      /** Nunchuk payload
       */
      struct {
         uint8_t b1;    /**< button A */
         uint8_t b2;    /**< button Z */
         float   x;     /**< accel X */
         float   y;     /**< accel Y */
         float   z;     /**< accel Z */
      } nunchuk;

      /** joystick payload
       */
      struct {
         uint8_t b1;    /**< button 1 */
         uint8_t b2;    /**< button 2 */
         uint8_t b3;    /**< button 3 */
         uint8_t b4;    /**< button 4 */
         int x1;        /**< axis 1 X */
         int x2;        /**< axis 2 X */
         int y1;        /**< axis 1 Y */
         int y2;        /**< axis 2 Y */
      } joystick;

      /** android payload
       */
      struct {
         uint8_t b1;    /**< button 1 */
         uint8_t b2;    /**< button 2 */
         uint8_t b3;    /**< button 3 */
         uint8_t b4;    /**< button 4 */
         uint8_t b5;    /**< button 5 */
         int x;         /**< axis X * 100 */
         int y;         /**< axis Y * 100 */
         int z;         /**< axis Z * 100 */
      } android;

   } pl;
} message_t;

#define HEADER_SIZE (size_t)(sizeof(uint8_t)+sizeof(uint8_t)+sizeof(msgType_t))

#endif /* __PROTO_H__ */
