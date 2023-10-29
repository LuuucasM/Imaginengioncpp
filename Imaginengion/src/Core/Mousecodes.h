#pragma once

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define IMAGINE_MOD_SHIFT           0x0001
 /*! @brief If this bit is set one or more Control keys were held down.
  *
  *  If this bit is set one or more Control keys were held down.
  */
#define IMAGINE_MOD_CONTROL         0x0002
  /*! @brief If this bit is set one or more Alt keys were held down.
   *
   *  If this bit is set one or more Alt keys were held down.
   */
#define IMAGINE_MOD_ALT             0x0004
   /*! @brief If this bit is set one or more Super keys were held down.
	*
	*  If this bit is set one or more Super keys were held down.
	*/
#define IMAGINE_MOD_SUPER           0x0008
	/*! @brief If this bit is set the Caps Lock key is enabled.
	 *
	 *  If this bit is set the Caps Lock key is enabled and the @ref
	 *  IMAGINE_LOCK_KEY_MODS input mode is set.
	 */
#define IMAGINE_MOD_CAPS_LOCK       0x0010
	 /*! @brief If this bit is set the Num Lock key is enabled.
	  *
	  *  If this bit is set the Num Lock key is enabled and the @ref
	  *  IMAGINE_LOCK_KEY_MODS input mode is set.
	  */
#define IMAGINE_MOD_NUM_LOCK        0x0020

	  /*! @} */

	  /*! @defgroup buttons Mouse buttons
	   *  @brief Mouse button IDs.
	   *
	   *  See [mouse button input](@ref input_mouse_button) for how these are used.
	   *
	   *  @ingroup input
	   *  @{ */
#define IMAGINE_MOUSE_BUTTON_1         0
#define IMAGINE_MOUSE_BUTTON_2         1
#define IMAGINE_MOUSE_BUTTON_3         2
#define IMAGINE_MOUSE_BUTTON_4         3
#define IMAGINE_MOUSE_BUTTON_5         4
#define IMAGINE_MOUSE_BUTTON_6         5
#define IMAGINE_MOUSE_BUTTON_7         6
#define IMAGINE_MOUSE_BUTTON_8         7
#define IMAGINE_MOUSE_BUTTON_LAST      IMAGINE_MOUSE_BUTTON_8
#define IMAGINE_MOUSE_BUTTON_LEFT      IMAGINE_MOUSE_BUTTON_1
#define IMAGINE_MOUSE_BUTTON_RIGHT     IMAGINE_MOUSE_BUTTON_2
#define IMAGINE_MOUSE_BUTTON_MIDDLE    IMAGINE_MOUSE_BUTTON_3