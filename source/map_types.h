//=============================================================================
// Spellcross MAP related data types.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <tuple>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif

class MapXY
{
public:
	int x;
	int y;
	int IsSelected() {return(x >= 0 && y >= 0);};
	int IsEqual(int x, int y) {return(this->x == x && this->y == y);};
	MapXY() {x=-1;y=-1;};
	MapXY(int xpos, int ypos) { x=xpos;y=ypos; };
	void Clear() { x=-1;y=-1; };
	// geometric distance of tile
	double Distance(MapXY mxy) { return(Distance(mxy.x,mxy.y)); };
	double Distance(int xx, int yy)
	{		
		auto [dx,dy] = this->RelativePos(xx,yy);
		return(sqrt(dx*dx + dy*dy));
	};
	// realtive position of tile <dx,dy>
	std::tuple<double,double> RelativePos(MapXY mxy) {return RelativePos(mxy.x,mxy.y);};
	std::tuple<double,double> RelativePos(int xx,int yy)
	{
		int xx1 = xx * 2 + (((yy & 1) != 0) ? 0 : 1);
		int yy1 = yy * 1;
		int xx2 = x * 2 + (((y & 1) != 0) ? 0 : 1);
		int yy2 = y * 1;
		return std::tuple(0.707106*(double)(xx1-xx2),0.707106*(double)(yy1-yy2));
	};
	// angle of <xx,yy> from this pos
	double Angle(MapXY mxy) {return Angle(mxy.x, mxy.y);};
	double Angle(int xx, int yy)
	{
		int xx1 = xx * 2 + (((yy & 1) != 0) ? 0 : 1);
		int yy1 = yy * 1;
		int xx2 = x * 2 + (((y & 1) != 0) ? 0 : 1);
		int yy2 = y * 1;
		if(xx == x && yy == y)
			return(0);
		return atan2(yy2 - yy1, xx1 - xx2)*180.0/M_PI;
	};
	// distance in isometric coordinates (45deg of visual plane)
	std::tuple<int,int> DistanceIso(MapXY mxy)
	{
		double angle = mxy.Angle(MapXY(x,y));
		double dist = mxy.Distance(MapXY(x,y));
		int da = round(dist*cos((angle + 45.0)/180.0*M_PI));
		int db = round(dist*sin((angle + 45.0)/180.0*M_PI));
		return(std::tuple(db,da));
	}
	MapXY operator + (MapXY par)
	{
		MapXY res(x,y);
		if(res.IsSelected() && par.IsSelected())
		{
			res.x += par.x;
			res.y += par.y;
		}
		return(res);
	}
	MapXY operator - (MapXY par)
	{
		MapXY res(x,y);
		if(res.IsSelected() && par.IsSelected())
		{
			res.x -= par.x;
			res.y -= par.y;
		}
		return(res);
	}
	int operator == (MapXY par)
	{		
		return(this->x == par.x && this->y == par.y);
	}
	int operator != (MapXY par)
	{
		return(this->x != par.x || this->y != par.y);
	}
	MapXY Min(MapXY pos)
	{
		pos.x = (std::min)(this->x,pos.x);
		pos.y = (std::min)(this->y,pos.y);
		return(pos);
	}
	MapXY Max(MapXY pos)
	{
		pos.x = (std::max)(this->x,pos.x);
		pos.y = (std::max)(this->y,pos.y);
		return(pos);
	}
};

