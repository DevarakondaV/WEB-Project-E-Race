#include <Stepper.h>
#include <Servo.h>
#include <Wire.h>
#include <String.h>

//########################### structure Declerations  ###########################
//###############################################################################
struct cord
{
  cord(){}
  cord(double m_x, double m_y)
  {
    x = m_x;
    y = m_y;
  }

  double x;
  double y;
};
//###############################################################################
//###############################################################################
struct section
{
  section(){}

  section(cord m_one, cord m_two, cord m_three, cord m_four)
  {
    one = m_one;
    two = m_two;
    three = m_three;
    four = m_four;
  }

  cord one;
  cord two;
  cord three;
  cord four;
};
//###############################################################################
//########################### class Declerations/Definition #####################
//###############################################################################
class Erace
{
public:

  Erace(Stepper * M_Horizontal_SMotor, Stepper * M_Vertical_SMotor, Servo * M_Servo);

  //STEPPER MOTOR METHODS
  void SetPosition(cord new_location); //Byron
  void Start_Erace(String my_section);//jai qi
  void Set_Motor_Speed(int New_Speed);
  
  //SERVO METHODS
  void serov_setPosition(int m_angle);

  //HELPER METHODS
  void write_diag(cord P1,cord P2,double RUN,double RISE);
  void write_Letter(char n,int TopPos);
  void setCurrent(cord m_curPos);

  cord GetPosition();
  int GetMode();
  cord get_steps_to(cord m_location);


private:

  Stepper * Horizontal_SMotor;
  Stepper * Vertical_SMotor;
  Servo * Servo_motor;

  int StepsX = 0;
  int StepsY = 0;

  cord currentLocation = cord(0, 0);
  double inches_per_step = 0.00283; //inches per step

  double Ewidth = 5;
  double Eheight = 1;
  //#########################################################################################################################################################
  //BOARD DEFINITIONS     ALL COORDINATES ARE IN INCHES
  //#########################################################################################################################################################
  double xp0 = 0;       double yp0 = 0;
  double xp1 = 11;   double yp1 = 11;
  double xp2 = 22;   double yp2 = 22;
  double xp3 = 33;      double yp3 = 30;

  //#########################################################################################################################################################
  //sections
  //#########################################################################################################################################################
  section Section1 = section(cord(xp0, yp0), cord(xp1, yp0), cord(xp1, yp1), cord(xp0, yp1));
  section Section2 = section(cord(xp1, yp0), cord(xp2, yp0), cord(xp2, yp1), cord(xp1, yp1));
  section Section3 = section(cord(xp2, yp0), cord(xp3, yp0), cord(xp3, yp1), cord(xp2, yp1));
  section Section4 = section(cord(xp0, yp1), cord(xp1, yp1), cord(xp1, yp2), cord(xp0, yp2));
  section Section5 = section(cord(xp1, yp1), cord(xp2, yp1), cord(xp2, yp2), cord(xp1, yp2));
  section Section6 = section(cord(xp2, yp1), cord(xp3, yp1), cord(xp3, yp2), cord(xp2, yp2));
  section Section7 = section(cord(xp0, yp2), cord(xp1, yp2), cord(xp1, yp3), cord(xp0, yp3));
  section Section8 = section(cord(xp1, yp2), cord(xp2, yp2), cord(xp2, yp3), cord(xp1, yp3));
  section Section9 = section(cord(xp2, yp2), cord(xp3, yp2), cord(xp3, yp3), cord(xp2, yp3));
};
//########################### Function Declerations #####################
//#######################################################################
String Convert(String Cord);
int Find_Command(String Command);
void command_call(int command_input);
String Find_section(String command);
int Find_mode(String command);
int* Find_position(String command,int m_pos_arry[]);
void writeINPUT(String IS,int SL);

//########################### Class Method Definitions #####################
//##########################################################################
void Erace::Set_Motor_Speed(int New_Speed)
{
  Horizontal_SMotor->setSpeed(New_Speed);
  Vertical_SMotor->setSpeed(New_Speed);
}

cord Erace::GetPosition()
{
  return currentLocation;
}

void Erace::setCurrent(cord m_curPos)
{
  currentLocation = m_curPos;
}

