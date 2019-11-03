# WarlockStaff

![WarlockStaff Idle](/assets/staff1.gif)

This is the source code for a FastLED + Arduino animated staff I built as part of a Warlock costume.  The staff has the following features:
 - It's fun!
 - If the user double taps the staff against the ground it erupts into fire!
 - If the user holds the staff horizontally (shooting antics not specifically required, but really help sell the light show) the staff goes into "shooting" mode.
 - It's FastLED based and supports variable number of light strips (I used 3).
 - The default executable (3 strips of 100 LEDs) is *20588 bytes in size*, and uses *2192 bytes of RAM*!
 - It runs well on a 16MHz chip @ 120FPS, but some animations max the CPU, so slower chips are not recommended.
 
 Here's the staff's double tap animation:

![WarlockStaff DoubleTap](/assets/staff3.gif)

Here's what the staff looks like when you "shoot" it (hold it sideways):

![WarlockStaff Shooting](/assets/staff2.gif)

## Design
The design I used is as follows:
 - 5v Arduino Micro controls strips of 5v WS2812B individually addressable LEDs
 - 3.3v Arduino Nano 33 BLE Sense handles processing its on board accelerometer and does all of the gesture detection computations
 - The two Arduinos communicate with each other via their extra onboard serial ports (Serial1)

### Why 2 Arduinos?
There's a few reasons for this.  The Nano is considerably more powerful and capable than the Micro, however it's only 3.3v and can't directly drive 5v strips without a level shifter.  But, more importantly, at the time this project was created the authors of FastLED had not ported the library to the Nano 33 BLE.  Also, I did this project after the kids went to bed, and I may have had a few drinks at several key points in the design process.

## Parts list
 - 1 Stick (5ft long or so).  I grabbed one from the yard, cut off the branches, and sanded it.
 - Arduino Micro (drives the 5v LED strips)
 - Arduino Nano 33 BLE Sense (reads and processes accelerometer data)
 - WS2812B LEDs (I used a single 5m roll of 300 LEDs -- cut into 3 strips of 100 LEDs)
 - A small portable battery pack
   + I used a cheapo 5600mAh one, but anything that can pump out 2-3a from a single USB port should do the trick.  Each LED at full brightness is ~60ma, which means in theory you might need 18 amps from the battery.  I turned the brightness down, and none of the animations I programmed make all the lights go full white at full bright.  (I designed it to work with a small battery pack as I knew this thing would need to be mobile).
   + Even the small 5600mAh battery I used is enough to power the 2 arduinos and the light strips for 5+ hours -- more than enough time to walk around making the kids collect candy for me.
 - 1 male normal old school big USB cable you don't mind cutting up to use as power delivery
 - 2 mini USB cables to send power to the arduinos
 - 10 pack of 3-pin JST SM Male-Female LED connector cables for WS2812B strips (these are not strictly needed but they came in real handy!)
 - 2 half or quarter sized perma bread boards
 - Soldering iron
 - Electrical tape
 - Some kind of cool mystical topper to make the staff look awesome, and to hold all the hardware.  I asked my friend Brian, who is a glass blower, to make me one.  

## Step 1 -- Preparation
 - Solder the Arduinos to their breadboards
   + If you're like me and you haven't soldered in a while, and you have a new iron, don't forget to tin the tip!  (Take a YouTube refresher course)
 - Solder the 3-pin connectors to the newly cut LED strips
 - Cut up your big USB cable that you intend to use as main power delivery.  All you need to do is cut off the end that's not going to the battery so you have some bare wires exposed.  Cut away all of them except *red* and *black* -- these are the power wires!  Strip and test them with a multimeter to ensure the battery is delivering +5v!  I had one battery that was wired incorrectly and, while it worked for charging devices, the LED strips didn't like the -5v it put out.
 
