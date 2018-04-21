#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

boolean backlight = false;
int contrast = PWMRANGE;

int menuitem = 1;
int page = 1;
//#define  BACKLIGHT  D0
volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;

int downButtonState = 0;
int upButtonState = 0;
int resetButtonState = 0;
int selectButtonState = 0;
int lastDownButtonState = 0;
int lastSelectButtonState = 0;
int lastUpButtonState = 0;

Adafruit_PCD8544 display = Adafruit_PCD8544(D6, -1, -1); //48�84

void checkIfDownButtonIsPressed()
{
	if (downButtonState != lastDownButtonState)
	{
		if (downButtonState == 0)
		{
			down = true;
		}
		delay(50);
	}
	lastDownButtonState = downButtonState;
}

void checkIfUpButtonIsPressed()
{
	if (upButtonState != lastUpButtonState)
	{
		if (upButtonState == 0) {
			up = true;
		}
		delay(50);
	}
	lastUpButtonState = upButtonState;
}

void checkIfSelectButtonIsPressed()
{
	if (selectButtonState != lastSelectButtonState)
	{
		if (selectButtonState == 0) {
			middle = true;
		}
		delay(50);
	}
	lastSelectButtonState = selectButtonState;
}
void SetMenuColor(bool currentMenu)
{
	display.setTextColor(currentMenu ? WHITE : BLACK, currentMenu ? BLACK : WHITE);
}


void drawMenu()
{
	if (page == 1)
	{
		display.setTextSize(1);
		display.clearDisplay();
		display.setTextColor(BLACK, WHITE);
		display.setCursor(0, 0);
		display.print("ESP8266 Timer");
		display.drawFastHLine(0, 10, 83, BLACK);
		display.setCursor(0, 15);


		SetMenuColor(menuitem == 1);

		display.print("Delay: ");
		display.print(contrast);
		display.setCursor(0, 25);

		SetMenuColor(menuitem == 2);
		display.print("Light: ");
		display.print(backlight ? "ON" : "OFF");

		display.display();

		SetMenuColor(menuitem == 3);
		display.setCursor(0, 35);
		display.print("Start");
		display.display();
	}
	else if (page == 2)
	{
		display.setTextSize(1);
		display.clearDisplay();
		display.setTextColor(BLACK, WHITE);
		display.setCursor(15, 0);
		display.print("DELAY");
		display.drawFastHLine(0, 10, 83, BLACK);
		display.setCursor(5, 15);
		display.print("Value");
		display.setTextSize(2);
		display.setCursor(5, 25);
		display.print(contrast);

		display.setTextSize(2);
		display.display();
	}
}
void setContrast()
{

}
void turnBacklightOn()
{
	contrast = 0;
	setContrast();
}

void turnBacklightOff()
{
	contrast = PWMRANGE;
	setContrast();
}

void resetDefaults()
{
	contrast = PWMRANGE;
	setContrast();
	backlight = false;
}

void setup() {

	pinMode(D1, INPUT_PULLUP);
	pinMode(D2, INPUT_PULLUP);
	pinMode(D3, INPUT_PULLUP);
	pinMode(D4, INPUT_PULLUP);

	setContrast();
	Serial.begin(9600);

	display.begin();
	display.setRotation(2);
	display.clearDisplay();
	display.display();
	drawMenu();
}

void loop() {



	downButtonState = digitalRead(D3);
	selectButtonState = digitalRead(D2);
	upButtonState = digitalRead(D1);
	resetButtonState = digitalRead(D4);

	checkIfDownButtonIsPressed();
	checkIfUpButtonIsPressed();
	checkIfSelectButtonIsPressed();

	if (up && page == 1) {
		up = false;
		menuitem--;
		if (menuitem == 0)
		{
			menuitem = 3;
		}
		drawMenu();
	}
	else if (up && page == 2) {
		up = false;
		contrast--;
		if (contrast < 0) contrast = 0;
		setContrast();
		drawMenu();
	}

	if (down && page == 1) {
		down = false;
		menuitem++;
		if (menuitem == 4)
		{
			menuitem = 1;
		}
		drawMenu();
	}
	else if (down && page == 2) {
		down = false;
		contrast++;
		if (contrast > PWMRANGE) contrast = PWMRANGE;
		setContrast();
		drawMenu();
	}
	if (!resetButtonState) {
		//    resetDefaults();
		//     drawMenu();
	}

	if (middle) {
		middle = false;

		if (page == 1 && menuitem == 2)
		{
			if (backlight)
			{
				backlight = false;
				turnBacklightOff();
			}
			else
			{
				backlight = true;
				turnBacklightOn();
			}
		}

		if (page == 1 && menuitem == 3)
		{
			resetDefaults();
		}
		else if (page == 1 && menuitem == 1) {
			page = 2;
		}
		else if (page == 2) {
			page = 1;
		}
		drawMenu();
	}
}

