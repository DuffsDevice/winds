/* The authors of this work have released all rights to it and placed it
in the public domain under the Creative Commons CC0 1.0 waiver
(http://creativecommons.org/publicdomain/zero/1.0/).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Median_cut_algorithm_(C_Plus_Plus)?oldid=19175
*/

#include <limits>
#include <queue>
#include <algorithm>
#include "_library/library.color.mediancut.h"

Block::Block(Point* points, int pointsLength)
{
	this->points = points;
	this->pointsLength = pointsLength;
	for(int i=0; i < NUM_DIMENSIONS; i++)
	{
		minCorner.x[i] = std::numeric_limits<unsigned char>::min();
		maxCorner.x[i] = std::numeric_limits<unsigned char>::max();
	}
}
Point * Block::getPoints()
{
	return points;
}

int Block::numPoints() const
{
	return pointsLength;
}
int Block::longestSideIndex() const
{
	int m = maxCorner.x[0] - minCorner.x[0];
	int maxIndex = 0;
	for(int i=1; i < NUM_DIMENSIONS; i++)
	{
		int diff = maxCorner.x[i] - minCorner.x[i];
		if (diff > m)
		{
			m = diff;
			maxIndex = i;
		}
	}
	return maxIndex;
}
int Block::longestSideLength() const
{
	int i = longestSideIndex();
	return maxCorner.x[i] - minCorner.x[i];
}
bool Block::operator<(const Block& rhs) const
{
	return this->longestSideLength() < rhs.longestSideLength();
}
void Block::shrink()
{
	int i,j;
	for(j=0; j<NUM_DIMENSIONS; j++)
	{
		minCorner.x[j] = maxCorner.x[j] = points[0].x[j];
	}
	for(i=1; i < pointsLength; i++)
	{
		for(j=0; j<NUM_DIMENSIONS; j++)
		{
			minCorner.x[j] = std::min(minCorner.x[j], points[i].x[j]);
			maxCorner.x[j] = std::max(maxCorner.x[j], points[i].x[j]);
		}
	}
}
std::vector<Point> medianCut(Point* image, int numPoints, unsigned int desiredSize)
{
	std::priority_queue<Block> blockQueue;
	Block initialBlock(image, numPoints);
	initialBlock.shrink();
	blockQueue.push(initialBlock);
	while (blockQueue.size() < desiredSize)
	{
		Block longestBlock = blockQueue.top();
		blockQueue.pop();
		Point * begin  = longestBlock.getPoints();
		Point * median = longestBlock.getPoints() + (longestBlock.numPoints()+1)/2;
		Point * end    = longestBlock.getPoints() + longestBlock.numPoints();
		switch(longestBlock.longestSideIndex())
		{
			case 0: std::nth_element(begin, median, end, CoordinatePointComparator<0>()); break;
			case 1: std::nth_element(begin, median, end, CoordinatePointComparator<1>()); break;
			case 2: std::nth_element(begin, median, end, CoordinatePointComparator<2>()); break;
		}

		Block block1(begin, median-begin), block2(median, end-median);
		block1.shrink();
		block2.shrink();
		blockQueue.push(block1);
		blockQueue.push(block2);
	}
	
	std::vector<Point> result{ blockQueue.size() };
	
	while(!blockQueue.empty())
	{
		Block block = blockQueue.top();
		blockQueue.pop();
		Point * points = block.getPoints();

		int sum[NUM_DIMENSIONS] = {0};
		for(int i=0; i < block.numPoints(); i++)
			for(int j=0; j < NUM_DIMENSIONS; j++)
				sum[j] += points[i].x[j];

		Point averagePoint;
		for(int j=0; j < NUM_DIMENSIONS; j++)
			averagePoint.x[j] = sum[j] / block.numPoints();

		result.push_back(averagePoint);
	}
	return result;
}

