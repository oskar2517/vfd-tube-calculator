# Homemade IV-12 VFD tube calculator

I have long been interested in working with microcontrollers, and I recently decided to start a project that incorporates them. I chose to build a simple calculator, using IV-12 VFD (Vacuum Fluorescent Display) tubes for the display. These tubes, with their vintage look, offer a unique alternative to modern displays. In this post, I will walk you through my process of creating this calculator, from the initial idea to the finished product.

## Links

I have made the designs I created for this project public.

- [Case on OnShape](https://cad.onshape.com/documents/a4ae1405305666595e09043b/w/0e8e4ae062d4480707db0af8/e/1a3de19df3b258f3a9cb75c0?renderMode=0&uiState=665d2b0371c1f274a04f2a77)
- [Mainboard on EasyEDA](https://oshwlab.com/oskar2233/rechner_pcb)
- [Keyboard STL file](keyboard/keyboard.stl)

## The Idea

I have been fascinated with [Nixie tubes](https://en.wikipedia.org/wiki/Nixie_tube) ever since I first learned about them in a [YouTube video](https://youtu.be/s4-pgzyT0No?si=MB0ZcV2Ts6cme0ZN) by Techmoan. Today, I own a beautiful Nixie clock with four IN-14 tubes and a Sharp Compet 23 desk calculator from the 1960s. I have wanted to build my own device using Nixie tubes for a long time, but as someone with very little experience in electrical engineering, I have always found the high voltages required to drive them somewhat daunting. So I began searching for alternative display technologies with a similar charm and came across VFD tubes. These largely replaced Nixie tubes in the 1970s and required much lower operating voltages. VFD tubes from old stock are still readily available in various shapes and sizes on eBay.

I did not want to build another clock, and I thought a calculator might be a bit more involved but still a manageable first project. After some research, I settled on using Soviet-made IV-12 tubes, mainly because I liked their size. They appear to be very similar to the more popular (?) IV-11 tubes but lack the segment for a decimal point and have proper pins that can be used with a tube socket, so I would not have to solder them in place. That does mean, however, that my calculator would not be able to display decimal numbers which I thought was fine.

<img src="assets/sharp_compet.jpg" width="500px">

## Driving an IV-12 Tube

<img src="assets/iv12_tubes.jpg" width="500px">

Image by eBay Seller "USSR Vacuum tubes and Soviet Valves" (https://www.ebay.com.au/itm/155079214689)

Before I went ahead with the project, I wanted to make sure I knew how to drive the tubes. Some research revealed the following information.

The pins of an IV-12 tube are keyed so they can only be socketed in the correct orientation. The image below shows the pinout of an IV-12 tube when looking at it from above.

<table>
    <tr>
        <td>
            <img src="assets/iv12_pinout.png" width="200px">
        </td>
         <td>
            <img src="assets/display_segments.png" height="230px">
        </td>
    </tr>
</table>

Pins `a`-`f` are connected to the anodes that form the segments of the digit. They follow the conventional segment names still used in modern 7-segment LCDs. To enable a segment, a positive potential of 25 volts has to be applied to its corresponding pin. To turn it off, a slightly negative potential should be applied instead but in my testing, leaving a segment floating reliably disabled it as well.

When  looking into a tube, you will notice a fine mesh formed from thin wire in front of the segments. It is connected to the `grid` pin and, when connected to positive potential of 25 volts, will enable the tube. When it is left floating or connected to a negative potential, all segments will remain disabled even when a positive voltage is applied to the anodes. This allows for multiplexing of the tubes.

When looking even closer, you may also notice two extremely thin wires in front of the grid. This is the so-called filament and acts as the tube's cathode. When connected to 1.5 volts (polarity irrelevant), the filament will heat up and start emitting electrons. The positively charged grid will accelerate them towards the segments behind it. In front of the anodes that form the segments lies a phosphor-coated substrate. When a positive potential is applied to an anode, the electrons will be further accelerated towards it until they hit the phosphor and cause it to light up (similar to a CRT). The segment is now illuminated. 

**Note:** For best performance, the filament should be driven by AC, as DC can apparently lead to uneven illumination. However, I have not noticed any brightness gradients when running them on DC, and assume that smaller tubes like these are not as affected by this issue as the [rare giants](https://www.youtube.com/watch?v=4tHa7A-6Uuk&t=911s).

No current-limiting resistors are required to drive a tube. IV-12 tubes require very little current (110mA for the filament, 12mA for the grid, 4mA for each segment) so driving them from a low-power source like a AA battery is possible when the voltage is properly regulated.

### Multiplexing the Tubes

When driving multiple tubes simultaneously, it is generally recommended to use multiplexing to reduce the number of data pins required to display a number. Normally, the seven segment pins of each tube would need to be connected individually to the data source (in this case, a microcontroller). With multiplexing, however, the same segment pin on every tube is connected in parallel with the corresponding pin on every other tube. For example, all the `a` pins would be connected together. This way, only seven data lines need to be connected to the microcontroller.

To prevent all tubes from displaying the same digit, the grid pin of each tube must be connected to the microcontroller. The microcontroller can then scan through the tubes by setting only one `grid` pin to 25 volts at a time and writing the digit it should display to the data lines. When driven at a sufficiently high frequency it will appear as if all tubes are on simultaneously to the human eye.

<img src="assets/multiplex_logic.png" width="500px">

## Designing the and Building the Keyboard

I started by designing the calculator's keypad for which I wanted to use regular key caps and Cherry MX switches. After a bit of research, I found [this tutorial](https://www.instructables.com/Arduino-Mechanical-Keypad-1/) on building a keypad using an Arduino. I followed the instructions and designed a backplate which I had 3D printed.

<img src="assets/keypad_model.png" width="500px">

I ordered Cherry MX Brown switches from Amazon and a set of key caps from AliExpress. Since there is no key which only contains an equals sign on a regular US keyboard, I had to substitute it with a one that had a cute little rocket on it. I could have had custom key caps printed but I did not think it would be worth it.

<img src="assets/keypad_keys.jpg" width="500px">

## Designing the Case

Next, I designed a case for the calculator. Having no experience in 3D CAD design, I initially experimented with Fusion 360. However, I found it to be extraordinarily cumbersome and buggy. So, I decided to give OnShape a try, which I had initially avoided simply because so many YouTube videos were being sponsored by it. To my surprise, it actually turned out to be great. With the help of [this tutorial](https://www.youtube.com/watch?v=YPoJ484-7tI), I was quickly able to create a 3D model of a case which would easily fit all the required components.

<img src="assets/case_model.png" width="500px">

I layed out the pieces my case would be constructed from and sent them off to a laser cutting company to be cut from 3mm plywood.

<img src="assets/case_parts.png" width="500px">

## Building the Case

My laser cut parts arrived a few days later. After getting my okay, the pieces were re-arragnged by the supplier to fit smaller plywood plates since they ran out of the larger ones I designed the cutting instructions for.

<table>
    <tr>
        <td>
            <img src="assets/laser_orig.jpg">
        </td>
         <td>
            <img src="assets/laser_pieces.jpg">
        </td>
    </tr>
</table>

All the pieces fit together nicely and I was soon left with a fully assembled case.

<img src="assets/case_assembled.jpg" width="500px">

I did not feel like the aesthetic of the raw plywood was quite what I was going for so I decided to try something I had always been hesitant about: veneering. I ordered a few sheets of .7mm walnut veneer from Amazon and looked up some veneering tutorials. When they arrived a few days later, I promptly got started. My plan was to cover one side of the box with a thin layer of wood glue, clamp on the veneer, let it cure, and then trim the excess with a scalpel and sandpaper. This method worked surprisingly well.

<table>
    <tr>
        <td>
            <img src="assets/case_veneer_1.jpg">
        </td>
         <td>
            <img src="assets/case_veneer_2.jpg">
        </td>
    </tr>
</table>

I repeated the process for all six sides, ensuring the grain patterns aligned. After a few days of work, I had a fully-veneered case, though it still needed holes for the keypad and tubes. Once I had cut those out, I was quite satisfied with the result.

<table>
    <tr>
        <td>
            <img src="assets/case_veneered.jpg">
        </td>
         <td>
            <img src="assets/case_veneered_holes.jpg">
        </td>
    </tr>
</table>

The only thing left to do now was finishing the case with a few coats of linseed oil varnish. The oil beautifully enhanced the wood grain.

<img src="assets/case_finished.jpg" width="500px">

I later attached some rubber feet to the bottom plate so the calculator would not rest directly on the case's edges.

## Wiring the Tubes

Since I wanted the tubes to be replaceable in case they ever failed, I ordered a few matching ceramic tube sockets from AliExpress (thanfully, IV-12 tubes use a common 10 pin socket). Unfortunately, it seems the IV-12 tubes I purchased were not produced to the highest standards. All the pins were ever so slightly bent and did not fit into the sockets initially. However, after some careful work with a pair of pliers, I was able to resolve this issue. 

I constructed a longer cable by soldering together short pieces of wire, arranging them to facilitate easy soldering to the socket pins. I connected the tubes as shown in the schematic above, leaving the grid pin of each socket separate (green wires). The yellow wires connect all segments in parallel, while the red and black wires are connected to the filament. To prevent any shorts, I covered the exposed solder joints with heat-shrink tubing.

<table>
    <tr>
        <td>
            <img src="assets/wiring_1.jpg">
        </td>
         <td>
            <img src="assets/wiring_2.jpg">
        </td>
    </tr>
</table>


## The Main PCB

After a few unsuccessful attempts at wiring the grid for the keypad on a perfboard, I abandoned the idea and decided to design a custom PCB instead. Although I had never done anything like this before, it turned out to be quite straightforward. Using [EasyEDA](https://easyeda.com/), I took advantage of the community-provided footprints to design a board that would not only hold the keys but also the microcontroller, the DC-DC voltage converters required to drive the tubes, and the transistor arrays that would allow the Arduino to supply the necessary voltages.

<img src="assets/pcb_logic.png" width="500px">

After sketching out the logic, I converted it into a PCB of the required dimensions and with holes through which the entire assembly could be mounted to the case. I ensured the connectors for the keyboard switches were on the top and all the other components would be mounted on the bottom of the circuit board.

<img src="assets/pcb_board.png" width="500px">

I sent my files off to [JLCPCB](https://jlcpcb.com/) for production. Because I selected the transistor arrays from their library, they were able source the chips and reflow solder them to the board for me. About two weeks later, the partly assembled PCBs arrived at my doorstep. Note how I went with pins to connect the tubes. I crimped Dupont connectors to the exposed tube wires so they could be easily attached to the PCB. 

<img src="assets/pcb_partly_assembled.jpg" width="500px">

The only thing I had to do was soldering on the microcontroller and the boost und buck converters that would provide the 1.5 volts for the filament and the 25 volts for the anodes and grids.

<table>
    <tr>
        <td>
            <img src="assets/pcb_final_bottom.jpg">
        </td>
         <td>
            <img src="assets/pcb_final_top.jpg">
        </td>
    </tr>
</table>

After assembling the main PCB, I mounted it to the case and connected the tubes. I later added a switch, a charge controller and a 21700 lithium battery that would allow me to use the calculator portably.

<table>
    <tr>
        <td>
            <img src="assets/pcb_connected.jpg">
        </td>
         <td>
            <img src="assets/pcb_with_battery.jpg">
        </td>
    </tr>
</table>

## Writing the Software


The only task remaining was to write the software to drive the calculator. This process went smoothly for the most part, as I consider myself a decent programmer with prior experience in C. However, I encountered one major hurdle that nearly drove me insane. Using the [keypad library](https://github.com/Chris--A/Keypad), one of the keypad rows simply would not register. Fortunately, a friend pointed out that digital pin 13 on the Arduino Nano is connected to an LED, which can cause issues when used as an input pin. Even after swapping the row and column pins, however, the keypad still malfunctioned, occasionally registering keys twice. To this day, I am unsure what exactly caused this behavior, but I was thankfully able to circumvent the problem by manually implementing the scanning algorithm.

```c
#define KEY_ROWS 4
#define KEY_COLS 4

char keys[KEY_ROWS][KEY_COLS] = {
  {'7', '8', '9', '*'},
  {'4', '5', '6', '+'},
  {'1', '2', '3', '/'},
  {'0', 'C', '-', '='}
};

void readKey() {
  static bool newKeyRead = false;
  static bool canPressKey = true;
  bool keyPressed = false;

  if (newKeyRead) {
    currentKey = '\0';
    newKeyRead = false;
  }

  for (int i = 0; i < KEY_ROWS; i++) {
    digitalWrite(keyRowPins[i], LOW);
    
    for (int j = 0; j < KEY_COLS; j++) {
      if (digitalRead(keyColPins[j]) == LOW) {
        if (canPressKey) {
          currentKey = keys[i][j];
          newKeyRead = true;
        }
        keyPressed = true;
      }
    }

    digitalWrite(keyRowPins[i], HIGH);
  }

  canPressKey = !keyPressed;
}
```

The full source code can be found [here](software/software.ino).

## The Result

I am very pleased with how this project turned out, especially the veneer, which exceeded my expectations. The calculator can perform integer addition, subtraction, multiplication, and division up to 99,999. It even produces a slight hum when turned on, which adds to its retro charm, although I am not exactly sure what causes it.

Here are some final pictures of the calculator in action.

<table>
    <tr>
        <td>
            <img src="assets/finished_1.jpg">
        </td>
         <td>
            <img src="assets/finished_2.jpg">
        </td>
    </tr>
    <tr>
        <td>
            <img src="assets/finished_3.jpg">
        </td>
         <td>
            <img src="assets/finished_4.jpg">
        </td>
    </tr>
    <tr>
        <td>
            <img src="assets/finished_5.jpg">
        </td>
         <td>
            <!-- -->
        </td>
    </tr>
</table>