cord Erace::get_steps_to(cord m_location)   //returns steps required to move form one location location to next
{
  cord currentLocation = GetPosition(); //Cordinate Contains the Current Location of the Device
  double cur_x = currentLocation.x;   //current x lcoation
  double cur_y = currentLocation.y;   //current y lcoation

  //Steps to the location below
  double new_x = m_location.x;    //moving to x location
  double new_y = m_location.y;  //moving to y location


  double distx = sqrt(pow(cur_x - new_x, 2));  //inches to location
  double disty = sqrt(pow(cur_y - new_y, 2));  //inches to new location

  int stepx = distx / inches_per_step;  //steps to location x
  int stepy = disty / inches_per_step;  //steps to location y

  //modifying steps to be negative depending on location
  stepx = (new_x<cur_x) ? ~stepx+1:stepx;
  stepy = (new_y<cur_y) ? ~stepy+1:stepy;

  currentLocation = m_location;
  StepsX = stepx;
  StepsY = stepy;
  cord steps = cord(stepx, stepy);
  currentLocation = m_location;
  return steps;
}

void Erace::Start_Erace(String my_sections)
{
  int stringLength = my_sections.length();
  for (int i = 0; i < stringLength; i++)
  {
      int curS = my_sections.substring(i,i+1).toInt();
      section cur_section;
      switch (curS)
      {
      case 1:
        cur_section = Section1;
        break;
      case 2:
        cur_section = Section2;
        break;
      case 3:
        cur_section = Section3;
        break;
      case 4:
        cur_section = Section4;
        break;
      case 5:
        cur_section = Section5;
        break;
      case 6:
        cur_section = Section6;
        break;
      case 7:
        cur_section = Section7;
        break;
      case 8:
        cur_section = Section8;
        break;
      case 9:
        cur_section = Section9;
        break;
      default:
        cur_section = Section1;
        break;
      }
    
      
      cord P1 = cur_section.one;
      cord P2 = cur_section.two;
      cord P3 = cur_section.three;
      cord P4 = cur_section.four;

      double MoveHeight = (P3.y-P1.y)/(Eheight+.5);
      SetPosition(P1);
      serov_setPosition(160);
      SetPosition(cord(P2.x-3,P2.y));
      String cur = "Right";

      for (int i=0;i <= MoveHeight-1;i++)
      {
        double newY = currentLocation.y+Eheight+.5;
        cord MovePos = cord(currentLocation.x,newY);
        SetPosition(MovePos);
        if (!cur.compareTo("Right"))
        {
          SetPosition(cord(P1.x,newY));
          cur = "Left";
        }
        else 
        {
          SetPosition(cord(P1.x+8,newY));
          cur = "Right";
        }
    }
    serov_setPosition(90);
  }
}

void Erace::SetPosition(cord new_location)
{
  cord steps = get_steps_to(new_location);
  //Horizontal_SMotor->setSpeed(540);//380//580
  Horizontal_SMotor->step(steps.x);

  //Vertical_SMotor->setSpeed(540);//380//580
  Vertical_SMotor->step(steps.y);
  setCurrent(new_location);
}

Erace::Erace(Stepper *  M_Horizontal_SMotor, Stepper * M_Vertical_SMotor, Servo * M_Servo)
{
  Horizontal_SMotor = M_Horizontal_SMotor;
  Vertical_SMotor = M_Vertical_SMotor;
  Horizontal_SMotor->setSpeed(540);
  Vertical_SMotor->setSpeed(540);
  Servo_motor = M_Servo;
}

void Erace::serov_setPosition(int m_angle)
{
  Servo_motor->write(m_angle);
}

void Erace::write_diag(cord P1,cord P2,double RUN,double RISE)
{
  //From P1 to P2
  cord line_slope;
  setCurrent(P1);
  cord steps = get_steps_to(P2);

  
  double distx = P2.x-P1.y;
  double disty = P2.y-P1.y;
  int limit = (steps.x < steps.y) ? (steps.x/RUN) : (steps.y/RISE);
  
  delay(500);

  for(int i = 0;i < abs(limit);i++)
  {
     Horizontal_SMotor->step(RUN);
     delay(1);
     Vertical_SMotor->step(RISE);
  }
  setCurrent(P2);
}
//#############################################################################################################################################################################
//#############################################################################################################################################################################
//CODE:
int stpRev = 200;
int dPinH = 4;
int sPinH = 5;
int ePinH = 6;

