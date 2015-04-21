#include <MotorMovement.h>
#include <NewPing.h>
#include <LaserSupport.h>

//DEFINES/CONSTANTS
#define SonarTriggerPin  14
#define SonarEchoPin     15
#define SonarMaxDistance 255

#define LaserTriggerPin 2
#define LaserMonitorPin 3
#define LaserPELine     4

//OBJECTS
AsymmetricMotorPair engines(10, 11, 12, 13);
NewPing sonarObject(SonarTriggerPin, SonarEchoPin, SonarMaxDistance);
I2CLaser laserObject(50);

//GLOBAL VARIABLES
command currentCommand = STOP;
unsigned char currentSonarDistance;
unsigned long currentLaserDistance;

void setup()
{
	//legit there is nothing to do
}

void loop()
{
	currentCommand = calculateNextCommand();

	switch (currentCommand)
	{
	case DRIVEFORWARDS:
		engines.driveForward(20, 20);
		break;
	case DRIVEBACKWARDS:
		engines.driveBackward(20, 20);
		break;
	case TURNLEFT:
		engines.turnLeft(20);
		break;
	case ROTATELEFT:
		engines.turnLeftOnPoint(20, 20);
		break;
	case TURNRIGHT:
		engines.turnRight(20);
		break;
	case ROTATERIGHT:
		engines.turnRightOnPoint(20, 20);
		break;
	case STOP:
		engines.stop();
		break;
	default:
		engines.stop();
		break;
	}
}

command calculateNextCommand()
{
	bool criticalSonarDistance;
	bool criticalLaserDistance;

	currentSonarDistance = sonarObject.convert_cm(sonarObject.ping_median());
	currentLaserDistance = laserObject.getLaserDistance();

	criticalSonarDistance = (currentSonarDistance <= 10) ? true : false;
	criticalLaserDistance = (currentLaserDistance <=  4) ? true : false;

	if (criticalLaserDistance && criticalSonarDistance)
	{
		currentCommand = ROTATERIGHT;
	}

	if (criticalLaserDistance && !criticalSonarDistance)
	{
		currentCommand = TURNRIGHT;
	}

	if (!criticalLaserDistance && !criticalSonarDistance)
	{
		currentCommand = DRIVEFORWARDS;
	}

	if (!criticalLaserDistance && criticalSonarDistance)
	{
		currentCommand = ROTATELEFT;
	}
}