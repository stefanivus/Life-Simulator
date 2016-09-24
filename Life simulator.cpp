/** 
Life Simulation
Copyright (C) 2016 Stefan Ivanovic 

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
**/


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <conio.h>

using namespace std;

void print_map(string map[70][70])//Prints map on the screen
{
    int i,j;
	for(i=0;i<70;i++)
	{
	    if(i<9)
        {
            cout << i+1 << "   ";
        }
        else
        {
            cout << i+1 << "  ";
        }

		for(j=0;j<70;j++)
      	{
		   cout << map[j][i];
	    }
	    cout << "\n";
	}
}//End of func
void map_define(string map[70][70])//Fills map with empty spaces
{
	int i,j;
    for(i=0;i<70;i++)
	{
		for(j=0;j<70;j++)
      	{
		   map[i][j] = "-";
	    }
	}
}//End of func

class food//Food class
{
	public:
		int x,y;
		food();
		void food_define(string map[70][70]);
		void food_eaten(string map[70][70]);
};

        food::food(void)
        {
        	x = rand() % 70;
	    	y = rand() % 70;
        }
		void food::food_define(string map[70][70])//Defines food's position
		{
	    	while(map[x][y] != "-")
	    	{
	    		x = rand() % 70;
	    	    y = rand() % 70;
	    	}
	    	map[x][y] = "X";
		}
		void food::food_eaten(string map[70][70])//If food is eaten , dissapear and appear somewhere else
		{
			if ((y != 0 && map[x][y-1] == "M") || (y != 69 && map[x][y+1] == "M") || (x != 0 && map[x-1][y] == "M") || (x != 69 && map[x+1][y] == "M") ||
			    (y != 0 && map[x][y-1] == "F") || (y != 69 && map[x][y+1] == "F") || (x != 0 && map[x-1][y] == "F") || (x != 69 && map[x+1][y] == "F") ||
				(y != 0 && map[x][y-1] == "m") || (y != 69 && map[x][y+1] == "m") || (x != 0 && map[x-1][y] == "m") || (x != 69 && map[x+1][y] == "m") ||
			    (y != 0 && map[x][y-1] == "f") || (y != 69 && map[x][y+1] == "f") || (x != 0 && map[x-1][y] == "f") || (x != 69 && map[x+1][y] == "f"))
			{
				map[x][y] = "-";
				x = rand() % 70;
	    	    y = rand() % 70;
				this->food_define(map);
			}
			else
			{
				return;
			}
		}


class organism //Organism class
{
	public://Organism variables
		organism();
		void org_print(string map[70][70]);
		void hunger_inc(string map[70][70],vector<organism> org);
		void eat(string map[70][70]);
		void org_age(string map[70][70],vector<organism> org);
		int direct_to_obj(string map[70][70],string obj);
		int unit_collision(string map[70][70],string obj);
		void org_move(string map[70][70]);
		void org_reproduce(string map[70][70], vector<organism>& childs);

