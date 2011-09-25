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
#pragma once

namespace gnucap_lib {
	class Component;
	class GnucapController;

	class GnucapController {
	private:
		std::string dtos(double d);
		std::string makeName(char type, int id);
		vector<Component*> components;
		static void insertComponent(std::string command);
		void runProbes();

	public:
		GnucapController();
		~GnucapController();
		void test();
		Component* newResistor(double r);
		Component* newCapacitor(double c);
		Component* newInductor(double l);
		Component* newDCSupply(double v);
		Component* newACSupply(double v, double f); // bias = phase = 0;
		Component* newACSupply(double v, double b, double f, double p);

	};
	class Component {
		string _name;
		bool active;
		int *nodes;
		string value;
		Component(string name);
		~Component() {}
		friend class GnucapController;

	public:

		int leads;
		double voltage;
		std::vector<Component*>* connections;
		void connectTo(Component *other, int lead) {connections[lead].push_back(other);}
		bool isActive() {return active;}
	};
}
