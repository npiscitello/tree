# tree
Code for a blinky palm tree, which is really just a long strip of fairy lights
bent up in the shape of a palm tree. The fairy lights are wired as a bunch of
parallel LEDs with alternating polarity, so by switching the polarity, you
switch the set of lights that are lit. Poke some MOSFETs into an ATMEGA328 and
you have yourself some good old-fashioned blinkenlight!

Check out [avr-libc] (https://nongnu.org/avr-libc/) for the libs I use.

# dimming profiles
I decided I didn't like a strictly linear dim, so I played around with some
custom dimming profiles, implemented as a lookup table. Turns out, it starts
looking weird if you stray too far from linear, so I settled on the
circular_mod profile. This is, of course, for my sister, so if she doesn't like
it I'll keep tweaking! See the ODS spreadsheet for calculation details.
Remember, since we're using inverted PWM, an output of 255 is off and 0 is
fully on.
![dimming profile plot](dimming_profiles.png)
