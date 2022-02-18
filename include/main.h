/*
    NDSForwarder for DSi
    Copyright (C) 2018-2020 JeffRuLz
    Copyright (C) 2022-present lifehackerhansol

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAIN_H
#define MAIN_H

#include <nds.h>
#include <fat.h>
#include <stdio.h>

void installMenu();
void testMenu();

extern PrintConsole topScreen;
extern PrintConsole bottomScreen;

void clearScreen(PrintConsole* screen);

#define abs(X) ( (X) < 0 ? -(X): (X) )
#define sign(X) ( ((X) > 0) - ((X) < 0) )
#define repeat(X) for (int _I_ = 0; _I_ < (X); _I_++)

#endif