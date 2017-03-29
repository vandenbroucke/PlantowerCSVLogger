# PlantowerPMInterface
The main goal of this repo is reading out Plantower™ sensors and providing a serial CSV-formatted stream for fast and simple data capture without having to worry about any additional incoming data.

## Interface connections (Sensor & Arduino)
The following connections are required in order for this build to function properly. For this example a [Plantower](http://www.plantower.com/) PMS7003 particulate matter sensor was used.

![Arduino micro plantower pms7003 setup](https://portalvhds601hqnwxgfhc9.blob.core.windows.net/cdn/media/github_plantower_setup "Arduino micro plantower pms7003 setup")


| Sensor (Plantower)| Arduino           |Cable color|Additional info|
| ----------------- |:-----------------:|-|---------|
| SET               | Pin 10 (BY CHOICE)|White|Keeping pin on HIGH to avoid the sensor pause|
| TX                | RX                  |Green| |
| VCC               | VCC(5V)             |Purple| |
| GND               | GND                 |Orange| |

## CSV formatting style
The serial output stream is formatted as follows:

**PM1.0;PM2.5;PM10;Time(ms);**

PM (Particulate Matter) values indicate the fraction of suspended particles also called aerosols with an aerodynamic diameter of less than, respectively, 1, 2.5 and 10 µm (microns). The concentration of particles is expressed in ug/m³.

The 4th parameter indicates the time expressed in milliseconds since the device started measuring.
This representation makes it easier to generate graphs from the CSV data.


Mainly based upon the [PMS7003-on-Particle](https://github.com/MartyMacGyver/PMS7003-on-Particle) repository of [Marty MacGyver](https://github.com/MartyMacGyver) as this was the most reliable one available at the time of creation of this repository.




