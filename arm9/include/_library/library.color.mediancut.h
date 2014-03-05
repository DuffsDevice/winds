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

#ifndef _MEDIAN_CUT_H_
#include <vector>

#define NUM_DIMENSIONS 3

struct Point{
    unsigned char x[NUM_DIMENSIONS];
};

class Block
{
		Point minCorner, maxCorner;
		Point* points;
		int pointsLength;
	public:
		Block(Point* points, int pointsLength);
		Point * getPoints();
		int numPoints() const;
		int longestSideIndex() const;
		int longestSideLength() const;
		bool operator<(const Block& rhs) const;
		void shrink();
};

template<int index>
struct CoordinatePointComparator
{
	bool operator()(Point left, Point right) const {
		return left.x[index] < right.x[index];
	}
};

std::vector<Point> medianCut(Point* image, int numPoints, unsigned int desiredSize);

#endif /* #ifndef _MEDIAN_CUT_H_ */
