#     Copyright (C) 2026 Valtteri Viirret
#     This file is part of the Nexilis Project.
#
#     This file is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
#
#     This file is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public License
#     along with this file.  If not, see <https://gnu.org>.


string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(ColorBold "${Esc}[1m")
set(ColorRed "${Esc}[31m")
set(ColorGreen "${Esc}[32m")
set(ColorYellow "${Esc}[33m")
set(ColorBlue "${Esc}[34m")

function(colorized_status label value)
    if(value)
        set(color ${ColorGreen})
        set(display_value "ON")
    else()
        set(color ${ColorRed})
        set(display_value "OFF")
    endif()
    message(STATUS "${ColorBold} ${label}:${ColorReset} ${color}${display_value}${ColorReset}")
endfunction()

function(print_nexilis_status)
    message(STATUS "${ColorBold} Version: ${NEXILIS_VERSION}${ColorReset}")
    colorized_status("Nix Environment" ${NEXILIS_IS_NIX})
    colorized_status("Local Development" ${NEXILIS_IS_LOCAL})
    colorized_status("CI Environment" ${NEXILIS_IS_CI})
    colorized_status("Using Submodules" ${NEXILIS_USE_SUBMODULES})
endfunction()

