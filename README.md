# MPA-NDE

Team members

1. Vít Charvátek (responsible for Pneumatics)
2. Vít Janoš (responsible for Electronics)
4. Antonín Putala (responsible for Software)

> [!WARNING]
> The component list will have been completed by February 25.

## Requirements
1. Determining position using a sensor
2. Compressed air drive
3. Maintaining a constant position on one axis
4. Instruction for rotation of 90°
5. Battery supply
6. Wireless control

## Task list
- [ ] System
   * [x] Conceptual diagram
   * [x] Requirements
   * [ ] Energy budget
   * [ ] Component list
- [ ]  Electronics
   * [x] Component choice 
      - [x] Battery
      - [x] Step-down
      - [x] Accelerometer
   * [x] Circuit design
      - [x] LED indicator
      - [x] Switching circuit
   * [ ] PCB design
   * [ ] Soldering
- [ ]  Pneumatics
   * [x] Component choice 
      - [x] Valve
      - [x] T-hub
      - [x] Tube
   * [ ]  Gas bottle construction
- [ ] Software
   * [x] Component choice 
       - [x] MCU
       - [x] Wireless module
   * [ ] UART communication
       - [ ] Sensor data
       - [ ] Valve control
       - [ ] Command implementation
   * [ ] Wireless communication
- [ ] Structure
   * [ ] Size estimation
   * [ ] Structural design

## Sharepoint
[Part list](https://vutbr-my.sharepoint.com/:x:/r/personal/246858_vutbr_cz/Documents/MPA-NDE%20Part%20List.xlsx?d=wf46451cfd72a41359eeacaa9987770c3&csf=1&web=1&e=Fm6BbQ)

## Reference
1. [Arduino HC-05](https://arduinogetstarted.com/tutorials/arduino-bluetooth#google_vignette)
2. [Calculator - Potencial Energy of Compression](https://codingace.net/physics/potential_energy_of_compressed.html)
3. [Calculator - Dynamic Viscosity](https://www.engineeringtoolbox.com/air-absolute-kinematic-viscosity-d_601.html)
