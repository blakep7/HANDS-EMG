# HANDS-EMG

## Hand Activity Neural Detection using SEMG

## Project Description
A battery-powered surface electromyography (sEMG) sensor system designed to classify hand movements based on the wearer's muscle activity.
The device utilizes four channels of wet electrodes placed on the user's forearm to capture sEMG signals.
These signals are processed through our procured analog front-end module operating at a minimum of 2000 samples per second.
The digitized signals are transmitted to our microcontroller via SPI, where a machine learning algorithm classifies the hand movements based on a pre-trained model.
Our responsibilities include configuring the analog front-end module, programming a low-power microcontroller, integrating the system on a custom PCB, and training the machine learning model for accurate classification.
The system will output the movement classifications via USB, which can be visualized on a PC simulator.
Designed for efficiency, the device operates at a voltage of 3.7V provided by a rechargeable lithium polymer battery for portability.

## Contributors

- Kelly Hubbard
- Noah Marosok
- Blake Pearson
- Jayden Sumbillo
- Kirk Young

## License 📜

This project is licensed under the [Apache License 2.0](LICENSE).

