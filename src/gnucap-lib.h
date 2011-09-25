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
#ifndef GNUCAP_LIB_H
#define GNUCAP_LIB_H

namespace gnucap_lib {
    class GnucapController {
    private:
		static void insertComponent(std::string command);
		void runProbes();
        
    public:
        GnucapController();
        ~GnucapController();
        void test();

    };

	class Component {
		char _type;
		bool active;
		int leftNode;
		int rightNode;
		friend class GnucapController;
	public:
		Component(char type) : 
		  _type(type), active(false), leftNode(-1), rightNode(-1) {}
		~Component() {}
		bool isActive() {return active;}
		char getType() {return _type;}
		std::vector<Component> leftConnections;
		std::vector<Component> rightConnections;
	};
}
#endif
