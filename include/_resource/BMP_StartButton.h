#ifndef _BMP_STARTBUTTON_H_
#define _BMP_STARTBUTTON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

//
// NORMAL
//
static const _pixel BMP_StartButton_bmp[380] = {
42503, 45709, 46733, 46733, 46733, 46732, 46732, 46733, 46733, 46733, 46733, 
46733, 46733, 46733, 46701, 46701, 46733, 46733, 46733, 46733, 46733, 46733, 
46733, 45708, 45708, 45708, 45708, 44683, 44683, 44683, 44651, 44651, 43626, 
43626, 41577, 40456, 57799, 65031, 45642, 53076, 53043, 51987, 51984, 51986, 
52018, 51985, 50928, 50928, 50929, 50928, 50929, 50929, 49904, 49904, 48879, 
48879, 49904, 49904, 49904, 49904, 49904, 49903, 49903, 48847, 48879, 47822, 
46797, 47822, 46797, 46765, 45740, 44683, 43658, 38505, 47591, 63941, 45642, 
47855, 44684, 45704, 42575, 40573, 39419, 40395, 43625, 42634, 43626, 41576, 
41576, 41576, 41576, 41576, 41576, 41576, 41576, 41576, 41576, 40551, 40551, 
40519, 40519, 40519, 40519, 40519, 40519, 40519, 40519, 40519, 40519, 39462, 
39430, 37381, 37316, 56643, 44585, 44684, 42568, 41541, 33273, 34335, 35098, 
35209, 33714, 37581, 41477, 40551, 40551, 40551, 40551, 40551, 40551, 40551, 
40551, 41575, 41575, 41575, 41575, 41575, 41575, 41575, 40551, 40551, 40551, 
40551, 40519, 40519, 40519, 40518, 39430, 37349, 35236, 54628, 42504, 42634, 
40518, 39495, 48624, 51665, 42217, 34377, 33682, 38411, 37314, 41640, 41640, 
41640, 41640, 41640, 41640, 41640, 41640, 41640, 41608, 41608, 41608, 41608, 
41608, 41608, 41608, 41608, 41608, 41608, 41608, 41608, 41608, 40551, 40519, 
38406, 34212, 51556, 41478, 41609, 35398, 47655, 65166, 65100, 48454, 33462, 
37559, 39368, 38499, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 
42697, 42665, 42665, 42665, 42665, 42665, 42665, 41641, 41641, 41608, 41608, 
41608, 41608, 41608, 41608, 40519, 39430, 34212, 52612, 41446, 40551, 36486, 
48647, 56813, 58695, 38353, 33759, 36605, 41476, 38596, 42697, 42697, 42697, 
42697, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 42665, 42665, 
42665, 42665, 42665, 41640, 41640, 41640, 41640, 41640, 41608, 40519, 38406, 
36260, 55684, 40421, 39527, 40582, 38470, 36356, 38308, 38381, 36468, 39436, 
41605, 41703, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 42697, 
42697, 42665, 42665, 42665, 42665, 42665, 42665, 42665, 42665, 41640, 41608, 
41608, 41608, 41576, 40519, 36358, 40356, 60804, 39364, 37446, 38470, 39526, 
39558, 38535, 39492, 40419, 40516, 39591, 40615, 41609, 41609, 41609, 41609, 
41609, 41609, 41609, 41609, 41609, 41609, 41609, 41609, 41608, 41608, 41576, 
41576, 41576, 41576, 41576, 41576, 41576, 41576, 40519, 39463, 34245, 47524, 
64933, 39332, 38340, 38372, 38372, 38372, 38372, 38405, 39461, 39429, 39397, 
40453, 39429, 40454, 40454, 40454, 40454, 40454, 40454, 40454, 40454, 40454, 
40455, 40455, 40455, 40455, 40455, 40455, 41479, 41479, 40455, 41479, 41479, 
40422, 40422, 38309, 37284, 56740, 59716
};

class BMP_StartButton : public _bitmap {
	public:
		BMP_StartButton() : _bitmap(const_cast<_pixelArray>(BMP_StartButton_bmp), 38, 10) { }
};

//
// PRESSED
//

static const _pixel BMP_StartButtonPressed_bmp[380] = {
35074, 36131, 36099, 36035, 36067, 36033, 36066, 36068, 37091, 37091, 37092, 
37092, 37092, 37092, 37092, 37124, 37124, 37124, 37124, 37124, 37124, 37124, 
37124, 37124, 37124, 36099, 36099, 36099, 36099, 36099, 36099, 36099, 36099, 
36099, 34050, 37122, 53605, 65031, 36163, 40358, 39334, 39334, 39333, 40391, 
40423, 39397, 39365, 39366, 40455, 40455, 40455, 40455, 40455, 39429, 40455, 
40455, 40455, 40455, 40487, 40487, 39430, 38405, 39462, 39462, 39462, 39462, 
39462, 39462, 39462, 39461, 39461, 38437, 37413, 34275, 51620, 63941, 36163, 
38309, 37316, 37313, 36331, 40605, 39418, 36261, 37411, 37413, 37380, 36387, 
36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 
36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 
36387, 33282, 41346, 56643, 36131, 37284, 36290, 35264, 33241, 36415, 36122, 
35273, 34771, 36491, 35232, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 
36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 
36387, 36387, 36355, 36355, 36355, 35330, 33250, 37282, 54628, 36163, 36259, 
34241, 35267, 48624, 51666, 41193, 34410, 35731, 37320, 33120, 37443, 37443, 
37443, 37443, 37443, 37443, 37443, 37443, 37443, 37443, 36387, 36387, 36387, 
36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36387, 36355, 
33250, 35233, 51556, 35138, 36291, 33217, 45508, 65199, 65100, 46407, 33495, 
37526, 37252, 33248, 37444, 37444, 37444, 37444, 37444, 37444, 37444, 37444, 
37444, 37444, 36420, 36420, 36420, 36420, 36420, 36420, 36420, 36420, 36420, 
36419, 36419, 36419, 36387, 36387, 33282, 36258, 52612, 35170, 35298, 33280, 
46533, 56813, 57671, 38387, 33791, 36570, 36256, 33280, 37476, 37476, 37476, 
37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37444, 
37444, 37444, 37444, 37444, 37444, 36420, 36420, 36420, 36419, 36387, 33250, 
39330, 55684, 35170, 35298, 34305, 34274, 33152, 35169, 37357, 36435, 37321, 
36353, 34369, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 
37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37476, 37444, 
36420, 36420, 36419, 36387, 33250, 44419, 60804, 34145, 36387, 36387, 36419, 
36451, 35427, 37409, 37312, 37409, 36484, 37476, 36451, 35394, 37507, 37508, 
36451, 37508, 35426, 37508, 37508, 37508, 38565, 37476, 36451, 38533, 38533, 
38501, 38501, 37476, 37476, 37476, 37476, 37476, 36419, 36387, 33218, 52644, 
64933, 34145, 36291, 36291, 36291, 36291, 36323, 36323, 36355, 36323, 36323, 
37316, 37348, 37347, 37348, 37348, 37348, 37348, 37348, 37348, 37348, 37348, 
37348, 37348, 37348, 37348, 37348, 37348, 37348, 37348, 37348, 37348, 37316, 
37316, 36291, 34178, 40323, 60805, 59716
};

class BMP_StartButtonPressed : public _bitmap {
	public:
		BMP_StartButtonPressed() : _bitmap(const_cast<_pixelArray>(BMP_StartButtonPressed_bmp), 38, 10) { }
};
#endif