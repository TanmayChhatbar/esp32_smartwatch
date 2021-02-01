
# [WIP]esp32_smartwatch
DIY smartwatch. Features time(from wifi), step count, calorie count(experimental) so far. 

I am nowhere near a pro at programming, and this project is just an effort to refine my skills at it. Do recommend changes and point out bugs! 

# User Setup
### Variables you will want to set
	in Smartwatch.ino
		!define	bluetoothLogging
			DebuggingScreens
	in time.ino
		define 	(WiFi)credentials
			gmtOffset_sec
			daylightOffset_sec
			
### Variables you can play around with
	in tft.ino
		define 	screen colors
	in battery.ino
		define 	battery sizes

# Changelog
### v0.32 (Current)
	Minor fixes
	Better comments
	Rewrote walking code (around 90% accuracy)
	n-step Debounce for movement
	
v0.22
	
	Better screen switching
	Fix in walking time
	Updated instructions in flashing and ToDo list
	Code cleanup
		
v0.21
	
	Easy toggle for bluetooth logging and debugging screens	
	Automatic screen detection	
	Fix in walking

Changelog being maintained since v0.2

# Libraries used
  	TFT_eSPI.h - https://github.com/Bodmer/TFT_eSPI
  	SPI.h - built-in
  	WiFi.h - with esp32 
  	time.h - built-in
  	Wire.h - built-in

# Hardware used
  	TTGO T-Display
  	GY-521
  	400mAh LiPo
  	Case STL here: https://www.thingiverse.com/thing:4730124

# Credit
	Must credit all those who have written those extensive libraries as part of the effort in this project.
	Without them, I couldn't have reached so far.


