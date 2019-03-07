/* Cutter
Copyright(C) 2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Pass all 4 axes round as a single entity.
template<class T>
struct Position {
	T x;
	T y;
	T u;
	T v;
	inline Position() : x(0), y(0), u(0), v(0) {}
	inline Position(const Position<T>& src) : x(src.x), y(src.y), u(src.u), v(src.v) {}
	inline Position(T px, T py, T pu, T pv) : x(px), y(py), u(pu), v(pv) {}
	inline void set(T px, T py, T pu, T pv) { x = px; y = py; u = pu; v = pv; }
	inline void set(const Position<T>& src) { x = src.x; y = src.y; u = src.u; v = src.v; }
	inline void zero() { x = y = u = v = 0; }
	inline void add(T dx, T dy, T du, T dv) { x += dx; y += dy; u += du; v += dv; }
	inline void add(const Position<T>& p) { x += p.x; y += p.y; u += p.u; v += p.v; }
	inline void sub(const Position<T>& p) { x -= p.x; y -= p.y; u -= p.u; v -= p.v; }
	inline void scale(double mult) { x = (T)(x * mult);  y = (T)(y * mult);  u = (T)(u * mult);  v = (T)(v * mult);	}
};