int dPinV = 9;
int sPinV = 8;
int ePinV = 6;

Stepper * m_Horizontal = new Stepper(stpRev, dPinH, sPinH);
Servo * m_servo = new Servo;
Stepper * y_Horizontal = new Stepper(stpRev, dPinV, sPinV);
Erace Erace_Board = Erace(m_Horizontal, y_Horizontal, m_servo);
String myCommand = "/";


void setup() {
  // put your setup code here, to run once:
  m_servo->attach(11);

  Wire.begin(15);
  Wire.setClock(200000);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  /*String command;
  Serial.flush();
  if (Serial.available()>=2)
  {  command = Serial.readString();
     //Serial.println(Find_Command(command));
     //Serial.println(command);
     command_call(Find_Command(command),command);
  }*/
  
  /*String letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  double x_pos;
  int write_at = 20;*/
  Serial.println(myCommand);
  if (myCommand.length() != 1)
  {
      int CVal = Find_Command(myCommand);
      command_call(CVal,myCommand);
  }
  myCommand = "/";
}


//RECIEVED INFORMATION EVENT REPLACES LOOP

void receiveEvent(int howMany) {
  int TotAval = Wire.available();
  myCommand = "";
  while (0<Wire.available()) {
    char c = Wire.read();
    myCommand = myCommand + String(c);
  }
  Serial.println(myCommand);
}
//###########################Function Definitions #######################
//#######################################################################
String Convert(String Cord)
{
  int CLength = Cord.length();
  String rVal="";
  for (int i=0; i<CLength;i++)
  {
    if (!(Cord[i] == '/'))
    {
      rVal = rVal+Cord[i];
    }
  }
  return rVal;
}
String Find_section(String command)
{
  int cLength = command.length();
  return command.substring(5,cLength);
}
int Find_Command(String Command)
{
  String Cord = "Cord=/";
  String Origin = "Orig=/";
  String Position = "Posi=/";
  String Write = "Writ=/";

  String command = Command.substring(0, 6);

  if (!command.compareTo(Cord))
  {
    return 1;
  }
  else if (!command.compareTo(Origin))
  {
    return 2;
  }
  else if (!command.compareTo(Position))
  {
    return 4;
  }
  else if (!command.compareTo(Write))
  {
    return 5;
  }
  else
  {
    return 0;
  }
}
int* Find_position(String command,int m_pos_array[])
{
  int index_of_period = command.indexOf('.');
  String xpos = command.substring(6, index_of_period+1);
  String ypos = command.substring(index_of_period + 1);
  int x_pos = xpos.toInt();
  int y_pos = ypos.toInt();
  m_pos_array[0] = x_pos;
  m_pos_array[1] = y_pos;
  return m_pos_array;
}
void command_call(int command_value, String command)
{
  switch (command_value)
  {
  case 0:
    break;
  case 1: {
    String Esection = Find_section(command);
    Esection = Convert(Esection);
    Erace_Board.Start_Erace(Esection);
  }
    break;
  case 2:
    Erace_Board.SetPosition(cord(0, 0));
    break;
  case 4: {
    int PosArr[2];
    int* Position = Find_position(command,PosArr);
    Erace_Board.SetPosition(cord(PosArr[0], PosArr[1]));
  }
    break;
  case 5: {
    writeINPUT(command.substring(6),30);
  }
    break;
  }
}

void writeINPUT(String IS,int SL)
{
  
	for(int i = 0; i<IS.length();i++)
	{
		if (Erace_Board.GetPosition().x+2 > 33)
		{
			SL = SL-3;
			Erace_Board.SetPosition(cord(0,SL));
		}
		else if (IS[i] == ' ')
		{
			Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
		}
		else
		{
			Erace_Board.write_Letter(IS[i],SL);
      Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+.5,Erace_Board.GetPosition().y));
		}
	}
	Erace_Board.SetPosition(cord(0,0));
}


