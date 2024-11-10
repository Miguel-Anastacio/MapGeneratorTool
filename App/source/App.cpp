#include "Core.h"
#include "Utilities/Timer.h"
#include "Utilities/Logger.h"
using namespace std;
int main()
{
	Core::Timer timer;
	Core::Logger::TimeMsec("Hey", timer.elapsedMilliseconds());
	return 0;
}
