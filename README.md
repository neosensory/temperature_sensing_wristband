# Temperature Sensing Wristband
An Arduino script that uses the [Neosensory Bluefruit SDK](https://github.com/neosensory/neosensory-sdk-for-bluefruit) to send temperature data to Buzz.

## Setup
[This tutorial](https://neosensory.com/blog/temperature-sensing-wristband/) will walk you through assembling the hardware required for this project and uploading the code. To compile this Arduino sketch, you must also install the [Plotter library](https://github.com/devinaconley/arduino-plotter) from Devin Conley, which you can do via Arduino's Library Manager. 

## Pairing
Once you have created the temperature sensing sensing wristband and uploaded the code, turn on your Buzz wristband and press and hold the plus and minus buttons on top of your Buzz to put it in pairing mode. Your temperature sensing wristband should quickly connect to your Buzz.

## Use
When connected to the temperature sensing wristband, your Buzz wristband will vibrate to indicate when there is a change in detected temperature from the two contactless thermometers. Each thermometer is connected to a different vibrating motor on Buzz, so as long as the thermometers are pointed in different directions, you'll be able to detect multiple sources of temperature. The motors vibrate if the detected temperature deviates from that thermometer's running mean. The intensity of vibration indicates how much the temperature has deviated from the running mean. The vibrations also use different textures to indicate a positive or a negative deviation: a smooth vibration is a positive change (warmer) while a 'rough' or 'pulsating' vibration indicates a negative change (cooler).

Try moving something warm like a mug of coffee in front of one of the thermometers on your temperature sensing wristband. You should feel a strong, smooth vibration when you move your mug in front of the thermometer, which will decrease gradually as the thermometer's running mean adjusts to the warm mug. Moving your wrist away from the mug (after the running mean has adjusted) will be similar, except with a 'rough' vibration texture to indicate a decrease in temperature. 

## Plotting
This Arduino sketch is setup to plot the temperature signals for convenient visualizing. To use these plots, follow the [installation instructions](https://github.com/devinaconley/arduino-plotter/wiki/Installation-and-Quickstart#installation) on Devin Conley's Arduino Plotter repository. If you are not using the plots, you still must install the library in order to compile the code or instead you must remove the `plot.ino` file and the plot functions from the `temperature_sensing_wristband.ino` file.

## License
Please note that while this Neosensory SDK has an Apache 2.0 license, 
usage of the Neosensory API to interface with Neosensory products is 
still  subject to the Neosensory developer terms of service located at:
https://neosensory.com/legal/dev-terms-service.

See [LICENSE](./LICENSE).

## Author
Originally created by Mike Perrotta for Neosensory, Inc.