void Erace::write_Letter(char n,int TopPos)
{ //C,E,F,G,H,I,J,L,O,P,Q,S,T,U,V,Y,Z
  switch (n)
  { 
    case 'A': //////////////// 
    SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y-2));
    delay(500);
    serov_setPosition(20); //engage
    delay(500);
    write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y+2) , 1, 4);
    write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y-2) , 1, -4);
    serov_setPosition(90);  //disengage
    delay(500);
    SetPosition(cord(Erace_Board.GetPosition().x-0.75, Erace_Board.GetPosition().y+1));
    serov_setPosition(20); //engage
    delay(500);
    SetPosition(cord(Erace_Board.GetPosition().x+0.5,Erace_Board.GetPosition().y));
    serov_setPosition(90);
    SetPosition(cord(Erace_Board.GetPosition().x+0.5,Erace_Board.GetPosition().y+1));
    break;

    case 'B': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(20);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.7, Erace_Board.GetPosition().y));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.3, Erace_Board.GetPosition().y-0.3) , 1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y-0.4));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.3, Erace_Board.GetPosition().y-0.3) , -1, -1);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.3, Erace_Board.GetPosition().y-0.3) , 1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y-0.4));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.3, Erace_Board.GetPosition().y-0.3) , -1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.7, Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.serov_setPosition(20);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.7, Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.3,Erace_Board.GetPosition().y+1));
    break;


    case 'C': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.serov_setPosition(20);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.7, Erace_Board.GetPosition().y));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.3, Erace_Board.GetPosition().y-0.3) , -1,-1); 
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y-1.4));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.3, Erace_Board.GetPosition().y-0.3) , 1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.7, Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    
    case 'D': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500); 
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.7, Erace_Board.GetPosition().y));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.3, Erace_Board.GetPosition().y-0.3) , 1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y-1.4));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.3, Erace_Board.GetPosition().y-0.3) , -1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.7,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    break;

    case 'E': ///////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    case 'F': ////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y+2));
    break;
    
    case 'G': //////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.5,Erace_Board.GetPosition().y+1));
    break;
    
    case 'H': ///////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;
    
    case 'I': ///////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.serov_setPosition(90);
    delay(500);  
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-.5,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-.5,TopPos-2));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    case 'J': //////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y+2));
    break;

    case 'K': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,
    Erace_Board.GetPosition().y-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-1, Erace_Board.GetPosition().y-1) , -1, -1);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y-1) , 1, -1);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;
    
    case 'L': /////////////////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    case 'M': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y+2), 1, 8);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y-1), 1, -4);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y+1), 1, 4);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y-2), 1, -8);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    case 'N': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,
    Erace_Board.GetPosition().y-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y-2) , 1, -2);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;
    
    case 'O': ////////////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    break;
    
    case 'P': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y+2));
    break;

    case 'Q': //////////////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+0.5));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+0.5));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    break;

    case 'R': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y+2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.7, Erace_Board.GetPosition().y));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.3,Erace_Board.GetPosition().y-0.3) , 1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-0.4));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.3, Erace_Board.GetPosition().y-0.3) ,-1, -1);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.7, Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.5,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y-1) , 1, -2);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;

    case 'S': ///////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y+1));
    break;
    
    case 'T': /////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,TopPos));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-0.5,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+0.5,Erace_Board.GetPosition().y+2));
    break;
    
    case 'U': ///////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos-2));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1,Erace_Board.GetPosition().y));
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;

    case 'V': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y-2), 1, -4);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y+2), 1, 4);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;

    case 'W': ////////////////
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y-2), 1, -8);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y+2), 1, 8);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y-2), 1, -8);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.25, Erace_Board.GetPosition().y+2), 1, 8);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;

    case 'X':
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y-2), 1, -2);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x-1,Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y+2), 1, 2);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y));
    break;
    
    case 'Y':
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y-1) , +1, -2);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x+0.5, Erace_Board.GetPosition().y+1) , 1, 2);
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-0.5, Erace_Board.GetPosition().y-1) , -1, -2);
     Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y-1));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+.5,Erace_Board.GetPosition().y+2));
    break;

    case 'Z':
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,TopPos));
    Erace_Board.serov_setPosition(30);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y));
    Erace_Board.write_diag(cord(Erace_Board.GetPosition().x, Erace_Board.GetPosition().y), cord(Erace_Board.GetPosition().x-1, Erace_Board.GetPosition().y-2) , -1, -2);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x+1, Erace_Board.GetPosition().y));
    Erace_Board.serov_setPosition(90);
    delay(500);
    Erace_Board.SetPosition(cord(Erace_Board.GetPosition().x,Erace_Board.GetPosition().y+2));
    break;                   
}
}


