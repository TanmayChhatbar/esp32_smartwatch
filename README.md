
# [WIP]esp32_smartwatch
DIY smartwatch.

I am nowhere near a pro at programming, and this project is just an effort to refine my skills at it. Do recommend changes and point out bugs! 

![83851rdyp8c61](https://user-images.githubusercontent.com/14368465/106637627-f688be80-65a8-11eb-9b04-b9190dd37171.jpg)
![qv8iawr2q8c61](https://user-images.githubusercontent.com/14368465/106637832-3059c500-65a9-11eb-8190-96edd26f87b8.gif)

# Features 
	Time (from WiFi)
	Step count
	Calorie count (experimental so far) 
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

### v0.4 (Current)
	
	Fix for font size when steps go into 5 digits
	Better movement timeout
	Optimized range of values for walking
	Added media to readme.md
	
v0.33
	Walking bug fix
	Better comments

v0.32

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