## Step 2 -- Loading the code onto the chips
 - Clone the repository
 - There are 2 .ino files:
   + WarlockStaff.ino -- This is the LED controller code that goes onto the Micro
   + WarlockStaffSense.ino -- This is the accelerometer gesture detection code that goes onto the BLE Sense
 - Open WarlockStaff.ino, connect the Micro (reminder to set the board type to Micro), upload the code, attach the serial monitor and make sure you see output.
   + You may need to change Settings.h!  You can set various things such as how many LED strips and how many LEDs per strip you're using, as well as frame rate, and which pins you want to use.
 - Open WarlockStaffSense.ino, connect the BLE Sense (again, reminder to set the board type), upload the code, attach the serial monitor.  You should see some output on the serial port when you move the board around.
 
The only trouble you might run into with this step is if you're using a different Arduino than I used, or you've forgotten to set the board type correctly.

## Step 3 -- Wiring harness

![Wiring Harness](/assets/WiringHarness.jpg)

This is without a doubt, the biggest time sink, and the most important step.  Connecting everything with the 3-pin JST connectors is real handy, and highly recommended.

### Step 3a -- LED Strips

Here's the basic design:
 - Data (green) wires from the 3 LED strips are wired to 3 of the Micro's pins
 - *THIS IS IMPORTANT*: The negative line from the LED strips needs to be connected to both the negative from your powersupply AND the Arduino's ground!
   + Negative is going to be spliced 4 ways: one to each LED strip (3 in total), and one to the ground pin on the Micro
 - Positive is going to be split 3 ways: one to each LED strip

Be careful to note the wiring order of your particular LED strips!  Not all manufacturers make them the same.  You'll probably have recieved a diagram with your strips that looks like this:

![Wiring Harness](/assets/leds_Wiring-Diagram.png)

I tested without the resistor and capacitor and it works fine with the strips I used, but YMMV.

1) Solder female 3-pin JST connectors to the cut LED strips
1) Solder a green data line wire to whatever pins on the Micro you prefer.  I chose 12, 6, 4 but this is easily changed -- see Settings.h!
1) Splice in the male 3-pin connectors to the green data wires you just soldered, along with power (red) and negative (black) from the power supply.
1) BEFORE THIS STEP: double check your voltage on positive and negative is correct.  Measure twice, solder / order LED strips once.  Now, connect the male and female connectors together!

*At this point you should see your strips lighting up!*

If you've gotten this far and there's nothing lighting up:
 - Double check the serial monitor on the Micro is showing FPS updates every second.
 - Double check you've gotten the data pins set correctly in Settings.h (do the pins set there match your soldered wires?) -- look at your pinout diagram of the Arduino.  Datapin 6 corresponds to D6 on the pinout, not the 6th pin, or A6, etc.
 - Double check that you've got a shared negative line between all LED strips and the ground pin on the Arduino.
 - You're supposed to have already done this but just in case: triple check everything with your multimeter.
 - If you're still here scratching your head wondering if you should add a resistor to the circuit.  The resistor should only be necessary to reduce signal bounce, so if the lights are blinking but in a weird random way, add a resistor.

### Step 3b - Board communication

The two Arduinos need to communicate.  No problem, they both have a built in secondary serial port just for this purpose!

- Connect the TX1 pin on the Sense directly to the RX1 pin on the Micro
- Verify that they're communicating.  When you rotate the sense 90 degrees you should see sense state change messages coming from serial port debug output on the Micro.

### Step 3c -- Power

Remember those two micro USB cables?  You need them now.  Cut these cables in a similar way to the main power cable, except this time you're keeping the micro USB end that is going to deliver power to the two Arduinos.  If you want you can wire directly to the power pins, but I like the flexibility of using USB power, especially since this has the added benefit of allowing the 5v input source directly into the 3.3v Sense.

 - Splice power from your main power source into your 2 micro USB cables
 - Connect them one at a time to verify they both still work after hooking them to battery power

## Step 4 -- Done!

That's it!  Just package up the harness, board, and battery nicely into something you can affix the the staff, and now you've got your very own mobile light show.