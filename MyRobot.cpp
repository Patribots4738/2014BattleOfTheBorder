#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */
class RobotDemo: public IterativeRobot {
	RobotDrive myRobot; // robot drive system

	//Declare joysticks (controllers)
	Joystick stick1, stick2, xboxController;

	//declare xbox "a" button
	JoystickButton aButton;

	//declare xbox "x" button
	JoystickButton bButton;

	JoystickButton xButton;

	JoystickButton yButton;

	JoystickButton l1Button;

	JoystickButton r1Button;
	//boolean that checks if wench should wind or not
	DigitalInput isWenchCheckPressed;

	//Declares Compressor
	Compressor compressor;

	//Declares the double solenoid for the arms
	DoubleSolenoid arm;

	Solenoid launch;

	DriverStationLCD *dsLCD;
	bool armState, r1_Old, x_Old;

	float turnforseconds;
	bool canturn;
public:
	RobotDemo() :
				myRobot(3, 4), // these must be initialized in the same order
				stick1(1), // as they are declared above.
				stick2(2), xboxController(3), aButton(&xboxController, 1),
				bButton(&xboxController, 2), xButton(&xboxController, 3),
				yButton(&xboxController, 4), l1Button(&xboxController, 5),
				r1Button(&xboxController, 6), isWenchCheckPressed(2),
				compressor(1, 1), arm(1, 2), launch(3), dsLCD()

	{

		myRobot.SetExpiration(0);
		this->SetPeriod(0.1); //Set update period to sync with robot control packets (20ms nominal)

		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2,"Wakarimasen!!");
		compressor.Start();
		armState = false;
		turnforseconds = 1;
		canturn=	false;
	}

	/**
	 * Robot-wide initialization code should go here.
	 * 
	 * Use this method for default Robot-wide initialization which will
	 * be called when the robot is first powered on.  It will be called exactly 1 time.
	 */
	void RobotDemo::RobotInit() {
	}

	/**
	 * Initialization code for disabled mode should go here.
	 * 
	 * Use this method for initialization code which will be called each time
	 * the robot enters disabled mode. 
	 */
	void RobotDemo::DisabledInit() {
	}

	/**
	 * Periodic code for disabled mode should go here.
	 * 
	 * Use this method for code which will be called periodically at a regular
	 * rate while the robot is in disabled mode.
	 */
	void RobotDemo::DisabledPeriodic() {
	}

	/**
	 * Initialization code for autonomous mode should go here.
	 * 
	 * Use this method for initialization code which will be called each time
	 * the robot enters autonomous mode.
	 */
	void RobotDemo::AutonomousInit() {
	}

	/**
	 * Periodic code for autonomous mode should go here.
	 *
	 * Use this method for code which will be called periodically at a regular
	 * rate while the robot is in autonomous mode.
	 */
	void RobotDemo::AutonomousPeriodic() {
	}

	/**
	 * Initialization code for teleop mode should go here.
	 * 
	 * Use this method for initialization code which will be called each time
	 * the robot enters teleop mode.
	 */
	void RobotDemo::TeleopInit() {
	}

	/**
	 * Periodic code for teleop mode should go here.
	 *
	 * Use this method for code which will be called periodically at a regular
	 * rate while the robot is in teleop mode.
	 */
	void RobotDemo::TeleopPeriodic() {
		//declares Victor used to control wench
		Victor *wench = new Victor(1);

		//declares Victor used to control wheels
		Victor *pickupWheel = new Victor(2);

		while (true) {
			//Set sticks for DriveRobot
			myRobot.TankDrive(stick1, stick2);

			//First check if the wench can be used 
			if (isWenchCheckPressed.Get()) {
				//switch wench on. canturn controls the countdown
				if (aButton.Get()&&!canturn) {
					compressor.Stop();
					wench->Set(1);
					canturn = true;
				} 
			}
			//if the limit switch is pressed, stop the wench and start the compressor
			else{
				compressor.Start();
				wench->Set(0);
				canturn = false;
			}
			//if time runs out before the switch is hit, stop the wench and start the compressor
			if (canturn){
				Wait(turnforseconds);
				compressor.Start();
				wench->Set(0);
				canturn = false;
			}

			// launch the ball
			if (!armState) {
				if (r1Button.Get() && !r1_Old) {
					launch.Set(true);
					Wait(1);
					launch.Set(false);
					r1_Old =true;
				}
				else if(!r1Button.Get() ){
					r1_Old=false;
				}
			}
			
			//switch arm forward on button press and off on button up
			if (xButton.Get() && !x_Old) {
				x_Old = true;
				armState = !armState;
				
				if (armState) {
					arm.Set(DoubleSolenoid::kForward);
					dsLCD->PrintfLine(DriverStationLCD::kUser_Line2,"%s","fwd");
					dsLCD->UpdateLCD();
					dsLCD->Clear();
				} else {
					arm.Set(DoubleSolenoid::kReverse);
					dsLCD->PrintfLine(DriverStationLCD::kUser_Line1,"%s","rvrse");
					dsLCD->UpdateLCD();
					dsLCD->Clear();
				}
				
				Wait(0.5);
				arm.Set(DoubleSolenoid::kOff);
				/*dsLCD->PrintfLine(DriverStationLCD::kUser_Line1,"%d, %B",arm.Get(),armState);
				dsLCD->UpdateLCD();*/
			} else if(!xButton.Get()){
				x_Old = false;
			//	dsLCD->Clear();
			//dsLCD->UpdateLCD();
			}

			/* if y is pressed start the pickup wheel forward. if the b is pressed, start it backward. 
			 else dont run
			 */
			if (yButton.Get()) {
				pickupWheel->Set(1);
			} else if (bButton.Get()) {
				pickupWheel->Set(-1);
			} else {
				pickupWheel->Set(0);
			}

		}
	}
	
	void RobotDemo::IsOperatorControl() {

	}
	/**
	 * Initialization code for test mode should go here.
	 * 
	 * Use this method for initialization code which will be called each time
	 * the robot enters test mode.
	 */
	void RobotDemo::TestInit() {
	}

	/**
	 * Periodic code for test mode should go here.
	 *
	 * Use this method for code which will be called periodically at a regular
	 * rate while the robot is in test mode.
	 */
	void RobotDemo::TestPeriodic() {
	}

};

START_ROBOT_CLASS(RobotDemo)
;

