#include "./selection_menu.h"

#include "sys/screen.h"
#include "sys/keymap.h"
#include "reader/sdl_utils.h"
#include "reader/system_styling.h"

SelectionMenu::SelectionMenu(SystemStyling &styling, TTF_Font *font)
    : SelectionMenu({}, styling, font)
{
}

SelectionMenu::SelectionMenu(std::vector<std::string> entries, SystemStyling &styling, TTF_Font *font)
    : entries(entries),
      font(font),
      styling(styling),
      styling_sub_id(styling.subscribe_to_changes([this]() {
          needs_render = true;
      })),
      line_height(detect_line_height(font)),
      num_display_lines((SCREEN_HEIGHT - line_padding) / (line_height + line_padding)),
      scroll_throttle(250, 100)
{
}

SelectionMenu::~SelectionMenu()
{
    styling.unsubscribe_from_changes(styling_sub_id);
}

void SelectionMenu::set_entries(std::vector<std::string> new_entries)
{
    entries = new_entries;
    set_cursor_pos(0);
    needs_render = true;
}

void SelectionMenu::set_on_selection(std::function<void(uint32_t)> callback)
{
    on_selection = callback;
}

void SelectionMenu::set_on_focus(std::function<void(uint32_t)> callback)
{
    on_focus = callback;
}

void SelectionMenu::set_default_on_keypress(std::function<void(SDLKey)> callback)
{
    default_on_keypress = callback;
}

void SelectionMenu::set_close_on_select()
{
    close_on_select = true;
}

void SelectionMenu::set_cursor_pos(const std::string &entry)
{
    for (uint32_t i = 0; i < entries.size(); ++i)
    {
        if (entries[i] == entry)
        {
            set_cursor_pos(i);
            break;
        }
    }
}

void SelectionMenu::set_cursor_pos(uint32_t new_cursor_pos)
{
    if (new_cursor_pos >= entries.size())
    {
        new_cursor_pos = 0;
    }

    cursor_pos = new_cursor_pos;
    if (on_focus)
    {
        on_focus(cursor_pos);
    }

    if (entries.size() <= num_display_lines)
    {
        scroll_pos = 0;
    }
    else
    {
        scroll_pos = std::max(
            0,
            static_cast<int>(new_cursor_pos) - (static_cast<int>(num_display_lines) / 2 - 1)
        );
    }

    needs_render = true;
}

void SelectionMenu::close()
{
    _is_done = true;
}

bool SelectionMenu::render(SDL_Surface *dest_surface, bool force_render)
{
    if (!needs_render && !force_render)
    {
        return false;
    }
    needs_render = false;

    const SDL_PixelFormat *pixel_format = dest_surface->format;

    const auto &theme = styling.get_loaded_color_theme();
    const SDL_Color &fg_color = theme.main_text;
    const SDL_Color &bg_color = theme.background;
    const SDL_Color &hl_color = theme.highlight;

    uint32_t rect_bg_color = SDL_MapRGB(pixel_format, bg_color.r, bg_color.g, bg_color.b);
    uint32_t rect_highlight_color = SDL_MapRGB(pixel_format, hl_color.r, hl_color.g, hl_color.b);

    Sint16 x = line_padding;
    Sint16 y = line_padding;

    // Clear screen
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_FillRect(dest_surface, &rect, rect_bg_color);

    // Draw lines
    for (uint32_t i = 0; i < num_display_lines; ++i)
    {
        uint32_t global_i = i + scroll_pos;
        if (global_i >= entries.size())
        {
            break;
        }

        const auto &entry = entries[global_i];

        bool is_highlighted = (global_i == cursor_pos);

        // Draw hightlight
        if (is_highlighted)
        {
            SDL_Rect rect = {0, (Sint16)(y - line_padding / 2), SCREEN_WIDTH, (Uint16)(line_height + line_padding)};
            SDL_FillRect(dest_surface, &rect, rect_highlight_color);
        }

        // Draw text
        {
            SDL_Rect rectMessage = {x, y, 0, 0};
            SDL_Surface *message = TTF_RenderUTF8_Shaded(font, entry.c_str(), fg_color, is_highlighted ? hl_color : bg_color);
            SDL_BlitSurface(message, NULL, dest_surface, &rectMessage);
            SDL_FreeSurface(message);
        }

        y += line_height + line_padding;
    }

    return true;
}

bool SelectionMenu::is_done()
{
    return _is_done;
}

void SelectionMenu::on_move_down(uint32_t step)
{
    if (cursor_pos < entries.size() - 1)
    {
        cursor_pos = std::min(
            cursor_pos + step,
            static_cast<uint32_t>(entries.size()) - 1
        );

        if (cursor_pos >= scroll_pos + num_display_lines)
        {
            scroll_pos = cursor_pos - num_display_lines + 1;
        }
        if (on_focus)
        {
            on_focus(cursor_pos);
        }

        needs_render = true;
    }
}

void SelectionMenu::on_move_up(uint32_t step)
{
    if (cursor_pos > 0)
    {
        cursor_pos = cursor_pos <= step ? 0 : cursor_pos - step;
        scroll_pos = std::min(scroll_pos, cursor_pos);

        if (on_focus)
        {
            on_focus(cursor_pos);
        }

        needs_render = true;
    }
}

void SelectionMenu::on_select_entry()
{
    if (!entries.empty() && on_selection)
    {
        on_selection(cursor_pos);
    }

    if (close_on_select)
    {
        _is_done = true;
    }
}

void SelectionMenu::on_keypress(SDLKey key)
{
    switch (key) {
        case SW_BTN_UP:
            on_move_up(1);
            break;
        case SW_BTN_DOWN:
            on_move_down(1);
            break;
        case SW_BTN_LEFT:
        case SW_BTN_L1:
            on_move_up(num_display_lines / 2);
            break;
        case SW_BTN_RIGHT:
        case SW_BTN_R1:
            on_move_down(num_display_lines / 2);
            break;
        case SW_BTN_A:
            on_select_entry();
            break;
        case SW_BTN_B:
            _is_done = true;
            break;
        default:
            if (default_on_keypress)
            {
                default_on_keypress(key);
            }
            break;
    }
}

void SelectionMenu::on_keyheld(SDLKey key, uint32_t held_time_ms)
{
    switch (key) {
        case SW_BTN_UP:
        case SW_BTN_DOWN:
        case SW_BTN_LEFT:
        case SW_BTN_RIGHT:
        case SW_BTN_L1:
        case SW_BTN_R1:
            if (scroll_throttle(held_time_ms))
            {
                on_keypress(key);
            }
            break;
        default:
            break;
    }
}