	private:
	    string sex;
		int age;
		int x,y;
		int hunger;
		int rep_cd;
};
		organism::organism()//Constructor
		{
			int random = rand()%2;
			if (random == 1){ sex = "m"; }
			else { sex = "f"; }
			age = 0;
			hunger = 0;
			rep_cd = 3;
			x = rand() % 70;
	    	y = rand() % 70;
		}//End of func


		void organism::org_print(string map[70][70])//Declares organisms position,sex
		{
			while(map[this->x][this->y] != "-")
	    	{
	    		this->x = rand() % 70;
	    	    this->y = rand() % 70;
	    	}
	    	map[this->x][this->y] = sex;
		}//End of func


		void organism::hunger_inc(string map[70][70],vector<organism> org)
		{
		    int i = 0;
			hunger++;
			if(hunger >= 50)
			{
				map[x][y] = "-";
				sex = "-";
				for (organism& f : org)
                {
                    if (f.x == x && f.y == y)
                    {
    		           org.erase(org.begin()+i);
                    }
                    i++;
                }
			}
		}


		void organism::eat(string map[70][70])//If food is next to organism, eat
		{
			if ((y != 0 && map[x][y-1] == "X") || (y != 69 && map[x][y+1] == "X") || (x != 0 && map[x-1][y] == "X") || (x != 69 && map[x+1][y] == "X"))
			{
				hunger = 0;
			}
			else
			{
				return;
			}

		}


		void organism::org_age(string map[70][70],vector<organism> org)//Ages the organism
		{
		    int i = 0;
			age++;
			if (age >= 20)
			{
				if(sex == "m")
				{
					sex = "M";
					map[x][y] = "M";
				}
				else if (sex == "f")
				{
					sex = "F";
					map[x][y] = "F";
				}
			}
			if (age >= 100)
			{
				map[x][y] = "-";
				sex = "-";
				for (organism& f : org)
                {
                    if (f.x == x && f.y == y)
                    {
    		           org.erase(org.begin()+i);
                    }
                    i++;
                }
			}
		}//End of func


		int organism::direct_to_obj(string map[70][70],string obj)//Recommends movement direction based on the closest object
		{
			int i,j;
			int t_closest = 0;
			int k = 0;
			vector<int> v;
			int obj_x, obj_y;
			int closest = 500;
			for(i=0;i<70;i++)//Scans map for objects and puts them into an array
			{
				for(j=0;j<70;j++)
				{
					if(map[j][i] == obj)
					{
						v.push_back(j);
						v.push_back(i);
					}
				}
			}
			unsigned int arr[v.size()];
			for(unsigned int i : v)
            {
                arr[k] = i;
                k++;
            }
			for(k=0;k<v.size();k += 2)//Loops through the array and finds the closest object
			{
				if (x > arr[k] && y > arr[k+1])
				{
					t_closest = (x-arr[k])+(y-arr[k+1]);
					if (t_closest < closest)
					{
						closest = t_closest;
						obj_x = arr[k];
						obj_y = arr[k+1];
					}
				}
				else if (x <= arr[k] && y <= arr[k+1])
				{
					t_closest = (arr[k]-x)+(arr[k+1]-y);
					if (t_closest < closest)
					{
						closest = t_closest;
						obj_x = arr[k];
						obj_y = arr[k+1];
					}
				}
				else if (x > arr[k] && y <= arr[k+1])
				{
					t_closest = (x-arr[k])+(arr[k+1]-y);
					if (t_closest < closest)
					{
						closest = t_closest;
						obj_x = arr[k];
						obj_y = arr[k+1];
					}
				}
				else if (x <= arr[k] && y > arr[k+1])
				{
					t_closest = (arr[k]-x)+(y-arr[k+1]);
					if (t_closest < closest)
					{
						closest = t_closest;
						obj_x = arr[k];
						obj_y = arr[k+1];
					}
				}
			}
			//Following IF statements direct the organism towards the object
			int random = rand()%2;
			if (x > obj_x && y > obj_y)
            {
                if (random == 1){ return 1; }
                else{ return 3; }
            }
            else if (x < obj_x && y < obj_y)
            {
                if (random == 1){ return 2; }
               	else{ return 4; }
            }
            else if (x > obj_x && y < obj_y)
            {
               	if (random == 1){ return 2; }
               	else{ return 3; }
            }
            else if (x < obj_x && y > obj_y)
            {
               	if (random == 1){ return 1; }
               	else{ return 4; }
            }
            else if (x < obj_x && y == obj_y){ return 4; }
            else if (x > obj_x && y == obj_y){ return 3; }
            else if (x == obj_x && y > obj_y){ return 1; }
            else if (x == obj_x && y < obj_y){ return 2; }
			else{ return 0; }
		}//End of func


		int organism::unit_collision(string map[70][70],string obj)//Prevents organism from running into other organisms or objects
		{
		    int count = 0;
		    int dir = this->direct_to_obj(map,obj);
			while((dir == 1 && y == 0) || (dir == 2 && y == 69) || (dir == 3 && x == 0) || (dir == 4 && x == 69))//If recommended direction is off the map, get new direction (this shouldn't happen but just in case)
			{
			  dir = rand() % 4 + 1;
			}
			while((dir == 1 && map[x][y-1] != "-") || (dir == 2 && map[x][y+1] != "-") || (dir == 3 && map[x-1][y] != "-") || (dir == 4 && map[x+1][y] != "-"))//If recommended direction is occupied, get new direction
			{
			  if ((y == 0 || map[x][y-1] != "-") && (y == 69 || map[x][y+1] != "-") && (x == 0 || map[x-1][y] != "-") && (x == 69 || map[x+1][y] != "-"))//If all directions are occupied ditch loop
			  {
			  	return 0;
			  }
			  if (count < 20)
              {
                  dir = this->direct_to_obj(map,obj);
                  count++;
              }
              else
              {
                  dir = rand() % 4 + 1;
              }
			  while((dir == 1 && y == 0) || (dir == 2 && y == 69) || (dir == 3 && x == 0) || (dir == 4 && x == 69))//Another off the map check to avoid bugs
			  {
			    dir = rand() % 4 + 1;
			  }
			}
			return dir;
		}//End of func


	    void organism::org_move(string map[70][70])//Moves organism by 1 space(1 = up, 2 = down, 3 = left, 4 = right)
	    {
	    	int dir;
	    	//If hungry or underage, search for food, otherwise search for a mate
	    	if (hunger >= 25 || age < 20)
			{
				dir = this->unit_collision(map,"X");
	    	}
	    	else
			{
				if (sex == "M")
				{
					dir = this->unit_collision(map,"F");
				}
				else if (sex == "F")
				{
					dir = this->unit_collision(map,"M");
				}
	    	}
	    	//Following IF statements move organism in the desired direction
	    	if(dir == 1)
	    	{
	    		map[x][y] = "-";
	    		y -= 1;
	    		map[x][y] = sex;
	    	}
	    	else if(dir == 2)
	    	{
	    		map[x][y] = "-";
	    		y += 1;
	    		map[x][y] = sex;
	    	}
	    	else if(dir == 3)
	    	{
	    		map[x][y] = "-";
	    		x -= 1;
	    		map[x][y] = sex;
	    	}
	    	else if(dir == 4)
	    	{
	    		map[x][y] = "-";
	    		x += 1;
	    		map[x][y] = sex;
	    	}
	    	else
	    	{
	    		return;
	    	}
	    }//End of func


	    void organism::org_reproduce(string map[70][70], vector<organism>& childs)//If organism of opposite sex exists in a movable to space, make another organism
        {
            if(sex == "F" && ((y != 0 && map[x][y-1] == "M") || (y != 69 && map[x][y+1] == "M") || (x != 0 && map[x-1][y] == "M") || (x != 69 && map[x+1][y] == "M")))
            {
              if(rep_cd % 3 == 0)
              {
                organism child;
                int random = rand() % 2 + 1;
                if (random == 1){ child.sex = "m"; }
                else{ child.sex = "f"; }
                child.age = 0;
                child.hunger = 0;
                if(y != 0 && map[x][y-1] == "-")
                {
                	child.x = this->x;
                    child.y = this->y-1;
                    map[x][y-1] = child.sex;
                }
                else if(y != 69 && map[x][y+1] == "-")
                {
                	child.x = this->x;
                    child.y = this->y+1;
                    map[x][y+1] = child.sex;
                }
                else if(x != 0 && map[x-1][y] == "-")
                {
                	child.x = this->x-1;
                    child.y = this->y;
                    map[x-1][y] = child.sex;
                }
                else if(x != 69 && map[x+1][y] == "-")
                {
                	child.x = this->x+1;
                    child.y = this->y;
                    map[x+1][y] = child.sex;
                }
                else
                {
                    return;
                }
                childs.push_back(child);
               }
               rep_cd++;
            }
            else
            {
                rep_cd++;
                return;
            }
        }//End of func



