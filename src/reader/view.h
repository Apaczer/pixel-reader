#ifndef VIEW_H_
#define VIEW_H_

#include <SDL/SDL_keysym.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

class View
{
public:
    // Returns true if rendering was performed.
    virtual bool render(SDL_Surface *dest, bool force_render) = 0;

    // Return true if the view is no longer needed.
    virtual bool is_done() = 0;

    // Key down event.
    virtual void on_keypress(SDLKey key) = 0;

    // Pass key and held time in ms.
    virtual void on_keyheld(SDLKey, uint32_t) {}

    // This view has been popped from the stack (now defunct).
    virtual void on_pop() {}
};

#endif
