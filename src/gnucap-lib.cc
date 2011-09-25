/*
 * Copyright (C) 2001 Albert Davis
 * Author: Albert Davis <aldavis@ieee.org>
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Wrapper functions written by Simon Richards - 24/09/2011
 */
#include "gnucap-lib.h"
#include "md.h"

namespace gnucap_lib {
    GnucapController::GnucapController() {
        SET_RUN_MODE xx(rINTERACTIVE);
    }
    
    GnucapController::~GnucapController() {}
    
    GnucapController::test() {
        std::string cmdbuf = "< example.cir";
        CMD::cmdproc(cmdbuf);    
        std::string cmdbuf = "print op v(nodes)";
        CMD::cmdproc(cmdbuf);  
        std::string cmdbuf = "op";
        CMD::cmdproc(cmdbuf);  
    }
}