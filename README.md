# Rover1
I learned the basics of driver development with tutorials and previous projects and wanted to create something using only datasheets to develop the needed drivers for a rover with wireless capabilities. The system works as follows. The rover waits for commands it gets over the antenna and executes them, and these might include transmitting back data. For the “command station” I quickly set up an Arduino and preexisting library for the second nRF24 antenna. I wrote some code to send the commands that are typed into the Arduino’s built in serial interface. 

Wrote some drivers to get a rover running and taking commands wirelessly. 
Put together a prototype of the rover to see if receive and follow commands wirelessly (though power is still from usb for now).

A very short video here shows it after recieving a wireless command to "navigate". I put a very simple algorithm for that, use sonic sensor to check of object is nearby, if yes, then turn, if not then drive forward. It's not quite there yet but the bones are all set up.
https://user-images.githubusercontent.com/64346135/188251362-eb3029d0-292e-473b-81ca-2807b02d660c.mp4

