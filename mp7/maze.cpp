#include "maze.h"


#define RIGHT 0
#define BOTTOM 1
#define LEFT 2
#define UP 3



SquareMaze::SquareMaze()
{
    mazeWidth = 0;
    mazeHeight = 0;
    size = 0;
    cells = DisjointSets();
}
void SquareMaze::makeMaze(int width, int height)
{
    // check if maze exists
    if (width < 2 || height < 2) 
        return;
 
    //cleaup first if it already represent a maze
    // the number of cells
    mazeWidth = width;
    mazeHeight = height;
    size = width * height;

    // setup randomization
    srand (time(NULL));

    // create a disjoint set to represent the cells
    // cells = NULL;
    cells.addelements(size);

    // setup all the walls to be present
    rightWalls = vector<bool> (size, true);
    bottomWalls = vector<bool> (size, true);
    
    // populate the walls vector with all the walls
    for (int j = 0; j < height; j++)
    {
		for (int i = 0; i < width; i++)
        {
            pair< int, int > point (i, j);
            pair< pair< int, int > , int> wallBottom ( point, BOTTOM);
            pair< pair< int, int > , int> wallRight ( point, RIGHT);

            walls.push_back(wallBottom);
            walls.push_back(wallRight);

        }
    }

    // instead of going through all the cells, remove all the walls randomly
    while (walls.size() != 1)
    {
        // there are more wlals to remove
        // pick a random number based on size
        int wallID = rand() % walls.size();
        pair < pair< int, int > , int > wall = walls[wallID];
        walls[wallID] = walls[walls.size()-1];
        walls.pop_back();

        pair < int, int > point = wall.first;
        int x = point.first;
        int y = point.second;        
        int dir = wall.second;

        int cellID = getCellIndex(x, y);

        if (dir)
        {
            if ( cellID + width >= size)
                continue;
            
            // 2. in same set
            if ( cells.find( cellID ) == cells.find( cellID + width ) )
                continue;

            //proceed to remove wall
            cells.setunion( cellID, cellID + width );

            //mark the wall false
            bottomWalls[cellID] = false;
        }
        else
        {

            //1. out of bounds
            if ( ( (cellID + 1)  % width) == 0 )
                continue;
            
            //2. in same set
            if ( cells.find(cellID) == cells.find( cellID + 1 ) )
                continue;

            //proceed to remove wall
            cells.setunion( cellID, cellID +1);

            //mark the wall false
            rightWalls[cellID] = false;
        }
    }
}
bool SquareMaze::canTravel(int x, int y, int dir) const
{
    if ( ( x == 0 && dir == LEFT ) ||
            ( y == 0 && dir == UP ) ||
            ( y == mazeHeight -1 && dir == BOTTOM ) ||
            ( x == mazeWidth -1 && dir == RIGHT ) ||x < 0 || y < 0 
            || x > mazeWidth || y > mazeHeight)
        return false;
        

    //check different cases
    switch (dir)
    {
        case LEFT:
            return !(rightWalls[getCellIndex(x-1, y)]);
        case BOTTOM:
            return !(bottomWalls[getCellIndex(x, y)]);
        case RIGHT:
            return !(rightWalls[getCellIndex(x, y)]);
        case UP:
            return !(bottomWalls[getCellIndex(x, y-1)]);
        default:
            return false;
    }
}
void SquareMaze::setWall (int x, int y, int dir, bool exists)
{
    int cellId = getCellIndex(x, y);

    if (dir == RIGHT)
        rightWalls[cellId] = exists;
    else
        bottomWalls[cellId] = exists;
}
vector <int> SquareMaze::solveMaze()
{
    //loop through all the bottom cells
    map <int, int> pathMap;
    bool visited [mazeWidth][mazeHeight];
    
    // initialize the array to be false
    for (int j = 0; j < mazeHeight; j++ )

    {
    	for (int i = 0; i < mazeWidth; i++ )			
        {
            visited[i][j] = false;
        }
    }

    // queue to store the neighbors
    queue <int > q;

    // enqueue the entry cell
    q.push(0);

    //mark start visited
    visited[0][0] = true;

    cout<<"Visiting all the cells..."<<endl;
    while (!q.empty())
    {
        // info of current cell
        int curCell = q.front();
        int curY = getCellY( curCell );        
        int curX = getCellX( curCell );


        // visited. pop it off queue
        q.pop();

        // loop through four directions to see if the next cell in that dir is travelable
        for (int i = 0; i < 4; i++)
        {
            //each direction
            //check if can travel that direction first

            // info of next cell
            int nextCell = getNextCell( curX, curY, i );
            int nextY = getCellY( nextCell );
            int nextX = getCellX( nextCell );


            // do something if we can go there
            if (canTravel( curX, curY, i) && !visited[ nextX ][ nextY ] )
            {
                //can travel there
                //get next cell and put it in queue
                q.push( nextCell );
                
                //mark that cell visited
                visited[ nextX ][ nextY ] = true;
                
                //map next to cur
                pathMap[nextCell] = curCell;
            } 
        }
    }
	int max_i = 0;
    int max_length = 0;

    //now we have all the cells mapped. find out the longest path
    for (int i = 0; i < mazeWidth; i++)
    {
        int end = getCellIndex(i, mazeHeight -1);
        int start = 0;
        int count = 0;


        // count number of steps
        while (end != start)
        {
            end = pathMap[end];
            count++;
        }
        // update if longer than current max
        if (count > max_length)
        {
            max_length = count;
            max_i = i;
        }
    }
    // now we have our longest path. make the exit open, and create a vector for steps to take
    exitCell = getCellIndex( max_i, mazeHeight -1);


    // go through the mapping again. this time determine the steps that should be taken. then push into stack to reverse order
    int start = 0;
    int end = exitCell;

    stack<int> steps;
    while (end != start)
    {
        int diff = end - pathMap[end];
        if (diff == -1)    //LEFT
            steps.push(2);
        else if (diff == 1)          //RIGHT
            steps.push(0);
        else if (diff == (-1) * mazeWidth)      //UP
            steps.push(3); 
        else if (diff == mazeWidth)     //DOWN
            steps.push(1);
        else                    // SOMETHING_WRONG
            steps.push(-1);

        end = pathMap[end];
    }
    //now pop everything off stack to vector to get the right order
    while (!steps.empty() )
    {
        // OXDEADBEEF   
        if (steps.top() == -1)
            cout<< "something went wrong"<<endl;
        // push to vector
        solution.push_back(steps.top());
        // POP IT
        steps.pop();
    }
    // we should now have a happy vector to return
    return solution;    
}
PNG * SquareMaze::drawMaze() const
{
    int curX;
    int curY;
    PNG * retval = new PNG( 10 * mazeWidth + 1, 10 * mazeHeight +1);
    
    for (int i = 0; i < 10 * mazeHeight +1; i++ )
    {
        (*retval)(0, i)->green = 0;
        (*retval)(0, i)->blue = 0;    
        (*retval)(0, i)->red = 0;

    }
    for ( int i = 10; i < 10 * mazeWidth +1; i++)
    {
        (*retval)(i, 0)->green = 0;
        (*retval)(i, 0)->blue = 0;    
        (*retval)(i, 0)->red = 0;

    }
    
    for (int y = 0; y < mazeHeight; y++ )
    {
		for ( int x = 0; x < mazeWidth; x++ )
        {
            if (bottomWalls[ getCellIndex(x, y) ] )
            {
                for (int i = 0; i <= 10; i++ )
                {
                    curY = ( y + 1 ) * 10;                
                    curX = x * 10 + i;
                	
                	(*retval)(curX, curY)->green = 0;
                    (*retval)(curX, curY)->blue = 0;
                    (*retval)(curX, curY)->red = 0;
    
                }
            }

            if ( rightWalls[ getCellIndex(x, y) ] )
            {
                for (int i = 0; i <= 10; i++)
                {
                    curY = y * 10 + i;                
                    curX = (x + 1) * 10;
                    
                    (*retval)(curX, curY)->green = 0;
                    (*retval)(curX, curY)->blue = 0;
                    (*retval)(curX, curY)->red = 0;

                }
            }
        }
    }

    return retval;    
}
PNG * SquareMaze::drawMazeWithSolution()
{
    if (solution.size() == 0)
    {
        return new PNG();
    }

    // get unsolved maze
    PNG * retval = drawMaze();

    // draw the solution on top of it
    int curY = 5;
    int curX = 5;


    for ( int i= 0; i < solution.size(); i++)
    {
        if ( solution[i] == RIGHT )
        {
            for (int j = 0; j <= 10; j++ )
            {
                (*retval)( curX + j, curY )->blue = 0;
                (*retval)( curX + j, curY )->green = 0;
                (*retval)( curX + j, curY )->red = 255;

            }
            curX += 10;
        }
        else if ( solution [i] == UP)
        {
            for (int j = 0; j <= 10; j++)
            {
                (*retval)( curX, curY - j)->blue = 0;
                (*retval)( curX, curY - j)->green = 0;
                (*retval)( curX, curY - j)->red = 255;

            }
            curY -= 10;
        }
        else if ( solution [i] == BOTTOM)
        {
            for (int j = 0; j <= 10; j++)
            {
                (*retval)( curX, curY + j)->blue = 0;
                (*retval)( curX, curY + j)->green = 0;            
                (*retval)( curX, curY + j)->red = 255;

            }
            curY += 10;
        }
        else if ( solution [i] == LEFT)
        {
            for (int j = 0; j <= 10; j++)
            {
                (*retval)( curX - j, curY )->blue = 0;
                (*retval)( curX - j, curY )->green = 0;
                (*retval)( curX - j, curY )->red = 255;

            }
            curX -= 10;
        }

    }
    
    int exitY = curY + 5;
    int exitX = curX - 5;

    for (int i = 1; i < 10; i++)
    {
        (*retval)( exitX + i, exitY )->green = 255;
        (*retval)( exitX + i, exitY )->blue = 255;    
        (*retval)( exitX + i, exitY )->red = 255;

    }

    return retval; 
}
int SquareMaze::getCellY ( int cellID )
{
    return cellID / mazeWidth;
}
int SquareMaze::getCellX ( int cellID )
{
	return cellID % mazeWidth;
}
int SquareMaze::getCellIndex(int x, int y) const
{
    return y * mazeWidth + x;
}


int SquareMaze::getNextCell( int x, int y, int dir )
{
    switch(dir)
    {
        case RIGHT:
            return getCellIndex(x, y) + 1;
        case LEFT:
            return getCellIndex(x, y) - 1;    
        case BOTTOM:
            return getCellIndex(x, y) + mazeWidth;
        case UP:
            return getCellIndex(x, y) - mazeWidth;
        default: return 0;      //sth wrong
    }
}
