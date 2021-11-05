/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2021, Sam Atkins <atkinssj@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>
#include <AK/String.h>
#include <AK/Types.h>
#include <LibCore/AnonymousBuffer.h>
#include <LibCore/ConfigFile.h>
#include <LibGfx/Color.h>

namespace Gfx {

#define ENUMERATE_COLOR_ROLES(C)   \
    C(Accent)                      \
    C(ActiveLink)                  \
    C(ActiveWindowBorder1)         \
    C(ActiveWindowBorder2)         \
    C(ActiveWindowTitle)           \
    C(ActiveWindowTitleShadow)     \
    C(ActiveWindowTitleStripes)    \
    C(Base)                        \
    C(BaseText)                    \
    C(Button)                      \
    C(ButtonText)                  \
    C(DesktopBackground)           \
    C(FocusOutline)                \
    C(Gutter)                      \
    C(GutterBorder)                \
    C(HighlightWindowBorder1)      \
    C(HighlightWindowBorder2)      \
    C(HighlightWindowTitle)        \
    C(HighlightWindowTitleShadow)  \
    C(HighlightWindowTitleStripes) \
    C(HighlightSearching)          \
    C(HighlightSearchingText)      \
    C(HoverHighlight)              \
    C(InactiveSelection)           \
    C(InactiveSelectionText)       \
    C(InactiveWindowBorder1)       \
    C(InactiveWindowBorder2)       \
    C(InactiveWindowTitle)         \
    C(InactiveWindowTitleShadow)   \
    C(InactiveWindowTitleStripes)  \
    C(Link)                        \
    C(MenuBase)                    \
    C(MenuBaseText)                \
    C(MenuSelection)               \
    C(MenuSelectionText)           \
    C(MenuStripe)                  \
    C(MovingWindowBorder1)         \
    C(MovingWindowBorder2)         \
    C(MovingWindowTitle)           \
    C(MovingWindowTitleShadow)     \
    C(MovingWindowTitleStripes)    \
    C(PlaceholderText)             \
    C(RubberBandBorder)            \
    C(RubberBandFill)              \
    C(Ruler)                       \
    C(RulerActiveText)             \
    C(RulerBorder)                 \
    C(RulerInactiveText)           \
    C(Selection)                   \
    C(SelectionText)               \
    C(SyntaxComment)               \
    C(SyntaxControlKeyword)        \
    C(SyntaxIdentifier)            \
    C(SyntaxKeyword)               \
    C(SyntaxNumber)                \
    C(SyntaxOperator)              \
    C(SyntaxPreprocessorStatement) \
    C(SyntaxPreprocessorValue)     \
    C(SyntaxPunctuation)           \
    C(SyntaxString)                \
    C(SyntaxType)                  \
    C(TextCursor)                  \
    C(ThreedHighlight)             \
    C(ThreedShadow1)               \
    C(ThreedShadow2)               \
    C(Tooltip)                     \
    C(TooltipText)                 \
    C(Tray)                        \
    C(TrayText)                    \
    C(VisitedLink)                 \
    C(Window)                      \
    C(WindowText)

#define ENUMERATE_FLAG_ROLES(C) \
    C(IsDark)

#define ENUMERATE_METRIC_ROLES(C) \
    C(TitleHeight)                \
    C(TitleButtonWidth)           \
    C(TitleButtonHeight)

#define ENUMERATE_PATH_ROLES(C) \
    C(TitleButtonIcons)         \
    C(InactiveWindowShadow)     \
    C(ActiveWindowShadow)       \
    C(TaskbarShadow)            \
    C(MenuShadow)               \
    C(TooltipShadow)

enum class ColorRole {
    NoRole,

#undef __ENUMERATE_COLOR_ROLE
#define __ENUMERATE_COLOR_ROLE(role) role,
    ENUMERATE_COLOR_ROLES(__ENUMERATE_COLOR_ROLE)
#undef __ENUMERATE_COLOR_ROLE

        __Count,

    Background = Window,
    DisabledText = ThreedShadow1,
};

inline const char* to_string(ColorRole role)
{
    switch (role) {
    case ColorRole::NoRole:
        return "NoRole";
#undef __ENUMERATE_COLOR_ROLE
#define __ENUMERATE_COLOR_ROLE(role) \
    case ColorRole::role:            \
        return #role;
        ENUMERATE_COLOR_ROLES(__ENUMERATE_COLOR_ROLE)
#undef __ENUMERATE_COLOR_ROLE
    default:
        VERIFY_NOT_REACHED();
    }
}

enum class FlagRole {
    NoRole,

#undef __ENUMERATE_FLAG_ROLE
#define __ENUMERATE_FLAG_ROLE(role) role,
    ENUMERATE_FLAG_ROLES(__ENUMERATE_FLAG_ROLE)
#undef __ENUMERATE_FLAG_ROLE

        __Count,
};

inline const char* to_string(FlagRole role)
{
    switch (role) {
    case FlagRole::NoRole:
        return "NoRole";
#undef __ENUMERATE_FLAG_ROLE
#define __ENUMERATE_FLAG_ROLE(role) \
    case FlagRole::role:            \
        return #role;
        ENUMERATE_FLAG_ROLES(__ENUMERATE_FLAG_ROLE)
#undef __ENUMERATE_FLAG_ROLE
    default:
        VERIFY_NOT_REACHED();
    }
}

enum class MetricRole {
    NoRole,

#undef __ENUMERATE_METRIC_ROLE
#define __ENUMERATE_METRIC_ROLE(role) role,
    ENUMERATE_METRIC_ROLES(__ENUMERATE_METRIC_ROLE)
#undef __ENUMERATE_METRIC_ROLE

        __Count,
};

inline const char* to_string(MetricRole role)
{
    switch (role) {
    case MetricRole::NoRole:
        return "NoRole";
#undef __ENUMERATE_METRIC_ROLE
#define __ENUMERATE_METRIC_ROLE(role) \
    case MetricRole::role:            \
        return #role;
        ENUMERATE_METRIC_ROLES(__ENUMERATE_METRIC_ROLE)
#undef __ENUMERATE_METRIC_ROLE
    default:
        VERIFY_NOT_REACHED();
    }
}

enum class PathRole {
    NoRole,

#undef __ENUMERATE_PATH_ROLE
#define __ENUMERATE_PATH_ROLE(role) role,
    ENUMERATE_PATH_ROLES(__ENUMERATE_PATH_ROLE)
#undef __ENUMERATE_PATH_ROLE

        __Count,
};

inline const char* to_string(PathRole role)
{
    switch (role) {
    case PathRole::NoRole:
        return "NoRole";
#undef __ENUMERATE_PATH_ROLE
#define __ENUMERATE_PATH_ROLE(role) \
    case PathRole::role:            \
        return #role;
        ENUMERATE_PATH_ROLES(__ENUMERATE_PATH_ROLE)
#undef __ENUMERATE_PATH_ROLE
    default:
        VERIFY_NOT_REACHED();
    }
}

struct SystemTheme {
    RGBA32 color[(int)ColorRole::__Count];
    bool flag[(int)FlagRole::__Count];
    int metric[(int)MetricRole::__Count];
    char path[(int)PathRole::__Count][256]; // TODO: PATH_MAX?
};

Core::AnonymousBuffer& current_system_theme_buffer();
void set_system_theme(Core::AnonymousBuffer);
Core::AnonymousBuffer load_system_theme(Core::ConfigFile const&);
Core::AnonymousBuffer load_system_theme(String const& path);

}

using Gfx::ColorRole;