int main()
{   //Defining variables
	string map[70][70];
	int turn = 0;
	srand(time(NULL));

	vector<organism> allOrganisms;
	map_define(map);

	//Defines organisms
    organism org;
    org.org_print(map);
    allOrganisms.push_back(org);
    organism org1;
    org1.org_print(map);
    allOrganisms.push_back(org1);
    organism org2;
    org2.org_print(map);
    allOrganisms.push_back(org2);
    organism org3;
    org3.org_print(map);
    allOrganisms.push_back(org3);
    organism org4;
    org4.org_print(map);
    allOrganisms.push_back(org4);
    organism org5;
    org5.org_print(map);
    allOrganisms.push_back(org5);
    organism org6;
    org6.org_print(map);
    allOrganisms.push_back(org6);
    organism org7;
    org7.org_print(map);
    allOrganisms.push_back(org7);
    organism org8;
    org8.org_print(map);
    allOrganisms.push_back(org8);
    organism org9;
    org9.org_print(map);
    allOrganisms.push_back(org9);



    food food1;
    food food2;
    food food3;
    food food4;
    food food5;
    food food6;
    food food7;
    food food8;
    food food9;
    food food10;
    food1.food_define(map);
    food2.food_define(map);
    food3.food_define(map);
    food4.food_define(map);
    food5.food_define(map);
    food6.food_define(map);
    food7.food_define(map);
    food8.food_define(map);
    food9.food_define(map);
    food10.food_define(map);
    cout<< "                          Welcome to Life Simulator v1.0\n\n";
    cout<< "SYMBOLS:\nM = Male adult\nF = Female adult\nm = Male child\nf = Female child\nX = food\n\n";
    cout<< "COMMANDS:\nPress enter to increase time\n\n";
    cout<< "Press enter to start...";
    getch();
    // loop that gives organisms life
    while(true)
    {
    	cout<<"\n\n";


        vector<organism> childs;
        for(organism& org : allOrganisms)
        {
            org.org_move(map);
            org.org_age(map, allOrganisms);

            org.hunger_inc(map, allOrganisms);
		    org.eat(map);
		    org.org_reproduce(map, childs);



		    food1.food_eaten(map);
		    food2.food_eaten(map);
		    food3.food_eaten(map);
		    food4.food_eaten(map);
		    food5.food_eaten(map);
		    food6.food_eaten(map);
		    food7.food_eaten(map);
		    food8.food_eaten(map);
		    food9.food_eaten(map);
		    food10.food_eaten(map);
        }

        // add all childs to the herd
        allOrganisms.insert(allOrganisms.end(), childs.begin(), childs.end());
        print_map(map);
        cout<< "Year:" << turn << endl;
        getch();
        turn++;
    }


	return 0;
}


